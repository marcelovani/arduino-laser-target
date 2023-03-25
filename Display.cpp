// Class responsible for displaying the menu, scores and tests
//
// LCD docs
// https://github.com/olikraus/u8g2/wiki
// https://github.com/olikraus/u8glib/wiki/userreference
// https://github.com/olikraus/u8glib/wiki/device
// LCD Pinout
// Connect SCL -> Port 21 on Arduino Mega
// Connect SDA -> Port 20 on Arduino Mega

#ifdef DISPLAY_ENABLED
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST); // Fast I2C / TWI

enum Menus {
  START = 0,
  TEST = 1,
} MenuState;

class Display: public Runnable {
  Bounce bounce = Bounce();

  private:
    unsigned char menuOption = 0;
    byte encoderClkPin, encoderDtPin, encoderBtnPin;
    int lastClk = HIGH;

    unsigned char priority() {
      return 1;
    }

    boolean buttonPressed() {
      return bounce.fell();
    }

    void _print(u8g_uint_t x, u8g_uint_t y, const char *str) {
      u8g.drawStr(x, y, str);
    }

    void _print(u8g_uint_t x, u8g_uint_t y, String str) {
      char charBuf[50];
      str.toCharArray(charBuf, 50);
      u8g.drawStr(x, y, charBuf);
    }

    /**
     * Reads the knob.
     * Returns:
     *  0 no change
     *  1 for anti-clockwise turn
     *  2 for clock-wise turn
     */
    byte readKnob() {
      int newClk = digitalRead(this->encoderClkPin);
      if (newClk != lastClk) {
        // There was a change on the CLK pin
        lastClk = newClk;
        int dtValue = digitalRead(this->encoderDtPin);
        if (newClk == LOW && dtValue == HIGH) {
          // Turned anti-clockwise
          return 1;
        }
        if (newClk == LOW && dtValue == LOW) {
          // Turned clockwise
          return 2;
        }
      }
      return 0;
    }

    /**
     * Helper to change the menu states.
     */
    void readMenuKnob() {
      byte knob = readKnob();
      if (knob == 1) {
        if (menuOption < 1) {
          menuOption++;
          MenuState = TEST;
        }
      }
      else if (knob == 2) {
        if (menuOption > 0) {
          menuOption--;
          MenuState = START;
        }
      }
    }

    /**
     * Helper to change the targets.
     */
    void readTargetKnob() {
      byte knob = readKnob();
      if (knob == 1) {
        if (activeTarget < targetCount) {
          activeTarget++;
        }
        displayStatus(String("Target >"));
      }
      else if (knob == 2) {
        if (activeTarget > 1) {
          activeTarget--;
        }
        displayStatus(String("< Target"));
      }
    }

    void displayMenu() {
      delay(10);
      u8g.setFont(u8g_font_9x15B);
      u8g.firstPage();
      do {
        this->readMenuKnob();
        switch (MenuState) {
            case START:
              this->_print(5, 30, "> Start Game");
              this->_print(5, 50, "  Test Mode");
              if (buttonPressed()) {
                // Change game state.
                GameState = RESET;
              }
            break;

            case TEST:
              this->_print(5, 30, "  Start Game");
              this->_print(5, 50, "> Test Mode");
              if (buttonPressed()) {
                // Change game state.
                GameState = TESTING;
                // Initialise activeTarget.
                activeTarget = 1;
                // Update display.
                displayStatus("Shoot targets");
              }
              break;
          }    
      } while ( u8g.nextPage() );
    }

    void splash() {
      u8g.setFont(u8g_font_gdb12);
      u8g.firstPage();
      do {
        this->_print(1, 15, "LASER TARGET");
        u8g.drawCircle(64, 40, 20);
        u8g.drawCircle(64, 40, 14);
        u8g.drawCircle(64, 40, 9);
        u8g.drawCircle(64, 40, 3);
      } while ( u8g.nextPage() );
      delay(3000);
    }

  public:
    Display(byte clk, byte dt, byte btn) {
      this->encoderClkPin = clk;
      this->encoderDtPin = dt;
      this->encoderBtnPin = btn;
    }

    void setup() {
      pinMode(this->encoderClkPin, INPUT);
      pinMode(this->encoderDtPin, INPUT);

      bounce.attach(this->encoderBtnPin, INPUT_PULLUP);
      bounce.interval(25);

      this->splash();
      u8g.setColorIndex(1);
    }

    void print(u8g_uint_t x, u8g_uint_t y, String str) {
      u8g.firstPage();
      do {
        this->_print(x, y, str);
      } while ( u8g.nextPage() );
    }

    // Display Status.
    void displayStatus(String msg) {
      u8g.setFont(u8g_font_9x15B);
      String status = "";
      String active = "";

      // Prepare strings.
      unsigned char p;
      for (p = 1; p < targetCount+1; p++) {

        // Display port number or _.
        if (disabledTargets[p]) {
          status = status + "_";
        }
        else {
          status = status + String(p);
        }

        // Show active target.
        if (p == activeTarget) {
          active = active + "^";
        }
        else {
          active = active + " ";
        }
      }

      u8g.firstPage();
      do {
        this->_print(20, 15, "TARGETS");
        this->_print(20, 35, status);
        this->_print(20, 50, active);
        this->_print(01, 59, msg);
      } while ( u8g.nextPage() );
    }

    void displayScores() {
      u8g.setFont(u8g_font_9x15B);
      u8g.firstPage();
      do {
        this->_print(30, 14, "SCORES");
        this->_print(10, 33, "Player 1: " + String(scores[1]));
        this->_print(10, 47, "Player 2: " + String(scores[2]));
        this->_print(10, 62, "Player 3: " + String(scores[3]));
      } while ( u8g.nextPage() );
    }

    void displayHit(unsigned char player, unsigned char target) {
      u8g.setFont(u8g_font_fub17);
      u8g.firstPage();
      do {
        this->_print(20, 25, "Player: " + String(player));
        this->_print(27, 55, "HIT: " + String(target));
      } while ( u8g.nextPage() );
    }

    void loop() {
      bounce.update();
      switch (GameState) {
        case MENU:
          displayMenu();
          break;

        case PLAYING:
          readTargetKnob();
          if (buttonPressed()) {
            GameState = MENU;
          }
          break;

        case TESTING:
          readTargetKnob();
          if (buttonPressed()) {
            GameState = MENU;
          }
          break;
      }
    }
};
#else
  // Empty class when display is disabled.
  class Display: public Runnable {
    public:
      Display(byte clk, byte dt, byte btn) {
      }
      void setup() {
        // We don't have display or menu, so we go straight into the game.
        GameState = TESTING;
        activeTarget = 1;
      }
      unsigned char priority() {
        return 1;
      }
      void loop() {
      }
      void displayHit(unsigned char player, unsigned char target) {
        Serial.println("Player " + String(player) + " hit " + String(target));
      }
      void displayMenu() {
      }
      void displayStatus(String msg) {
        Serial.println("" + msg);
      }
      void displayScores() {
        Serial.println("Player 1: " + String(scores[1]));
      }
  };
#endif

// Create instance of display.
Display display(6, 7, 8);
