class Target: public Runnable {
    Laser &laser;
    RgbLed &rgb;
    Arm &arm;
    Infra &infra;
    byte testButtonPin;
    unsigned char targetId;

    enum State {
      DISABLED = 0,
      DROPPED = 1,
      UP = 2,
      READY = 3
    } state;

  private:
    unsigned char gunShot;

    unsigned char priority() {
      return 2;
    }

    // Check if test button was clicked on emulator.
    bool testButtonClicked() {
      #ifndef INFRA_ENABLED
        if (this->infra.getPin() > 0) {
          return digitalRead(this->testButtonPin) == LOW;
        }
      #endif
      return false;
    }

    /**
     * Test mode.
     * Tests one target at a time, using the knob to change targets.
     */
    void test() {
      if (activeTarget != this->targetId) {
        return;
      }

      // Reset bad readings so that we can continue debugging targets.
      this->infra.reset();

      rgb.green();

      // Test button on emulator counts as player 1.
      if (this->testButtonClicked()) {
        this->gunShot = 1;
      }
      else {
        this->gunShot = infra.getShot();
      }

      // Check if target is shot.
      if (this->gunShot > 0) {
        // Update display.
        display.displayHit(this->gunShot, this->targetId);

        // Blink lights.
        laser.on();
        delay(200);

        laser.off();
        delay(200);

        rgb.red();
        delay(200);

        rgb.blue();
        delay(200);

        rgb.off();
        delay(100);

        // Update display.
        display.displayStatus("Shoot targets");
      }
    }

    /**
     * Play mode.
     */
    void play() {
      switch (this->state) {
      case DROPPED:
        if (arm.isOn()) {
          // Change state.
          this->state = UP;
        }
        else {
          rgb.off();
        }
        break;

      case UP:
        if (activeTarget == this->targetId) {
          // Blink laser.
          laser.on();
          delay(200);
          laser.off();
          delay(200);
          laser.on();
          delay(200);
          laser.off();
          delay(200);
          // Change stage.
          this->state = READY;
        }
        else {
          // @todo find out why blue turns off when laser is blinking and turn this on again.
          // rgb.blue();
        }
        break;

      case READY:
        if (activeTarget == this->targetId) {
          rgb.green();

          // On emulator, test button counts as player 1.
          if (this->testButtonClicked()) {
            this->gunShot = 1;
          }
          else {
            this->gunShot = infra.getShot();
          }

          // Check if target is shot.
          if (this->gunShot > 0) {
            // Update display.
            display.displayHit(this->gunShot, this->targetId);
            // Red light.
            rgb.red();
            // Drop arm.
            this->state = DROPPED;
            delay(100);
            arm.drop();
            // Special code to add random delay before next target selection.
            activeTarget = 254;
            // Update scores;
            scores[this->gunShot]++;
            delay(500);
            // Update display.
            display.displayScores();
          }
        }
        else {
          // Change state.
          this->state = UP;
        }
        break;
      }
    }

  public:
    Target(
        Laser &laserInstance,
        RgbLed &rgbInstance,
        Arm &armInstance,
        Infra &infraInstance
      ) :
        arm(armInstance),
        laser(laserInstance),
        rgb(rgbInstance),
        infra(infraInstance)
      {
        // Increase global counter;
        targetCount++;
        this->targetId = targetCount;
    }

    void setup() {
      this->state = DROPPED;

      laser.off();
      rgb.blue();

      // When infra is not enabled, we use the test button to shoot.
      #ifndef INFRA_ENABLED
        if (this->infra.getPin() > 0) {
          this->testButtonPin = this->infra.getPin();
          pinMode(this->testButtonPin, INPUT_PULLUP);
        }
      #endif
    }

    // Disables the target.
    void disable() {
      this->state = DISABLED;
      rgb.off();
      arm.reset();
    }

    void enable() {
      this->state = DROPPED;
      this->infra.reset();
    }

    boolean isDisabled() {
      return this->state == DISABLED;
    }

    boolean isReady() {
      return this->state == READY;
    }

    boolean isUp() {
      return this->state == UP;
    }
  
    void loop() {
      // When infra is not working correctly or arm is not available, disable and change target.
      if (infra.isDisabled() && !this->isDisabled()) {
        this->disable();
        disabledTargets[this->targetId] = true;
        activeTarget = 0;
        // Update display.
        display.displayStatus("Disabling " + String(this->targetId));
      }

      // Return if target is disabled.
      if (this->isDisabled()) {
        return;
      }

      // Test mode.
      if (GameState == TESTING) {
        this->test();
        return;
      }

      // Play mode.
      if (GameState == PLAYING) {
        this->play();
        return;
      }
    }
};
