#ifdef MP3_PLAYER_ENABLED
    #include "SoftwareSerial.h"
    #include "DFRobotDFPlayerMini.h"
#endif

// MP3 player
// Docs https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
class Mp3Player {
  #ifdef MP3_PLAYER_ENABLED
    DFRobotDFPlayerMini player;
    SoftwareSerial serial;
  #endif

  public:
    #ifdef MP3_PLAYER_ENABLED
      Mp3Player(byte rx, byte tx) :
      serial(rx, tx) {}
    #else
      Mp3Player(byte rx, byte tx) {}
    #endif

    void setup() {
      #ifdef MP3_PLAYER_ENABLED
        this->serial.begin(9600);
        this->player.begin(this->serial);
        this->player.setTimeOut(100);
        this->player.outputDevice(DFPLAYER_DEVICE_SD);
        this->player.disableLoop();
      #endif
    }

    void play(byte track, byte vol) {
      #ifdef MP3_PLAYER_ENABLED
        this->player.volume(vol);
        this->player.play(track);
      #endif
    }
};
