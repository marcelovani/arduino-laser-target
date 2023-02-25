class Target: public Runnable {
    Laser &laser;
    RgbLed &rgb;
    Arm &arm;
    Infra &infra;
    byte testButtonPin;
    byte targetId;

    enum State {
      DISABLED = 0,
      DROPPED = 1,
      UP = 2,
      READY = 3
    } state;

  private:
    unsigned char gunShot;

  public:
    Target() {}

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
        this->testButtonPin = this->infra.getPin();
        pinMode(this->testButtonPin, INPUT_PULLUP);
      #endif
    }

    bool testButtonClicked() {
      #ifndef INFRA_ENABLED
        return digitalRead(testButtonPin) == LOW;
      #endif

      return false;
    }

    void disable() {
      this->state = DISABLED;
      rgb.off();
      arm.reset();
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
        activeTarget = 0;
      }

      if (!this->isDisabled()) {
        switch (this->state) {
          case DROPPED:
            rgb.off();
            if (arm.isOn()) {
              this->state = UP;
            }
            break;

          case UP:
            if (activeTarget == this->targetId) {
              laser.on();
              delay(200);
              laser.off();
              delay(200);
              laser.on();
              delay(200);
              laser.off();
              delay(200);
              //laser.blink();
              this->state = READY;
            }
            else {
              rgb.blue();
            }
            break;

          case READY:
            if (activeTarget == this->targetId) {
              rgb.green();
              this->gunShot = infra.getShot();
              if (this->gunShot >  0 || this->testButtonClicked()) {
                Serial.println("Target " + String(this->targetId) + " Hit by player " + String(this->gunShot));
                rgb.red();
                this->state = DROPPED;
                delay(100);
                arm.drop();
                activeTarget = 0;
              }
            }
            else {
              this->state = UP;
            }
            break;
        }
      }
    }
};
