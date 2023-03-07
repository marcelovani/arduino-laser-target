#ifdef INFRA_ENABLED

#define RAW_BUFFER_LENGTH 18
#define NO_DECODER
#define DEBUG

#include <IRremote.hpp>

IRrecv receiver;

// Infra Red Receiver.
class Infra: public Runnable {
    uint_fast8_t pin;
    const uint_fast8_t bufferSize = 18;

  private:
    // Count bad readings.
    byte bad_readings;
    // Bad readings limit.
    const byte bad_readings_limit = 50;
    // Enable/disable IR.
    bool disabled;
    // User detection threshold.
    uint32_t t = 1100;
    // Which gun shot the target.
    unsigned char player;

    unsigned char priority() {
      return 2;
    }

    /**
     * Compare the received data and try to get the player who shot the target.
     */
    void getPlayer() {
      uint32_t d1, d2;
      this->player = 0;

      if (this->isDisabled()) {
        return;
      }

      // Decode IR interrupt data.
      if (!receiver.decode()) {
        return;
      }

      receiver.resume();
      uint_fast8_t lengh = receiver.decodedIRData.rawDataPtr->rawlen;

      if (receiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
        // Too many bytes received.
      }
      else if (lengh < RAW_BUFFER_LENGTH) {
        // Too few bytes received.
      }
      else {
        // Use data length to get the duration of the 3th and 1st position from the end.
        d1 = receiver.decodedIRData.rawDataPtr->rawbuf[lengh - 3] * MICROS_PER_TICK;
        d2 = receiver.decodedIRData.rawDataPtr->rawbuf[lengh - 1] * MICROS_PER_TICK;
      }

      // Ignore noise and bad readings.
      if (d1 > t * 3 || d2 > t * 3) {
        this->bad_readings++;
        return;
      }

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
    }

  public:
    Infra(uint_fast8_t pin): pin(pin) {}

    void setup() {
      receiver.begin(this->pin);
    }

    void reset() {
      this->bad_readings = 0;
    }

    /**
     * Disables the timer for IR reception.
     */
    void stop() {
      receiver.stop();
    }

    void resume() {
      receiver.resume();
    }

    void changePin() {
      receiver.setReceivePin(this->pin);
    }

    uint_fast8_t getPin() {
      return this->pin;
    }

    // Checks received an IR signal
    unsigned char getShot() {
      // Change receiver pin.
      this->changePin();
      // Get player.
      this->getPlayer();

      return this->player;
    }

    bool isDisabled() {
      if (!this->disabled) {
        if (this->getPin() == 0 || (this->bad_readings > this->bad_readings_limit)) {
          Serial.println("Pin " + String(this->getPin()));
          Serial.println("Bad readings " + String(this->bad_readings));
          this->disabled = true;
        }
      }
      return this->disabled;
    }

    void loop() {
      // Loop will be controlled by parent class (Target).
    }
};

#endif