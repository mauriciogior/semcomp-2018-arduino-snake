#include "LedControl.h"

char buffer[100];
const int DEBOUNCE = 50;

// our button structure
typedef struct Button {
  short PIN;

  int lastState;
  int lastDebounceTime;

  Button(short PIN, int lastState, int lastDebounceTime) {
    this->PIN = PIN;
    this->lastState = lastState;
    this->lastDebounceTime = lastDebounceTime;
  }

} Button;

// our Led Matrix structure
struct LED {
  static const short CS  = 10; // chip-select for LED matrix
  static const short CLK = 11; // clock for LED matrix
  static const short DIN = 12; // data-in for LED matrix
  static const short intensity = 8;
};

// initializes our variables
Button BUP    = Button(2, HIGH, 0);
Button BDOWN  = Button(3, HIGH, 0);
Button BLEFT  = Button(4, HIGH, 0);
Button BRIGHT = Button(5, HIGH, 0);

LedControl matrix(LED::DIN, LED::CLK, LED::CS, 1);

// the setup function runs once when you press reset or power the board
void setup() {
  // Initialize pins
  pinMode(BUP.PIN, INPUT_PULLUP);
  pinMode(BDOWN.PIN, INPUT_PULLUP);
  pinMode(BLEFT.PIN, INPUT_PULLUP);
  pinMode(BRIGHT.PIN, INPUT_PULLUP);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // initialize our matrix
  matrix.shutdown(0, false);
  matrix.setIntensity(0, LED::intensity);
  matrix.clearDisplay(0);
}

// the loop function runs over and over again forever
void loop() {
  readButton(&BUP);
  readButton(&BDOWN);
  readButton(&BLEFT);
  readButton(&BRIGHT);
}

void readButton(Button *B) {
  int buttonState = digitalRead(B->PIN);

  // filter out any noise by setting a time buffer
  if ((millis() - B->lastDebounceTime) > DEBOUNCE) {
    if (B->lastState != buttonState) {
      B->lastState = buttonState;

      // print out the value you read
      sprintf(buffer, "[%d] you have %d", B->PIN, B->lastState);

      // lets turn on/off LED on x - y axis
      matrix.setLed(0, B->PIN, B->PIN, buttonState);

      // outputs on serial
      Serial.println(buffer);
    }

    B->lastDebounceTime = millis();
  }
}
