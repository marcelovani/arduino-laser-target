Target* targets[NUM_PORTS];

class TargetRandomizer: public Runnable {
  private:
    unsigned char gunShot;
    MillisTimer _timerOn;
    // How long to wait before changing targets.
    const byte idle_delay = 2;

    unsigned char priority() {
      return 2;
    }

    /**
     * Calculate delay based on game scores.
     */
    int getDelay() {
      // Start with 5 seconds delay.
      int delay = 5;

      int game_level = game.getLevel();
      display.displayStatus(String("Game Level " + String(game_level)));

      // Reduce delay based on level.
      delay = delay - (game_level - 1);

      // Avoid negative numbers.
      if (delay < 0) {
        delay = 0;
      }

      return delay;
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
        // Timed delay to select a new target when there are no shoots.
        unsigned long seconds = this->getDelay() + this->idle_delay;
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
      game.reset();

      // Reset targets.
      unsigned char p;
      for (p = 1; p < targetCount+1; p++) {
        disabledTargets[p] = false;
        targets[p]->enable();
      }
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
        // Timed delay to select next target.
        unsigned long seconds = this->getDelay();
        this->timerOnStart(seconds);
      }
    }
};
