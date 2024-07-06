#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Defines
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // QT-PY / XIAO
#define SCREEN_ADDRESS 0x3C

#define DINO_WIDTH 25
#define DINO_HEIGHT 26
#define DINO_INIT_X 10 // Dino initial spawn location
#define DINO_INIT_Y 29 // Dino initial spawn location

#define BASE_LINE_X 0
#define BASE_LINE_Y 54
#define BASE_LINE_X1 127
#define BASE_LINE_Y1 54

#define TREE1_WIDTH 11
#define TREE1_HEIGHT 23

#define TREE2_WIDTH 22
#define TREE2_HEIGHT 23

#define TREE_Y 35

#define JUMP_PIXEL 22 // Number of pixels dino will jump

#define PIN_BUTTON 2 // Digital pin for the button

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, SCREEN_ADDRESS);

static const unsigned char PROGMEM dino1[] = {
  // 'dino', 25x26px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0x0e, 0xff, 0x00,
  0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xc0, 0x00,
  0x00, 0x0f, 0xfc, 0x00, 0x40, 0x0f, 0xc0, 0x00, 0x40, 0x1f, 0x80, 0x00, 0x40, 0x7f, 0x80, 0x00,
  0x60, 0xff, 0xe0, 0x00, 0x71, 0xff, 0xa0, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x7f, 0xff, 0x80, 0x00,
  0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00,
  0x03, 0xfc, 0x00, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00,
  0x01, 0x0c, 0x00, 0x00, 0x01, 0x8e, 0x00, 0x00
};

static const unsigned char PROGMEM tree1[] = {
  // 'tree1', 11x23px
  0x1e, 0x00, 0x1f, 0x00, 0x1f, 0x40, 0x1f, 0xe0, 0x1f, 0xe0, 0xdf, 0xe0, 0xff, 0xe0, 0xff, 0xe0,
  0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xc0, 0xff, 0x00,
  0xff, 0x00, 0x7f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00,
  0x1f, 0x00, 0x1f, 0x00
};

static const unsigned char PROGMEM tree2[] = {
  // 'tree2', 22x23px
  0x1e, 0x01, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x4f, 0xe8, 0x1f, 0xff, 0xfc, 0x1f, 0xff, 0xfc, 0xdf,
  0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff,
  0xfc, 0xff, 0xef, 0xfc, 0xff, 0x83, 0xfc, 0xff, 0x03, 0xfc, 0xff, 0x03, 0xf8, 0x7f, 0x03, 0xe0,
  0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f,
  0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0
};

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BUTTON, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor

  // Initialize the display
  display.begin(SCREEN_ADDRESS, true); // Address 0x3C default
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

  introMessage();

  // Run game in loop
  while (1) {
    if (digitalRead(PIN_BUTTON) == LOW) { // Check if button is pressed
      delay(50); // Debounce delay
      while (digitalRead(PIN_BUTTON) == LOW); // Wait for button release
      play();
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

void introMessage() {
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 5);
  display.println("Dino Game");

  display.setTextSize(1);
  display.setCursor(5, 45);
  display.println("Press Button To Play");

  display.display();
}

void moveDino(int16_t *y, int type = 0) {
  display.drawBitmap(DINO_INIT_X, *y, dino1, DINO_WIDTH, DINO_HEIGHT, SH110X_WHITE);
}

void moveTree(int16_t *x, int type = 0) {
  if (type == 0) {
    display.drawBitmap(*x, TREE_Y, tree1, TREE1_WIDTH, TREE1_HEIGHT, SH110X_WHITE);
  } else if (type == 1) {
    display.drawBitmap(*x, TREE_Y, tree2, TREE2_WIDTH, TREE2_HEIGHT, SH110X_WHITE);
  }
}

void gameOver(int score = 0) {
  display.clearDisplay();

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SH110X_WHITE);
  display.setCursor(20, 5);
  display.println("Game Over");

  display.setTextSize(1);
  display.setCursor(0, 45);
  display.print("Score: ");
  display.print(score);

  display.display();
  delay(2000);

  display.clearDisplay();
  introMessage();
}

void displayScore(int score = 0) {
  display.setCursor(95, 0);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.print("Score:");
  display.setCursor(95, 10);
  display.print(score);
}

void play() {
  int16_t dino_y = DINO_INIT_Y;
  int16_t tree_x = 127;
  int16_t tree1_x = 64;
  int tree_type = random(0, 2);
  int tree_type1 = random(0, 2);
  int score = 0;
  int jump = 0;

  while (1) {
    display.clearDisplay();

    if (digitalRead(PIN_BUTTON) == LOW) { // Check if button is pressed
      delay(50); // Debounce delay
      while (digitalRead(PIN_BUTTON) == LOW); // Wait for button release
      if (jump == 0) {
        dino_y = DINO_INIT_Y - JUMP_PIXEL;
        jump = 1;
      }
    }

    if (jump > 0) {
      dino_y += 2;
      jump++;
    }

    if (jump == 12) {
      jump = 0;
    }

    moveDino(&dino_y);

    if (tree_x <= -22) {
      tree_x = 127;
      tree_type = random(0, 2);
    }

    if (tree1_x <= -22) {
      tree1_x = 127;
      tree_type1 = random(0, 2);
    }

    moveTree(&tree_x, tree_type);
    moveTree(&tree1_x, tree_type1);

    display.drawLine(BASE_LINE_X, BASE_LINE_Y, BASE_LINE_X1, BASE_LINE_Y1, SH110X_WHITE);

    if (((tree_x <= 35) && (tree_x >= 5)) || ((tree1_x <= 35) && (tree1_x >= 5))) {
      if (dino_y >= 25) {
        gameOver(score);
        break;
      }
    }

    displayScore(score);
    display.display();

    tree_x -= 3;
    tree1_x -= 3;

    score++;
  }
}
