#ifdef INFRA_ENABLED
  // Library http://www.harctoolbox.org/downloads/index.html
  #include <IrReceiverPoll.h>
#endif

// Infra Red Receiver.
class Infra: public Runnable {
    byte pin;
    // Duration readings.
    int d1, d2;
    // Threshold. @todo make configurable
    int t = 1100;
    // Which gun shot the target.
    byte player;

    #ifdef INFRA_ENABLED
      IrReceiverPoll receiver;
      static constexpr size_t BUFFERSIZE = 200U;
    #endif

  private:
    // Count bad readings.
    byte bad_readings;
    const byte bad_readings_limit = 5;

    unsigned char translateIR()
    {
      this->player = 0;

      #ifdef INFRA_ENABLED

        // Ignore noise and bad readings.
        if (receiver.getDataLength() < 17) {
          Serial.println("IR " + String(this->pin) + " Error: too short: " + String(receiver.getDataLength()));
          return 0;
        }

        // Use data length to get the duration of the 4th position from the end.
        d1 = receiver.getDuration(receiver.getDataLength() - 4);
        // Use data length to get the duration of the 2th position from the end.
        d2 = receiver.getDuration(receiver.getDataLength() - 2);

        // Ignore noise and bad readings.
        if (d1 > t * 3 || d2 > t * 3) {
          Serial.println("IR " + String(this->pin) + " ERROR: Bad code: " + String(d1) + " " + String(d2));
          this->bad_readings++;
          return 0;
        }

        Serial.println("Data points " + String(d1) + ", " + String(d2));

        if (d1 > t) {
          if (d2 < t) {
            this->player = 1;
            Serial.println("***********");
          } else {
            Serial.println("***********     ***********     ***********");
            this->player = 3;
          }
        }
        else if (d2 > t) {
          Serial.println("***********     ***********");
          this->player = 2;
        }
      #endif

      return this->player;
    }

  public:
    #ifdef INFRA_ENABLED
      Infra(byte pin): pin(pin), receiver(this->BUFFERSIZE, pin) {}
    #else
      Infra(byte pin): pin(pin) {}
    #endif

    void setup() {}

    void reset() {
      this->bad_readings = 0;
    }

    byte getPin() {
      return this->pin;
    }

    unsigned char getShot() {
      // Checks received an IR signal
      #ifdef INFRA_ENABLED
        receiver.receive();
        if (!receiver.isEmpty()) {
          return this->translateIR();
        }
      #endif

      return 0;
    }

    bool isDisabled() {
      return this->getPin() == 0 || this->bad_readings > this->bad_readings_limit;
    }

    void loop() {
    }
};
