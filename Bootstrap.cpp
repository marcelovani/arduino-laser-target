// Uncomment to test on emulators.
// #define ON_EMULATOR

// @todo use include for this file and in the port_test.ino file.
#if defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_NANO)
  const byte NUM_PORTS = 1;
  #define INFRA_ENABLED
#else
  const byte NUM_PORTS = 7;
  #define DISPLAY_ENABLED
#endif

const byte PINS = 8;

// Manual set number of targets. @todo use arrays and make this dynamic
byte targetCount = 0;

// Global variable to make sure only one target can receive shots at a time.
byte activeTarget = 0;

// Global variable to track disabled targets
boolean disabledTargets[NUM_PORTS+1];

// Global configuration for target positions.
const byte targetStartPosition = 160;
const byte targetDropPosition = 90;

#ifdef ON_EMULATOR
  // Emulator configuration.
  const unsigned long targetStartDelay = 2;
  const unsigned long targetDropDelay = 10;
#else
  #define INFRA_ENABLED
  #define MP3_PLAYER_ENABLED
  // Used to determine the delay to start and drop the target.
  unsigned long targetStartDelay = 10;
  unsigned long targetDropDelay = 20;
#endif

#include <Bounce2.h>
