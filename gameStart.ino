#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
AsyncDelay stepTimer;
int whoopSpeed = 200;
bool buttonFlag = false;
bool winner = false;
bool loser = false;
bool nextStep = false;
const byte buttonOne = 4;
const byte buttonTwo = 5;

void setup() {
  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(buttonOne), buttonTripper, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonTwo), buttonTripper, RISING);
}

void loop() {
  if (!winner && !loser) {
    for (int i = 0; i < 10; i++) {
      int ledVal = map(i + 1, 0, 10, 0, 255);
      CircuitPlayground.setPixelColor(i, 50 - (3 * i), 50 - (3 * i), ledVal);
      if (i < 9) {
        CircuitPlayground.setPixelColor(9, 0xFF0000);
      }
      stepTimer.repeat();
      stepTimer.start(whoopSpeed, AsyncDelay::MILLIS);
      while (!nextStep) {
        if (stepTimer.isExpired()) {
          nextStep = true;
        }
        if (buttonFlag) {
          if (i == 9) {
            winner = true;
          } else {
            loser = true;
          }
          i = 9;
          nextStep = true;
          buttonFlag = false;
        }
      }
      nextStep = false;
      CircuitPlayground.clearPixels();
    }
  }
}

void buttonTripper() {
  buttonFlag = true;
}