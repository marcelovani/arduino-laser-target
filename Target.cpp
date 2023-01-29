class Target: public Runnable {
    const byte brakeSensePin;
    Laser &laser;
    RgbLed &rgb;
    Servos &servo;
    Infra &infra;
    byte targetId;
    byte ready;

  private:
    byte gunShot;

    void randomTarget() {
      Serial.println("Active target " + String(activeTarget));
      activeTarget = random(targetCount) + 1;
      if (activeTarget > targetCount || (activeTarget == this->targetId && !this->ready)) {
        this->randomTarget();
      }
      Serial.println("Active target " + String(activeTarget));
    }

  public:
    Target() {}

    Target(
        byte id,
        byte attachToBrakeSense,
        Laser &laserInstance,
        RgbLed &rgbInstance,
        Servos &servoInstance,
        Infra &infraInstance
      ) :
        targetId(id),
        servo(servoInstance),
        brakeSensePin(attachToBrakeSense),
        laser(laserInstance),
        rgb(rgbInstance),
        infra(infraInstance) {
    }

    byte getPin() {
      return brakeSensePin;
    }

    boolean isReady() {
      return this->ready;
    }

    void setup() {
      pinMode(brakeSensePin, INPUT_PULLUP);
      laser.off();
      rgb.yellow();
      rgb.blink();
      this->ready = 1;
    }

    void loop() {
      // Update ready status. @todo we dont need to associate this, we can get status directy in isReady()
      this->ready = servo.isOn();

      // Check for shots.
      if (activeTarget == this->targetId) {
        if (this->ready) {
          rgb.green();
          gunShot = infra.getShot();
          if (gunShot || digitalRead(brakeSensePin) == LOW) {
            Serial.print("Gun " + String(gunShot) + " shot target " + String(this->targetId) + " - ");
            Serial.println("Hit");
            laser.blink();
            rgb.red();
            this->ready = 0;
            servo.drop();
            // Force TargetRandomizer to pick another target.
            //activeTarget = 0;
            this->randomTarget();
          }
        }
        else {
          rgb.red();
        }
      }
    }
};
