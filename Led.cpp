class Led: public Runnable {
  byte pin;

  unsigned long _timerInterval;
  unsigned long _timerDuration;
  short int _timerRepeat;

  private:
    MillisTimer _timerOn;
    MillisTimer _timerOff;
    byte status;

  public:
    Led(byte pin) : pin(pin) {
    }

    void setup() {
      status = LOW;
      pinMode(pin, OUTPUT);
      send();
    }

    void loop() {
      this->timerLoop();
    }

    void send() {
      digitalWrite(pin, status);
    }

    void powerToggle() {
      if (status) {
        status = LOW;
      }
      else {
        status = HIGH;
      }
      send();
    }

    byte getPin() {
      return pin;
    }

    void on() {
      if (!status) {
        status = HIGH;
        send();
      }
    }

    void off() {
      if (status) {
        status = LOW;
        send();
      }
    }

    byte isOn() {
      return status == HIGH;
    }

    byte isOff() {
      return status == LOW;
    }

    /**
     * Blink needs 3 arguments
     *  - Interval: How how long between blinks (when Repeat is > 1)
     *  - Duration: For how long the Led stays on when it blinks
     *  - Repeat: How many times it should blink
     */
    void blink(unsigned long interval, unsigned long duration, byte repeat)
    {
      if (this->_timerOn.isRunning() || this->_timerOff.isRunning()) {
        return;
      }

      _timerRepeat = repeat;
      _timerInterval = interval;
      _timerDuration = duration;
      this->status = LOW;
      this->timerOnStart();
    }

    /**
     * Start a timer to turn ON the Led.
     */
    void timerOnStart()
    {
      this->_timerOn.setInterval(_timerDuration);
      this->_timerOn.setRepeats(1);
      this->_timerOn.start();
    }

    /**
     * Start a timer to turn OFF the Led.
     */
    void timerOffStart()
    {
      this->_timerOff.setInterval(_timerInterval);
      this->_timerOff.setRepeats(1);
      this->_timerOff.start();
    }

    /**
     * Timer loop controller.
     * Flip-flop mechanism to toggle the Led status.
     */
    void timerLoop() {
      // When there is no more repeats, turn everything off
      if (_timerRepeat == 0) {
        this->_timerOn.stop();
        this->_timerOff.stop();
        this->off();
      }

      // Flip-flop for Led OFF.
      if (this->status == LOW && _timerRepeat > 0) {
        this->_timerOn.run();
        if (this->_timerOn.isRunning())
        {
          this->send();
        }

        if (!this->_timerOn.isRunning())
        {
          this->on();
          _timerRepeat--;
          this->timerOffStart();
        }
      }

      // Flip flop for Led ON.
      if (this->status == HIGH && _timerRepeat > 0) {
        this->_timerOff.run();
        if (this->_timerOff.isRunning())
        {
          this->send();
        }

        if (!this->_timerOff.isRunning())
        {
          this->off();
          this->timerOnStart();
        }
      }
    }
};
