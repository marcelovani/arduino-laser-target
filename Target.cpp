class Target: public Runnable {
    Laser &laser;
    RgbLed &rgb;
    Servos &servo;
    Infra &infra;
    byte testButtonPin;
    byte targetId;

    enum State {
      DROPPED = 0,
      UP = 1,
      READY = 2
    } state;

  private:
    unsigned char gunShot;

    void pickRandomTarget() {
      byte currentTarget;
      currentTarget = activeTarget;
      // Randomize.
      randomSeed(analogRead(0));
      activeTarget = random(targetCount) + 1;
      // Validate.
      if (activeTarget == currentTarget) {
        this->pickRandomTarget();
      }
      if (activeTarget > targetCount) {
        this->pickRandomTarget();
      }
      if (activeTarget == this->targetId && !this->state != READY) {
        this->pickRandomTarget();
      }
      Serial.println("Active target " + String(activeTarget));
    }

  public:
    Target(
        Laser &laserInstance,
        RgbLed &rgbInstance,
        Servos &servoInstance,
        Infra &infraInstance
      ) :
        servo(servoInstance),
        laser(laserInstance),
        rgb(rgbInstance),
        infra(infraInstance)
      {
        // Increase global counter;
        targetCount++;
        this->targetId = targetCount;
    }

    boolean isReady() {
      return this->state == READY;
    }

    void setup() {
      this->state = DROPPED;

      laser.off();
      rgb.yellow();
      // rgb.blink();

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

    void loop() {
      if (activeTarget == 0) {
        Serial.println("Active target " + String(activeTarget));
        this->pickRandomTarget();
      }

      switch (this->state) {
        case DROPPED:
          rgb.red();
          if (servo.isOn()) {
            this->state = UP;
          }
          break;

        case UP:
          if (activeTarget == this->targetId) {
            laser.blink();
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
              servo.drop();
              this->pickRandomTarget();
            }
          }
          break;
      }
    }
};
