// Emulators
// https://wokwi.com/projects/353876404628899841
// https://www.tinkercad.com/things/30sClkF22Tl-copy-of-led-demo-with-classes/editel?tenant=circuits

#include "Bootstrap.cpp"
#include "Runnable.cpp"
#include "MillisTimer.h"
#include "MillisTimer.cpp"
#include "Led.cpp"
#include "Display.cpp"
#include "Infra.cpp"
#include "Laser.cpp"
#include "RgbLed.cpp"
// #include "Button.cpp"
// #include "LedControlButton.cpp"
#include "Arm.cpp"
#include "Target.cpp"
#include "TargetRandomizer.cpp"
#include "Setup.cpp"

void loop() {
  Runnable::loopAll();
}
