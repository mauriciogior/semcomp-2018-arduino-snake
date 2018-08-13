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

typedef struct Point {
  int row = 0, col = 0;
  Point(int row = 0, int col = 0): row(row), col(col) {}
} Point;

typedef struct Snake {
  // direction constants
  static const short up     = 1;
  static const short right  = 2;
  static const short down   = 3; // 'down - 2' must be 'up'
  static const short left   = 4; // 'left - 2' must be 'right'

  Point head, food;
  int length = 3;
  int speed = 3;
  int direction = 1;

  // the age array: holds an 'age' of the every pixel in the matrix. If age > 0, it glows.
  // on every frame, the age of all lit pixels is incremented.
  // when the age of some pixel exceeds the length of the snake, it goes out.
  // age 1 is added in the current snake direction next to the last position of the snake head.
  int age[8][8] = {};
} Snake;

// initializes our variables
Button BUP    = Button(2, HIGH, 0);
Button BDOWN  = Button(3, HIGH, 0);
Button BLEFT  = Button(4, HIGH, 0);
Button BRIGHT = Button(5, HIGH, 0);
Snake snake;

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

  // set a random seed
  randomSeed(analogRead(A5));

  // initial position for snake
  snake.head.row = random(8);
  snake.head.col = random(8);

  snake.food.row = -1;
  snake.food.col = -1;
}

// the loop function runs over and over again forever
void loop() {
  long timestamp = millis() + snake.speed * 100; // when the next frame will be rendered

  while (millis() < timestamp) {
    readButton(&BUP);
    readButton(&BDOWN);
    readButton(&BLEFT);
    readButton(&BRIGHT);

    // blink the food
    matrix.setLed(0, snake.food.row, snake.food.col, millis() % 150 < 75 ? 1 : 0);
  };

  printFood();
  printSnake();
}

void readButton(Button *B) {
  int buttonState = digitalRead(B->PIN);

  // filter out any noise by setting a time buffer
  if ((millis() - B->lastDebounceTime) > DEBOUNCE) {

    // When button is pressed
    if (B->lastState != buttonState) {
      B->lastState = buttonState;

      // print out the value you read
      sprintf(buffer, "[%d] you have %d", B->PIN, buttonState);
      Serial.println(buffer);

      // Lets change the snake position
      if (B->PIN == BUP.PIN && snake.direction != Snake::down) snake.direction = Snake::up;
      if (B->PIN == BDOWN.PIN && snake.direction != Snake::up) snake.direction = Snake::down;
      if (B->PIN == BLEFT.PIN && snake.direction != Snake::right) snake.direction = Snake::left;
      if (B->PIN == BRIGHT.PIN && snake.direction != Snake::left) snake.direction = Snake::right;
    }

    B->lastDebounceTime = millis();
  }
}

void printFood() {
  if (snake.food.row == -1 || snake.food.col == -1) {
    // well... game over
    if (snake.length >= 64) {
      return;
    }

    // generate food until it is in the right position
    do {
      snake.food.col = random(8);
      snake.food.row = random(8);
    } while (snake.age[snake.food.row][snake.food.col] > 0);
  }
}

// calculate snake movement data
void printSnake() {
  switch (snake.direction) {
  case Snake::up:
    snake.head.row--;
    edges();
    matrix.setLed(0, snake.head.row, snake.head.col, 1);
    break;

  case Snake::right:
    snake.head.col++;
    edges();
    matrix.setLed(0, snake.head.row, snake.head.col, 1);
    break;

  case Snake::down:
    snake.head.row++;
    edges();
    matrix.setLed(0, snake.head.row, snake.head.col, 1);
    break;

  case Snake::left:
    snake.head.col--;
    edges();
    matrix.setLed(0, snake.head.row, snake.head.col, 1);
    break;

  default: // if the snake is not moving, exit
    return;
  }

  // if there is any age (snake body), this will cause the end of the game (snake must be moving)
  if (snake.age[snake.head.row][snake.head.col] != 0 && snake.direction != 0) {
    // gameOver = true;
    return;
  }

  // check if the food was eaten
  if (snake.head.row == snake.food.row && snake.head.col == snake.food.col) {
    snake.length++;
    snake.food.row = -1; // reset food
    snake.food.col = -1;
  }

  // increment ages if all lit leds
  updateAges();

  // change the age of the snake head from 0 to 1
  snake.age[snake.head.row][snake.head.col]++;
}

// causes the snake to appear on the other side of the screen if it gets out of the edge
void edges() {
  snake.head.col < 0 ? snake.head.col += 8 : 0;
  snake.head.col > 7 ? snake.head.col -= 8 : 0;
  snake.head.row < 0 ? snake.head.row += 8 : 0;
  snake.head.row > 7 ? snake.head.row -= 8 : 0;
}

// increment ages if all lit leds, turn off too old ones depending on the length of the snake
void updateAges() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      // if the led is lit, increment it's age
      if (snake.age[row][col] > 0 ) {
        snake.age[row][col]++;
      }

      // if the age exceeds the length of the snake, switch it off
      if (snake.age[row][col] > snake.length) {
        matrix.setLed(0, row, col, 0);
        snake.age[row][col] = 0;
      }
    }
  }
}
