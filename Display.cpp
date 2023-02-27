// Class responsible for displaying the menu, scores and tests
//
// LCD docs
// https://github.com/olikraus/u8g2/wiki
// https://github.com/olikraus/u8glib/wiki/userreference
// https://github.com/olikraus/u8glib/wiki/device
// LCD Pinout
// Connect SCL -> Port 21 on Arduino Mega
// Connect SDA -> Port 20 on Arduino Mega

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST); // Fast I2C / TWI

enum Menus {
  START = 0,
  TEST = 1,
} MenuState;

class Display: public Runnable {
  Bounce bounce = Bounce();

  private:
    int menuOption = 0;
    byte encoderClkPin, encoderDtPin, encoderBtnPin;
    int lastClk = HIGH;

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

    void displayMenu() {
      u8g.firstPage();
      do {
        // Check rotary knob.
        int newClk = digitalRead(this->encoderClkPin);
        if (newClk != lastClk) {
          // There was a change on the CLK pin
          lastClk = newClk;
          int dtValue = digitalRead(this->encoderDtPin);
          if (newClk == LOW && dtValue == HIGH) {           
            if (menuOption < 1) {
              menuOption++;
            }
          }
          if (newClk == LOW && dtValue == LOW) {
            if (menuOption > 0) {
              menuOption--;
            }
          }
          MenuState = menuOption;
        }

        switch (MenuState) {
            case START:
              this->_print(10, 25, "> Start Game");
              this->_print(10, 45, "   Test Targets");
              if (buttonPressed()) {
                scores[0] = 0;
                scores[1] = 0;
                scores[2] = 0;
                displayScores();
                GameState = PLAYING;
              }
            break;

            case TEST:
              this->_print(10, 25, "   Start Game");
              this->_print(10, 45, "> Test Targets");
              if (buttonPressed()) {
                u8g.firstPage();
                do {
                  this->_print(10, 15, "TEST MODE");
                  this->_print(5, 35, "Shoot any target");
                } while ( u8g.nextPage() );
                GameState = TESTING;
              }
              break;
          }    
      } while ( u8g.nextPage() );
    }

    void splash() {
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

      u8g.setFont(u8g_font_gdb12);
      u8g.setColorIndex(1);
      this->splash();
    }

    void print(u8g_uint_t x, u8g_uint_t y, String str) {
      u8g.firstPage();
      do {
        this->_print(x, y, str);
      } while ( u8g.nextPage() );
    }
  
    void displayScores() {
      u8g.firstPage();
      do {
        this->_print(10, 15, "Player 1: " + String(scores[0]));
        this->_print(10, 35, "Player 2: " + String(scores[1]));
        this->_print(10, 55, "Player 3: " + String(scores[2]));
      } while ( u8g.nextPage() );
    }

    void displayTest(byte targetId, unsigned char playerId) {
      u8g.firstPage();
      do {
        this->_print(10, 15, "TEST MODE");
        this->_print(10, 35, "Target " + String(targetId) + " Hit");
        this->_print(10, 55, "Player " + String(playerId));
      } while ( u8g.nextPage() );
    }

    void loop() {
      bounce.update();

      switch (GameState) {
        case MENU:
          displayMenu();
          break;

        case PLAYING:
          if (buttonPressed()) {
            GameState = MENU;
          }
          break;

        case TESTING:
          if (buttonPressed()) {
            GameState = MENU;
          }
          break;
      }
    }
};

// Create instance of display.
Display display(6, 7, 8);
