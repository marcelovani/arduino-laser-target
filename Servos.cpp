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
      // @todo add offset configuration for each target
      this->start_position = targetStartPosition;
      this->drop_position = targetDropPosition;
      // Timer delay between 5 and 35 seconds. @todo implement this.
      this->timer_delay = 5 + rand() % targetStartDelay;
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
      // Timer delay between 5 and 35 seconds.
      this->timerOnStart(5 + rand() % targetDropDelay);
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
