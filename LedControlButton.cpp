// @todo decide what todo with this class
class LedControlButton: public Button {
    Led &led;

  public:
    LedControlButton(byte attachToPin, Led &attachToLed) :
      Button(attachToPin),
      led(attachToLed) {
    }
  protected:
    void click() {
      led.powerToggle();
    }
};
