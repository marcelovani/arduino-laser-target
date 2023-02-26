Target* targets[NUM_PORTS];

class TargetRandomizer: public Runnable {
  private:
    unsigned char gunShot;
    MillisTimer _timerOn;
    const byte start_delay = 5;

    /**
     * Logic to select the next available target.
     */
    void pickRandomTarget() {
      byte nextTarget;
      byte max = NUM_PORTS - 1;

      // Randomize.
      randomSeed(analogRead(0));
      nextTarget = random(NUM_PORTS);

      // Check if target is disabled.
      if (this->isTargetDisabled(nextTarget)) {
        this->pickRandomTarget();
      }

      // Try not to select same target.
      if (nextTarget == activeTarget && max > nextTarget) {
        nextTarget++;
      }

      // If value is invalid, disable and pick another one.
      if (nextTarget > max) {
        this->pickRandomTarget();
      }

      // Check if target is up.
      if (this->isTargetUp(nextTarget)) {
        activeTarget = nextTarget;
      }

      if (activeTarget > 0) {
        Serial.println("Active target " + String(activeTarget));
        this->_timerOn.stop();
        // Timer delay to select a new target when there is no shoots.
        randomSeed(analogRead(0));
        // @todo reduce delay based on game level.
        byte seconds = random(5) + this->start_delay;
        Serial.println("Idle delay " + String(seconds) + "s");
        this->timerOnStart(seconds);
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
        activeTarget = 0;
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
      else if (activeTarget == 254) {
        this->_timerOn.stop();
        // Special code until timer is up and a new target is selected.
        activeTarget = 255;
        // Random timer for next target selection.
        randomSeed(analogRead(0));
        // @todo reduce delay based on game level.
        byte seconds = random(5);
        Serial.println("Random start in " + String(seconds) + "s");
        this->timerOnStart(seconds);
      }
    }
};
