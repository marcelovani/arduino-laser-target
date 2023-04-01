// See Board definitions https://github.com/Arduino-CI/arduino_ci/issues/89

#if defined(__ARDUINO_AVR_UNO__) || defined(ARDUINO_AVR_UNO)
    // Mapping for 7 Ports vs 8 Pins each port
    // Brown            - 1 (Ground)
    // White/Brown      - 2 (Servo)
    // Green            - 3 (RGB Green)
    // White/Blue       - 4 (IR)
    // Blue             - 5 (RGB Blue)
    // White/Green      - 6 (Laser)
    // Orange           - 7 (+5v)
    // White/Orange     - 8 (RGB Red)
    int pins[NUM_PORTS+1][PINS+1] = {
        //     1   2   3   4   5   6   7   8    <- Wires
        { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // Unused
        { 00, 00,  8, 11,  9, 10, 13, 00, 12 }, // 1 -> ok
        { 00, 00, 30, 35, 39, 38, 31, 00, 34 }, // 2 -> ok
        { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 3 -> ok
        { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 4 -> ok
        { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 5 -> ok
        { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 6 -> ok
        { 00, 00, 00, 00, 00, 00, 00, 00, 00 }  // 7 -> ok
        };
#endif

#if defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_NANO)
  // Mapping for 7 Ports vs 8 Pins each port
  // Brown            - 1 (Ground)
  // White/Brown      - 2 (Servo)
  // Green            - 3 (RGB Green)
  // White/Blue       - 4 (IR)
  // Blue             - 5 (RGB Blue)
  // White/Green      - 6 (Laser)
  // Orange           - 7 (+5v)
  // White/Orange     - 8 (RGB Red)
  int pins[NUM_PORTS+1][PINS+1] = {
    //     1   2   3   4   5   6   7   8    <- Wires
    { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // Unused
    { 00, 00, 12,  5,  2,  7,  4, 00,  8 }, // 1 -> ok
  };
#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  // Mapping for 7 Ports vs 8 Pins each port
  // Brown            - 1 (Ground)
  // White/Brown      - 2 (Servo)
  // Green            - 3 (RGB Green)
  // White/Blue       - 4 (IR)
  // Blue             - 5 (RGB Blue)
  // White/Green      - 6 (Laser)
  // Orange           - 7 (+5v)
  // White/Orange     - 8 (RGB Red)
  int pins[NUM_PORTS+1][PINS+1] = {
    //     1   2   3   4   5   6   7   8    <- Wires
    { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // Unused
    { 00, 00,  3, 16,  2, 17, 15, 00, 14 }, // 1 -> ok
    { 00, 00, 43, 45, 47, 49, 51, 00, 53 }, // 2 -> ok
    { 00, 00, 50, 39, 37, 35, 41, 00, 52 }, // 3 -> ok
    { 00, 00, 32, 42, 48, 46, 44, 00, 33 }, // 4 -> ok
    { 00, 00, 26, 34, 40, 38, 36, 00, 27 }, // 5 -> ok
    { 00, 00, 25, 28, 31, 30, 29, 00, 24 }, // 6 -> ok
    { 00, 00, 23,  5, 19, 18, 22, 00,  4 }  // 7 -> ok
  };
#endif

// Create all device instances.
// @todo use array and loop
// i.e. Arm * arms = new Arm[NUM_PORTS];

Arm     arm1(pins[1][2]);
Infra    ir1(pins[1][4]);
Laser laser1(pins[1][6]);
RgbLed  rgb1(pins[1][8], pins[1][3], pins[1][5]);

#if ! defined(ARDUINO_AVR_NANO) && ! defined(ARDUINO_AVR_NANO)
Arm     arm2(pins[2][2]);
Infra    ir2(pins[2][4]);
Laser laser2(pins[2][6]);
RgbLed  rgb2(pins[2][8], pins[2][3], pins[2][5]);

Arm     arm3(pins[3][2]);
Infra    ir3(pins[3][4]);
Laser laser3(pins[3][6]);
RgbLed  rgb3(pins[3][8], pins[3][3], pins[3][5]);

Arm     arm4(pins[4][2]);
Infra    ir4(pins[4][4]);
Laser laser4(pins[4][6]);
RgbLed  rgb4(pins[4][8], pins[4][3], pins[4][5]);

Arm     arm5(pins[5][2]);
Infra    ir5(pins[5][4]);
Laser laser5(pins[5][6]);
RgbLed  rgb5(pins[5][8], pins[5][3], pins[5][5]);

Arm     arm6(pins[6][2]);
Infra    ir6(pins[6][4]);
Laser laser6(pins[6][6]);
RgbLed  rgb6(pins[6][8], pins[6][3], pins[6][5]);

Arm     arm7(pins[7][2]);
Infra    ir7(pins[7][4]);
Laser laser7(pins[7][6]);
RgbLed  rgb7(pins[7][8], pins[7][3], pins[7][5]);
#endif

// Create instance of the target randomizer.
TargetRandomizer randomizer;

void setupTargets() {
  // Create instances of targets.
  targets[1] = new Target(laser1, rgb1, arm1, ir1, 1, 1);
  #if ! defined(ARDUINO_AVR_NANO) && ! defined(ARDUINO_AVR_NANO)
    targets[2] = new Target(laser2, rgb2, arm2, ir2, 2, 1);
    targets[3] = new Target(laser3, rgb3, arm3, ir3, 3, 1);
    targets[4] = new Target(laser4, rgb4, arm4, ir4, 2, 2);
    targets[5] = new Target(laser5, rgb5, arm5, ir5, 1, 3);
    targets[6] = new Target(laser6, rgb6, arm6, ir6, 3, 3);
    targets[7] = new Target(laser7, rgb7, arm7, ir7, 3, 3);
  #endif
}
