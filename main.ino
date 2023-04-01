// Emulators
// https://wokwi.com/projects/353876404628899841
// https://www.tinkercad.com/things/30sClkF22Tl-copy-of-led-demo-with-classes/editel?tenant=circuits

#include "Bootstrap.cpp"
#include "Game.cpp"
#include "Runnable.cpp"
#include "MillisTimer.h"
#include "MillisTimer.cpp"
#include "Led.cpp"
#include "Infra.cpp"
#include "Laser.cpp"
#include "RgbLed.cpp"
#include "Arm.cpp"
#include "Display.cpp"
#include "Mp3Player.cpp"
#include "Target.cpp"
#include "TargetRandomizer.cpp"
#include "Setup.cpp"

void setup() {
  Serial.begin(115200);
  setupTargets();
  #ifdef MP3_PLAYER_ENABLED
    mp3.setup();
  #endif
  Runnable::setupAll();
}

void loop() {
  Runnable::loopAll();
}
