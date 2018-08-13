const int BUTTON_UP = 2;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
  int buttonState = digitalRead(BUTTON_UP);

  if (buttonState == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
