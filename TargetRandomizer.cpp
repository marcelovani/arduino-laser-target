Target* targets[NUM_PORTS];

class TargetRandomizer: public Runnable {
  private:
    unsigned char gunShot;
    MillisTimer _timerOn;
    const byte start_delay = 5;

    unsigned char priority() {
      return 2;
    }

    /**
     * Logic to select the next available target.
     */
    void pickRandomTarget() {
      // Randomize.
      randomSeed(analogRead(0));
      long max = (long) targetCount + 1;
      byte nextTarget = random(max);

      // Check if target is disabled.
      if (this->isTargetDisabled(nextTarget)) {
        return;
      }

      // Try not to select same target.
      if (nextTarget == activeTarget && activeTarget < targetCount) {
        nextTarget++;
      }

      // Check for invalid target.
      if (nextTarget > targetCount) {
        return;
      }

      // Check if target is up.
      if (this->isTargetUp(nextTarget)) {
        activeTarget = nextTarget;
      }

      if (activeTarget > 0 && !this->_timerOn.isRunning()) {
        // Timer delay to select a new target when there is no shoots.
        randomSeed(analogRead(0));
        // @todo reduce delay based on game level.
        byte seconds = random(5) + this->start_delay;
        display.displayStatus(String("Idle delay " + String(seconds) + "s"));
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

    /**
     * Reset game.
     */
    void reset() {
      // Reset scores.
      scores[1] = 0;
      scores[2] = 0;
      scores[3] = 0;

      // Reset targets.
      unsigned char p;
      for (p = 1; p < targetCount+1; p++) {
        disabledTargets[p] = false;
        targets[p]->enable();
      }

      // Game start.
      GameState = PLAYING;
    }

  public:
    TargetRandomizer() {}

    void setup() {
      // Change the value of the global variable.
      activeTarget = 0;
    }

    void loop() {
      // Reset game
      if (GameState == RESET) {
        this->reset();
      }

      if (GameState != PLAYING) {
        return;
      }

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
