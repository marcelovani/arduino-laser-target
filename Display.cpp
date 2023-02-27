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
#include <Bounce2.h>

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
                GameState = PLAYING;
              }
            break;

            case TEST:
              this->_print(10, 25, "   Start Game");
              this->_print(10, 45, "> Test Targets");
              if (buttonPressed()) {
                GameState = TESTING;
              }
              break;
          }    
      } while ( u8g.nextPage() );
    }

    void displayTest() {
      u8g.firstPage();
      do {
        this->_print(10, 15, "TEST MODE");
        if (buttonPressed()) {
          GameState = MENU;
        }
      } while ( u8g.nextPage() );
    }

    void displayScores() {
      u8g.firstPage();
      do {
        this->_print(10, 15, "Player 1: 10");
        this->_print(10, 35, "Player 2: 1");
        this->_print(10, 55, "Player 3: 0");
        if (buttonPressed()) {
          GameState = MENU;
        }
      } while ( u8g.nextPage() );
    }

  public:
    Display(byte clk, byte dt, byte btn) {
      this->encoderClkPin = clk;
      this->encoderDtPin = dt;
      this->encoderBtnPin = btn;
    }

    void setup() {
      pinMode(this->encoderClkPin, INPUT);
      pinMode(encoderDtPin, INPUT);
      pinMode(encoderBtnPin, INPUT_PULLUP);

      bounce.attach(encoderBtnPin, INPUT_PULLUP);
      bounce.interval(25);

      u8g.setFont(u8g_font_gdb12);
      u8g.setColorIndex(1);
    }

    void print(u8g_uint_t x, u8g_uint_t y, const char *str) {
      u8g.firstPage();
      do {
        this->_print(x, y, str);
      } while ( u8g.nextPage() );
    }
  
    void splash() {
      u8g.firstPage();
      do {
        u8g.drawStr(1, 15, "LASER TARGET");
        u8g.drawCircle(64, 40, 20);
        u8g.drawCircle(64, 40, 14);
        u8g.drawCircle(64, 40, 9);
        u8g.drawCircle(64, 40, 3);
      } while ( u8g.nextPage() );
      delay(5000);
    }

    void loop() {
      bounce.update();

      switch (GameState) {
        case MENU:
          displayMenu();
          break;

        case PLAYING:
          displayScores();
          break;

        case TESTING:
          displayTest();
          break;
      }
    }
};
