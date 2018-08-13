const int BUTTON_UP    = 2;
const int BUTTON_DOWN  = 3;
const int BUTTON_LEFT  = 4;
const int BUTTON_RIGHT = 5;
int lastStateUp = HIGH;
int lastStateDown = HIGH;
int lastStateLeft = HIGH;
int lastStateRight = HIGH;

char buffer[100];

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  int buttonStateUp = digitalRead(BUTTON_UP);
  int buttonStateDown = digitalRead(BUTTON_DOWN);
  int buttonStateLeft = digitalRead(BUTTON_LEFT);
  int buttonStateRight = digitalRead(BUTTON_RIGHT);

  if (lastStateUp != buttonStateUp) {
    lastStateUp = buttonStateUp;

    // print out the value you read
    sprintf(buffer, "[UP] you have %d", lastStateUp);
    Serial.println(buffer);
  }

  if (lastStateDown != buttonStateDown) {
    lastStateDown = buttonStateDown;

    // print out the value you read
    sprintf(buffer, "[DOWN] you have %d", lastStateDown);
    Serial.println(buffer);
  }

  if (lastStateLeft != buttonStateLeft) {
    lastStateLeft = buttonStateLeft;

    // print out the value you read
    sprintf(buffer, "[LEFT] you have %d", lastStateLeft);
    Serial.println(buffer);
  }

  if (lastStateRight != buttonStateRight) {
    lastStateRight = buttonStateRight;

    // print out the value you read
    sprintf(buffer, "[RIGHT] you have %d", lastStateRight);
    Serial.println(buffer);
  }
}
