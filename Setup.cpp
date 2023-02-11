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

    Servos servo1(13);
    Laser laser1(12);
    RgbLed rgb1(10, 9, 8);
    Infra ir1(7);
    Target target1(laser1, rgb1, servo1, ir1);

    Servos servo2(30);
    Laser laser2(31);
    RgbLed rgb2(34, 35, 38);
    Infra ir2(39);
    Target target2(laser2, rgb2, servo2, ir2);

    Servos servo3(40);
    Laser laser3(41);
    RgbLed rgb3(42, 43, 44);
    Infra ir3(45);
    Target target3(laser3, rgb3, servo3, ir3);

    #ifdef ON_EMULATOR
        Servos servo4(26);
        Laser laser4(22);
        RgbLed rgb4(23, 25, 24);
        Infra ir4(27);
        Target target4(laser4, rgb4, servo4, ir4);

        Servos servo5(37);
        Laser laser5(28);
        RgbLed rgb5(29, 32, 33);
        Infra ir5(36);
        Target target5(laser5, rgb5, servo5, ir5);

        Servos servo6(51);
        Laser laser6(46);
        RgbLed rgb6(47, 48, 49);
        Infra ir6(50);
        Target target6(laser6, rgb6, servo6, ir6);
    #endif
#endif

void setup() {
  Serial.begin(9600);
  Runnable::setupAll();
}
