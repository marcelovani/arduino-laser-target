class TargetRandomizer: public Runnable {
  byte targetCount;

  public:
    TargetRandomizer(byte number) : targetCount(number - 1) {
    }

    void setup() {
      // Change the value of the global variable.
      activeTarget = 0;
    }

    void loop() {
      // Find next available target.
      if (activeTarget == 0) {
        activeTarget = random(this->targetCount + 1);
        // Check if the target is ready.
        // @todo loop arrays of targets.
        switch(activeTarget) {
          case 1:
            if (!target1.isReady()) {
              activeTarget = 0;
            }
            break;

          case 2:
            if (!target2.isReady()) {
              activeTarget = 0;
            }
            break;

        }
        Serial.println("Target 1 ready " + String(target1.isReady()));
        Serial.println("Target 2 ready " + String(target2.isReady()));
        Serial.println("Active target " + String(activeTarget));
      }
    }
};
