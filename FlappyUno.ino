#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BIRD_SIZE 8
#define GRAVITY 1
#define JUMP_STRENGTH -5
#define PIPE_WIDTH 10
#define PIPE_GAP 30
#define BUTTON_PIN 2

int birdY;
int birdVelocity;
int pipeX;
int pipeGapY;
bool gameOver;
int score;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
  display.begin(i2c_Address, true); 
  display.clearDisplay();
  resetGame();
}

void loop() {
  if (gameOver) {
    if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
      resetGame();
    }
    displayGameOver();
  } else {
    if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
      birdVelocity = JUMP_STRENGTH;
    }
    updateGame();
    drawGame();
  }
  delay(50);
}

void resetGame() {
  birdY = SCREEN_HEIGHT / 2;
  birdVelocity = 0;
  pipeX = SCREEN_WIDTH;
  pipeGapY = random(10, SCREEN_HEIGHT - PIPE_GAP - 10);
  gameOver = false;
  score = 0;
}

void updateGame() {
  birdVelocity += GRAVITY;
  birdY += birdVelocity;
  pipeX -= 2;

  if (pipeX < -PIPE_WIDTH) {
    pipeX = SCREEN_WIDTH;
    pipeGapY = random(10, SCREEN_HEIGHT - PIPE_GAP - 10);
    score++;
  }

  if (birdY < 0 || birdY > SCREEN_HEIGHT - BIRD_SIZE || 
      (pipeX < BIRD_SIZE && (birdY < pipeGapY || birdY > pipeGapY + PIPE_GAP))) {
    gameOver = true;
  }
}

void drawGame() {
  display.clearDisplay();

  // Draw bird
  display.fillRect(10, birdY, BIRD_SIZE, BIRD_SIZE, SH110X_WHITE);

  // Draw pipe
  display.fillRect(pipeX, 0, PIPE_WIDTH, pipeGapY, SH110X_WHITE);
  display.fillRect(pipeX, pipeGapY + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - pipeGapY - PIPE_GAP, SH110X_WHITE);

  // Draw score
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  display.display();
}

void displayGameOver() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(20, 20);
  display.print("Game Over!");
  display.setCursor(20, 30);
  display.print("Score: ");
  display.print(score);
  display.setCursor(20, 40);
  display.print("Press to restart");
  display.display();
}
