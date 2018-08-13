const int DEBOUNCE = 50;
char buffer[100];

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

// Initializes our buttons
Button BUP    = Button(2, HIGH, 0);
Button BDOWN  = Button(3, HIGH, 0);
Button BLEFT  = Button(4, HIGH, 0);
Button BRIGHT = Button(5, HIGH, 0);

// the setup function runs once when you press reset or power the board
void setup() {
  // Initialize pins
  pinMode(BUP.PIN, INPUT_PULLUP);
  pinMode(BDOWN.PIN, INPUT_PULLUP);
  pinMode(BLEFT.PIN, INPUT_PULLUP);
  pinMode(BRIGHT.PIN, INPUT_PULLUP);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
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
      Serial.println(buffer);
    }

    B->lastDebounceTime = millis();
  }
}
