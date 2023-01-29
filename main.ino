// Emulators
// https://wokwi.com/projects/353876404628899841
// https://www.tinkercad.com/things/30sClkF22Tl-copy-of-led-demo-with-classes/editel?tenant=circuits

#include "Bootstrap.cpp"
#include "Runnable.cpp"
#include "MillisTimer.h"
#include "MillisTimer.cpp"
#include "Led.cpp"
#include "Infra.cpp"
#include "Laser.cpp"
#include "RgbLed.cpp"
// #include "Button.cpp"
// #include "LedControlButton.cpp"
#include "Servos.cpp"
#include "Target.cpp"
#include "Setup.cpp"
//#include "TargetRandomizer.cpp"

// Create instance of the target randomizer.
//TargetRandomizer randomizer(2);

void loop() {
  Runnable::loopAll();
}
