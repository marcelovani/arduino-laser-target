// Uncomment to test on emulators.
// #define ON_EMULATOR

// Manual set number of targets. @todo use arrays and make this dynamic
byte targetCount = 0;

// Global variable to make sure only one target can receive shots at a time.
byte activeTarget = 0;

// Global configuration for target positions.
const byte targetStartPosition = 160;
const byte targetDropPosition = 90;

#ifdef ON_EMULATOR
  // Emulator configuration.
  const unsigned long targetStartDelay = 1;
  const unsigned long targetDropDelay = 1;
#else
  #define INFRA_ENABLED
  // Used to determine the delay to start and drop the target.
  unsigned long targetStartDelay = 30
  unsigned long targetDropDelay = 30;
#endif
