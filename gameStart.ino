// libraries
#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
// spinner timer variables
AsyncDelay stepTimer;
int whoopSpeed = 200;
// interrupt variables
bool buttonFlag = false;
bool switchFlag = false;
const byte buttonOne = 4;
const byte buttonTwo = 5;
const byte switchPin = 7;
// which screen to run
bool winner = false;
bool loser = false;
bool newGame = true;
// utility booleans
bool nextStep = false;
bool firstTime = true;
bool sound = false;
// level counter
int level = 1;

// setup function
void setup() {
// intitializes some things
  CircuitPlayground.begin();
  Serial.begin(9600);
// sets interrupts
  attachInterrupt(digitalPinToInterrupt(buttonOne), buttonTripper, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonTwo), buttonTripper, RISING);
  attachInterrupt(digitalPinToInterrupt(switchPin), switchTripper, CHANGE);
}

// main loop function
void loop() {
// set brightness
  CircuitPlayground.setBrightness(15);
// play sounds?
  if (switchFlag) {
    delay(5);
    sound = !sound;
    switchFlag = false;
  }
// instructions
/* The serial monitor is kinda weird, so this only works if you open the serial monitor after the program has
fully uploaded, otherwise it'll just skip this section. */
  if (firstTime) {
    while(!Serial);
    Serial.println("The single blinking red light is the new game screen.");
    Serial.println("Press either button to start a game.");
    Serial.println("Press either button when the spinner lands on the solid red light.");
    Serial.println("All lights flashing white means you beat the level, and the spinner will get faster.");
    Serial.println("All lights flashing red means you lose, and you have to start over.");
    Serial.println("Toggle the switch to turn sound on and off.");
    firstTime = false;
  }
// main game
  if (!winner && !loser && !newGame) {
    for (int i = 0; i < 10; i++) {
// sets color of pixel based on it's location
      int ledVal = map(i + 1, 0, 10, 0, 255);
      CircuitPlayground.setPixelColor(i, 50 - (3 * i), 50 - (3 * i), ledVal);
      if (i < 9) {
        CircuitPlayground.setPixelColor(9, 0xFF0000);
      }
// speed of spinner
      stepTimer.repeat();
      stepTimer.start(whoopSpeed, AsyncDelay::MILLIS);
      while (!nextStep) {
        if (stepTimer.isExpired()) {
          nextStep = true;
        }
// checks to see if the button has been pressed
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
// win animation
  } else if (winner) {
// flashes lights
    for (int i = 0; i < 9; i++) {
      CircuitPlayground.setPixelColor(i, 0xFFFFFF);
    }
    if (sound) {
      CircuitPlayground.playTone(950.00, 120);
    }
    delay(200);
    CircuitPlayground.clearPixels();
    delay(200);
    for (int i = 0; i < 9; i++) {
      CircuitPlayground.setPixelColor(i, 0xFFFFFF);
    }
    if (sound) {
      CircuitPlayground.playTone(950.00, 120);
    }
    delay(200);
    CircuitPlayground.clearPixels();
    delay(200);
// spins lights back to beginning
    for (int i = 9; i >= 0; i--) {
      int ledVal = map(i + 1, 0, 10, 0, 255);
      CircuitPlayground.setPixelColor(i, 50 - (3 * i), 50 - (3 * i), ledVal);
      delay(100);
      CircuitPlayground.clearPixels();
    }
// increments level, speeds up spinner
    winner = false;
    whoopSpeed = whoopSpeed - 10;
    level++;
    Serial.print("Level: ");
    Serial.println(level);
    buttonFlag = false;
// lose animation
  } else if (loser) {
// flashes lights
    for (int i = 0; i < 9; i++) {
      CircuitPlayground.setPixelColor(i, 0xFF0000);
    }
    if (sound) {
      CircuitPlayground.playTone(950.00, 120);
    }
    delay(200);
    CircuitPlayground.clearPixels();
    delay(200);
    for (int i = 0; i < 9; i++) {
      CircuitPlayground.setPixelColor(i, 0xFF0000);
    }
    if (sound) {
      CircuitPlayground.playTone(950.00, 120);
    }
    delay(200);
    CircuitPlayground.clearPixels();
    delay(150);
// prints lose message
    Serial.print("You beat ");
    Serial.print(level - 1);
    Serial.println(" levels.");
// resets level and spinner speed
    newGame = true;
    loser = false;
    whoopSpeed = 200;
    level = 1;
    buttonFlag = false;
// new game animation
  } else if (newGame) {
// blinks light
    CircuitPlayground.setPixelColor(9, 0xFF0000);
    delay(200);
    CircuitPlayground.setPixelColor(9, 0x000000);
    delay(200);
// checks to start new game
    if (buttonFlag) {
      delay(5);
      newGame = false;
      buttonFlag = false;
      Serial.print("Level: ");
      Serial.println(level);
    }
  }
}

// button interrupt function
void buttonTripper() {
  buttonFlag = true;
}

// swutch interrupt function
void switchTripper() {
  switchFlag = true;
}