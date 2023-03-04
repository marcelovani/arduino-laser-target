#ifdef INFRA_ENABLED
  #if !defined(RAW_BUFFER_LENGTH)
    #define RAW_BUFFER_LENGTH 18
  #endif
  #define NO_LED_FEEDBACK_CODE
  #include <IRremote.hpp>
#endif

// Infra Red Receiver.
class Infra: public Runnable {
    uint_fast8_t pin;
    #ifdef INFRA_ENABLED
      IRrecv receiver;
    #endif

  private:
    // Count bad readings.
    byte bad_readings;
    const byte bad_readings_limit = 3;
    bool disabled;
    // Duration readings.
    uint32_t d1, d2;
    // Threshold. @todo make configurable
    uint32_t t = 1100;
    // Which gun shot the target.
    unsigned char player;

    unsigned char priority() {
      return 2;
    }

    void translateIR() {
      this->player = 0;

      #ifdef INFRA_ENABLED
        if (this->isDisabled()) {
          return 0;
        }

        uint_fast16_t len = receiver.decodedIRData.rawDataPtr->rawlen;

        // Ignore noise and bad readings.
        if (len < 17) {
          return 0;
        }

        // Use data length to get the duration of the 4th position from the end.
        d1 = receiver.decodedIRData.rawDataPtr->rawbuf[len - 3] * MICROS_PER_TICK;
        // Use data length to get the duration of the 2th position from the end.
        d2 = receiver.decodedIRData.rawDataPtr->rawbuf[len - 1] * MICROS_PER_TICK;

        // Ignore noise and bad readings.
        if (d1 > t * 3 || d2 > t * 3) {
          this->bad_readings++;
          Serial.println("IR " + String(this->pin) + " ERROR: Bad code: " + String(d1) + " " + String(d2));
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
    }

  public:
    #ifdef INFRA_ENABLED
      Infra(uint_fast8_t pin): pin(pin), receiver() {}
    #else
      Infra(uint_fast8_t pin): pin(pin) {}
    #endif

    void setup() {
      receiver.begin(this->pin, false);
    }

    void reset() {
      this->bad_readings = 0;
    }

    uint_fast8_t getPin() {
      return this->pin;
    }

    unsigned char getShot() {
      // Checks received an IR signal
      #ifdef INFRA_ENABLED
        if (receiver.decode()) {
          // Serial.println(receiver.decodedIRData.flags);
          if (receiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
            Serial.println("IR Data overflow");
          }
          else {
            this->translateIR();
            receiver.resume();
            return this->player;
          }
        }
      #endif
      return 0;
    }

    bool isDisabled() {
      if (!this->disabled) {
        if (this->getPin() == 0) {
          #ifdef INFRA_ENABLED
            receiver.stop();
          #endif
          this->disabled = true;
        }
        else if (this->bad_readings > this->bad_readings_limit) {
          Serial.println("Disabled IR due to bad readings");
          #ifdef INFRA_ENABLED
            receiver.stop();
          #endif
          this->disabled = true;
        }
      }
      return this->disabled;
    }

    void loop() {
    }
};
