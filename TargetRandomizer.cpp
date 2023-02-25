Target* targets[NUM_PORTS];

class TargetRandomizer: public Runnable {
  private:
    unsigned char gunShot;
    MillisTimer _timerOn;
    const byte start_delay = 10;

    /**
     * Logic to select the next available target.
     */
    void pickRandomTarget() {
      byte nextTarget;

      // Randomize.
      randomSeed(analogRead(0));
      nextTarget = random(targetCount) + 1;

      // Check if target is disabled.
      if (this->isTargetDisabled(nextTarget)) {
        this->pickRandomTarget();
      }

      // Try not to select same target.
      if (nextTarget == activeTarget && targetCount > nextTarget) {
        // Serial.print(" EQUAL ");
        nextTarget++;
      }

      // If value is invalid, disable and pick another one.
      if (nextTarget > targetCount) {
        this->pickRandomTarget();
      }

      // Check if target is up.
      if (this->isTargetUp(nextTarget)) {
        activeTarget = nextTarget;
      }

      if (activeTarget > 0) {
        Serial.println("Active target " + String(activeTarget));
        // Timer delay to select a new target in N seconds.
        this->timerOnStart(this->start_delay);
      }
    }

    void disableTarget(byte targetId) {
      targets[targetId]->disable();
    }

    boolean isTargetDisabled(byte targetId) {
      return targets[targetId]->isDisabled();
    }

    boolean isTargetUp(byte targetId) {
      return targets[targetId]->isUp() || targets[targetId]->isReady();
    }

    /**
     * Timer to change target.
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
      // Change target when the active target did not receive any shot.
      if (!this->_timerOn.isRunning()) {
        this->pickRandomTarget();
      }
    }

  public:
    TargetRandomizer() {}

    void setup() {
      // Change the value of the global variable.
      activeTarget = 0;
    }

    void loop() {
      this->timerLoop();

      if (activeTarget == 0) {
        this->pickRandomTarget();
      }
    }
};
