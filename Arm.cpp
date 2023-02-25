#include <Servo.h>

class Arm: public Runnable {
  byte pin;
  Servo arm;

  private:
    MillisTimer _timerOn;
    byte position;
    byte start_position;
    byte drop_position;
    unsigned long timer_delay;

    /**
     * Send command to arm.
     */
    void send() {
      arm.write(this->position);
    }

    /**
     * Timer to reset the arm position.
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
      // Check if its time to reset.
      if (!this->_timerOn.isRunning()) {
        this->reset();
      }
    }

  public:
    Arm() {}

    Arm(byte pin) : pin(pin) {
    }

    void setup() {
      if (pin == 0) {
        return;
      }

      arm.attach(pin);
      // @todo add offset configuration for each target
      this->start_position = targetStartPosition;
      this->drop_position = targetDropPosition;
      // Start dropped.
      this->position = this->drop_position;
      this->send();
      // Timer delay to start between 5 and 15 seconds.
      this->timerOnStart(5 + rand() % targetStartDelay);
    }

    void loop() {
      this->timerLoop();
    }

    /**
     * Change the arm to start position. 
     */
    void reset() {
      this->position = this->start_position;
      this->send();
    }

    /**
     * Change the arm to drop position. 
     */
    void drop() {
      this->position = this->drop_position;
      this->send();
      // Timer delay between 5 and 35 seconds.
      this->timerOnStart(5 + rand() % targetDropDelay);
    }

    /**
     * Used to check if the target is up or dropped.
     */
    boolean isOn() {
      return this->position == this->start_position;
    }
};
