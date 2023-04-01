#ifdef MP3_PLAYER_ENABLED
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// MP3 player
// https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
// https://circuitjournal.com/how-to-use-the-dfplayer-mini-mp3-module-with-an-arduino
class Mp3Player {
  SoftwareSerial serial;
  DFRobotDFPlayerMini player;

  public:
    Mp3Player(uint8_t rx, uint8_t tx) : serial(rx, tx) {}

    void setup() {
      this->serial.begin(9600);
      if (this->player.begin(this->serial)) {
        Serial.println("Connected to DFPlayer sucessfully!");
        this->player.setTimeOut(100);
        this->player.volume(1);
      } else {
        Serial.println("Connecting to DFPlayer Mini failed!");
      }
    }

    void play(int track, uint8_t vol) {
      Serial.println("Playing " + String(track));
      this->player.volume(vol);
      this->player.playMp3Folder(track);
      // this->player.volume(1);
    }
};

// @todo this should be done in Setup.cpp
Mp3Player mp3(9, 10); // RX, TX

#endif
