#include <Servo.h>

class Servos: public Runnable {
  byte pin;
  Servo servo;

  private:
    MillisTimer _timerOn;
    byte position;
    byte start_position;
    byte drop_position;
    unsigned long timer_delay;

  public:
    Servos(byte pin) : pin(pin) {
    }

    void setup() {
      servo.attach(pin);
      // @todo make this configurable.
      this->start_position = 160;
      this->drop_position = 90;
      // Timer delay between 10 and 30 seconds.
      this->timer_delay = 10 + rand() % 20;
      // Debugging
      this->timer_delay = 5;
      this->reset();
    }

    void loop() {
      this->timerLoop();
    }

    void reset() {
      this->position = this->start_position;
      this->send();
    }

    void drop() {
      this->position = this->drop_position;
      this->send();
      // temp this->timerOnStart(10 + rand() % 20);
      this->timerOnStart(5 + rand() % 5);
    }

    void send() {
      servo.write(this->position);
    }

    /**
     * Used to check if the target is up or dropped.
     */
    boolean isOn() {
      return this->position == this->start_position;
    }

    /**
     * Start a timer to reset the servo position.
     */
    void timerOnStart(unsigned long _interval)
    {
      this->_timerOn.setInterval(_interval * 1000);
      this->_timerOn.setRepeats(1);
      this->_timerOn.start();
    }

    /**
     * Timer loop controller.
     */
    void timerLoop() {
      this->_timerOn.run();
      if (!this->_timerOn.isRunning()) {
        this->reset();
      }
    }
};
