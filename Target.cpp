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
    byte gunShot;

    void pickRandomTarget() {
      Serial.println("Active target " + String(activeTarget));
      activeTarget = random(targetCount) + 1;
      if (activeTarget > targetCount || (activeTarget == this->targetId && !this->state != READY)) {
        this->pickRandomTarget();
      }
      Serial.println("Active target " + String(activeTarget));
    }

  public:
    Target() {}

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
      laser.off();
      rgb.yellow();
      rgb.blink();

      // When infra is not enabled, we use the test button to shoot.
      #ifndef INFRA_ENABLED
        this->testButtonPin = this->infra.getPin();
        pinMode(this->testButtonPin, INPUT_PULLUP);
      #endif
      this->state = UP;
    }

    void loop() {
      if (activeTarget == 0) {
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
          laser.blink();
          this->state = READY;
          break;

        case READY:
          if (activeTarget == this->targetId) {
            rgb.green();
            gunShot = infra.getShot();
            if (gunShot || digitalRead(testButtonPin) == LOW) {
              Serial.print("Gun " + String(gunShot) + " shot target " + String(this->targetId) + " - ");
              Serial.println("Hit");
              rgb.red();
              this->state = DROPPED;
              servo.drop();
              activeTarget = 0;
            }
          }
          break;
      }
    }
};
