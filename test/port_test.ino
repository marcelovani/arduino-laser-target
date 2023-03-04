// Used to test the ports

const int PORTS = 7;
const int PINS = 8;
bool SHORT=false;

// Mapping for 7 Ports vs 8 Pins each port
// Brown            - 1 (Ground)
// White/Brown      - 2 (Servo)
// Green            - 3 (RGB Green)
// White/Blue       - 4 (IR)
// Blue             - 5 (RGB Blue)
// White/Green      - 6 (Laser)
// Orange           - 7 (+5v)
// White/Orange     - 8 (RGB Red)
int pins[PORTS+1][PINS+1] = {
  //     1   2   3   4   5   6   7   8    <- Wires
  { 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // Unused
  { 00, 00,  3, 16,  2, 17, 15, 00, 14 }, // 1 -> ok
  { 00, 00, 43, 45, 47, 49, 51, 00, 53 }, // 2 -> ok
  { 00, 00, 50, 39, 37, 35, 41, 00, 52 }, // 3 -> ok
  { 00, 00, 32, 42, 48, 46, 44, 00, 33 }, // 4 -> ok
  { 00, 00, 26, 34, 40, 38, 36, 00, 27 }, // 5 -> ok
  { 00, 00, 25, 28, 31, 30, 29, 00, 24 }, // 6 -> ok
  { 00, 00, 23,  5, 19, 18, 22, 00,  4 }  // 7 -> ok
};

void setup() {
    Serial.begin(115200);

    // Test to make sure that no pin is in short.
    int p, i, pin;
    for(p = 1; p < PORTS+1; p++) {
        Serial.println("Port " + String(p));
        for(i = 1; i < PINS+1; i++) {
            pin = pins[p][i];
            if (pin > 0) {              
              Serial.print("Pos " + String(i) + " Pin " + String(pin) + " ");
              pinMode(pin, INPUT);
              delay(50);
              // Test if in short.
              if (digitalRead(pin) == HIGH) {
                  Serial.println("SHORT");
              }
              else {
                // Set mode to output.
                Serial.println("OK");
              }
              delay(100);
              // Skip IR port.
              if (i != 4) {
                pinMode(pin, OUTPUT);
              }
            }
        }
    }
}

void loop() {
  // LED test.
  int p, i, pin;
  // LED on.
  for(i = 1; i < PINS+1; i++) {
    for(p = 1; p < PORTS+1; p++) {
      pin = pins[p][i];
      // Skip IR port.
      if (i != 4) {
        digitalWrite(pin, HIGH);
      }
    }
    delay(500);
    // LED off.
    for(p = 1; p < PORTS+1; p++) {
      pin = pins[p][i];
      // Skip IR port.
      if (i != 4) {
        digitalWrite(pin, LOW);
      }
    }
  }
}
