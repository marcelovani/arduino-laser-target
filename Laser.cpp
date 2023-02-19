class Laser: public Runnable {
    Led led;

  public:
    Laser(byte pin) :
      led(pin) {
    }

    void setup() {
    }

    void loop() {
    }

    void on() {
        led.on();
    }

    void off() {
        led.off();
    }

    void blink() {
        led.blink(100, 200, 2);
    }
};
