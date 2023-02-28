class RgbLed: public Runnable {
  Led rLed, gLed, bLed;

  private:
    unsigned char priority() {
      return 2;
    }

  public:
    RgbLed(byte rPin, byte gPin, byte bPin) :
      rLed(rPin),
      gLed(gPin),
      bLed(bPin) {
    }

    void setup() {
    }

    void loop() {
    }

    void red() {
        rLed.on();
        gLed.off();
        bLed.off();
    }

    void green() {
        rLed.off();
        gLed.on();
        bLed.off();
    }
    
    void blue() {
        rLed.off();
        gLed.off();
        bLed.on();
    }

    void yellow() {
        rLed.on();
        gLed.on();
        bLed.off();
    }

    void purple() {
        rLed.on();
        gLed.off();
        bLed.on();
    }

    void cyan() {
        rLed.off();
        gLed.on();
        bLed.on();
    }

    void white() {
        rLed.on();
        gLed.on();
        bLed.on();
    }


    void off() {
        gLed.off();
    }

    void blink() {
        if (rLed.isOn()) {
            rLed.blink(100, 400, 3);
        }
        if (gLed.isOn()) {
            gLed.blink(100, 400, 3);
        }
        if (bLed.isOn()) {
            bLed.blink(100, 400, 3);
        }
    }
};
