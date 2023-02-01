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
  const unsigned long targetStartDelay = 10;
  const unsigned long targetDropDelay = 10;
#else
  #define INFRA_ENABLED
  #define MP3_PLAYER_ENABLED
  // Used to determine the delay to start and drop the target.
  unsigned long targetStartDelay = 20;
  unsigned long targetDropDelay = 20;
#endif
