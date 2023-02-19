// See Board definitions https://github.com/Arduino-CI/arduino_ci/issues/89

#if defined(__ARDUINO_AVR_UNO__) || defined(ARDUINO_AVR_UNO)
    //@todo update this
    Servos servo1(9);
    Laser laser1(13);
    RgbLed rgb1(12, 10, 11);
    Infra receiver(15);//todo
    Target target1(1, 8, laser1, rgb1, servo1);

    Servos servo2(2);
    Laser laser2(6);
    RgbLed rgb2(5, 3, 4);
    Infra receiver(15);//todo
    Target target2(2, 7, laser2, rgb2, servo2);

#endif

#if defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_NANO)
    Servos servo1(12);
    Laser laser1(2);
    RgbLed rgb1(A4, A5, A6);
    Infra ir1(5);
    Target target1(laser1, rgb1, servo1, ir1);

    Servos servo2(11);
    Laser laser2(A7);
    RgbLed rgb2(A1, A2, A3);
    Infra ir2(6);
    Target target2(laser2, rgb2, servo2, ir2);
#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    // @todo use include for this file and in the port_test.ino file.
    const int PORTS = 7;
    const int PINS = 8;

    // Mapping for 7 Ports vs 8 Pins each port
    // Brown            - 1 (Ground)
    // White/Brown      - 2 (Servo)
    // Green            - 3 (RGB Green)
    // White/Blue       - 4 (IR)
    // Blue             - 5 (RGB Blue)
    // White/Green      - 6 (Laser)
    // Orange           - 7 (+5v)
    // White/Orange     - 8 (RGB Red)
    int pins[PORTS+1][PINS+1] = {
        //     1   2   3   4   5   6   7   8    <- Wires
        { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // Unused
        { 00, 00, 03, 16, 02, 17, 15, 00, 14 }, // 1 -> ok
        { 00, 00, 43, 45, 47, 49, 51, 00, 53 }, // 2 -> ok
        { 00, 00, 50, 39, 37, 35, 41, 00, 52 }, // 3 -> ok
        { 00, 00, 32, 42, 48, 46, 44, 00, 33 }, // 4 -> ok
        { 00, 00, 26, 34, 40, 38, 36, 00, 27 }, // 5 -> ok
        { 00, 00, 25, 28, 31, 30, 29, 00, 24 }, // 6 -> ok
        { 00, 00, 23, 20, 19, 18, 22, 00, 21 }  // 7 -> ok
    };

    // @todo make this dynamically
    #ifdef ON_EMULATOR
        Servos servo1(13);
        Laser laser1(12);
        RgbLed rgb1(10,9,8);
        Infra ir1(7);
        Target target1(laser1, rgb1, servo1, ir1);

        Servos servo4(26);
        Laser laser4(22);
        RgbLed rgb4(23, 25, 24);
        Infra ir4(27);
        Target target4(laser4, rgb4, servo4, ir4);

        // Servos servo5(37);
        // Laser laser5(28);
        // RgbLed rgb5(29, 32, 33);
        // Infra ir5(36);
        // Target target5(laser5, rgb5, servo5, ir5);

        // Servos servo6(51);
        // Laser laser6(46);
        // RgbLed rgb6(47, 48, 49);
        // Infra ir6(50);
        // Target target6(laser6, rgb6, servo6, ir6);

        Servos servo9(3);
        Laser laser9(4);
        RgbLed rgb9(53, 52, 2);
        Infra ir9(5);
        Target target9(laser9, rgb9, servo9, ir9);
    #else
        Servos servo1(pins[1][2]);
        Laser laser1(pins[1][6]);
        RgbLed rgb1(pins[1][8], pins[1][3], pins[1][5]);
        Infra ir1(pins[1][4]);
        Target target1(laser1, rgb1, servo1, ir1);

        // Servos servo2(pins[2][2]);
        // Laser laser2(pins[2][6]);
        // RgbLed rgb2(pins[2][8], pins[2][3], pins[2][5]);
        // Infra ir2(pins[2][4]);
        // Target target2(laser2, rgb2, servo2, ir2);

        // Servos servo3(pins[3][2]);
        // Laser laser3(pins[3][6]);
        // RgbLed rgb3(pins[3][8], pins[3][3], pins[3][5]);
        // Infra ir3(pins[3][4]);
        // Target target3(laser3, rgb3, servo3, ir3);

        Servos servo4(pins[4][2]);
        Laser laser4(pins[4][6]);
        RgbLed rgb4(pins[4][8], pins[4][3], pins[4][5]);
        Infra ir4(pins[4][4]);
        Target target4(laser4, rgb4, servo4, ir4);
    #endif
#endif

void setup() {
  Serial.begin(9600);
  Runnable::setupAll();
}
