#if defined(__ARDUINO_AVR_UNO__) || defined(ARDUINO_AVR_UNO)
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

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    Servos servo1(13);
    Laser laser1(12);
    RgbLed rgb1(10, 9, 8);
    Infra ir1(7);
    Target target1(1, 11, laser1, rgb1, servo1, ir1);

    Servos servo2(30);
    Laser laser2(31);
    RgbLed rgb2(34, 35, 38);
    Infra ir2(39);
    Target target2(2, 5, laser2, rgb2, servo2, ir2);

    // Servos servo3(22);
    // Laser laser3(23);
    // RgbLed rgb3(24, 25, 26);
    // Infra ir3(27);
    // Target target3(3, 28, laser3, rgb3, servo3, ir3);
#endif

void setup() {
  Serial.begin(9600);
  Runnable::setupAll();
}
