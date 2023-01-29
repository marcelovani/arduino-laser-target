class Target: public Runnable {
    Laser &laser;
    RgbLed &rgb;
    Servos &servo;
    Infra &infra;
    byte testButtonPin;
    byte targetId;
    byte ready;

  private:
    byte gunShot;

    void pickRandomTarget() {
      Serial.println("Active target " + String(activeTarget));
      activeTarget = random(targetCount) + 1;
      if (activeTarget > targetCount || (activeTarget == this->targetId && !this->ready)) {
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
      return this->ready;
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
      this->ready = 1;
    }

    void loop() {
      // Update ready status. @todo we dont need to associate this, we can get status directy in isReady()
      this->ready = servo.isOn();

      if (activeTarget == 0) {
        this->pickRandomTarget();
      }

      // Check for shots.
      if (activeTarget == this->targetId) {
        if (this->ready) {
          rgb.green();
          gunShot = infra.getShot();
          if (gunShot || digitalRead(testButtonPin) == LOW) {
            Serial.print("Gun " + String(gunShot) + " shot target " + String(this->targetId) + " - ");
            Serial.println("Hit");
            laser.blink();
            rgb.red();
            this->ready = 0;
            servo.drop();
            activeTarget = 0;
          }
        }
        else {
          rgb.red();
        }
      }
    }
};
