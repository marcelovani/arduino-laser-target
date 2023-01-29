// Comment out to test on emulators with button instead of infra.
#define INFRA_ENABLED

// Manual set number of targets
byte targetCount = 2;
// Global variable to make sure only one target can receive shots at a time.
byte activeTarget = random(targetCount + 1);
