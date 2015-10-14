/*
 * Joystick Controlled Neopixel Ring
 * Board: Arduino Esplora
 *
 * David Siver
 * CS 4985
 * Fall 2015
 */
#include <Esplora.h>
#include <Adafruit_NeoPixel.h>

#define DEBUG 1
#define DELAY 20
#define TEST_DELAY 10
#define BAUD_RATE 9600
#define TAN_MIN -90
#define TAN_MAX 90
#define NEOPIXEL_NUM_PIXELS 16
#define NEOPIXEL_PIN_NUMBER 7
#define NEOPIXEL_BRIGHTNESS 20
#define NO_PIXEL -1
#define JOYSTICK_MAX 512
#define JOYSTICK_MIN -512
#define SENSITIVITY 400

int xCenter, yCenter, xReading, yReading, pixelNumber, oldPixelNumber;
int upState, downState, leftState, rightState, oldUpState, oldDownState, oldLeftState, oldRightState;
Adafruit_NeoPixel neoPixel;

void setup() {
  upState = 0;
  downState = 0;
  leftState = 0;
  rightState = 0;
  oldUpState = 0;
  oldDownState = 0;
  oldLeftState = 0;
  oldRightState = 0;
  xCenter = Esplora.readJoystickX();
  yCenter = Esplora.readJoystickY();
  xReading = 0;
  yReading = 0;
  pixelNumber = -1;
  oldPixelNumber = pixelNumber;
  Serial.begin(BAUD_RATE);
  neoPixel = Adafruit_NeoPixel(NEOPIXEL_NUM_PIXELS, NEOPIXEL_PIN_NUMBER, NEO_GRB + NEO_KHZ800);
  neoPixel.begin();
  neoPixel.show();
  pinMode(NEOPIXEL_PIN_NUMBER, OUTPUT);
}

void loop() {
  controlNeoPixelWithJoystick();
  //spinner();
  //controlNeoPixelWithButtons();
  delay(DELAY);
}

void controlNeoPixelWithJoystick() {
  xReading = Esplora.readJoystickX();
  yReading = Esplora.readJoystickY();
  double joystickRadians;
  double joystickDegrees;
  if (abs(xReading) > SENSITIVITY || abs(yReading) > SENSITIVITY) {
    joystickDegrees = getDegrees(xReading, yReading);
    pixelNumber = map(joystickDegrees, 0, 330, 0, 15);
    pixelNumber = constrain(pixelNumber, 0, 15);
    if (oldPixelNumber != pixelNumber) {
      neoPixel.setPixelColor(pixelNumber, neoPixel.Color(0, NEOPIXEL_BRIGHTNESS, 0));
      neoPixel.setPixelColor(oldPixelNumber, neoPixel.Color(0, 0, 0));
    }
  }
  else{
    neoPixel.setPixelColor(pixelNumber, neoPixel.Color(0, 0, 0));
    pixelNumber = NO_PIXEL;
  }
  neoPixel.show();
  oldPixelNumber = pixelNumber;
}

double getDegrees(int x, int y) {
  int xCalibrated = x - xCenter;
  int yCalibrated = y - yCenter;
  xCalibrated = -xCalibrated;
  yCalibrated = -yCalibrated;
  double calculation = atan2(yCalibrated, xCalibrated) * 180 / PI;
  if (calculation < 0) {
    calculation += 360;
  }
  return calculation;
}

void spinner() {
  int brightness = map(Esplora.readSlider(), 0, 1023, 0, 255);
  for (int i = 0; i < neoPixel.numPixels(); i++) {
    if (Esplora.readButton(SWITCH_UP) == LOW) {
      neoPixel.setPixelColor(i, neoPixel.Color(brightness, 0, 0));
    }
    else {
      neoPixel.setPixelColor(i, neoPixel.Color(0, brightness, 0));
    }
    neoPixel.show();
    delay(TEST_DELAY);
    for (int j = neoPixel.numPixels() - 1; j >= 0; j--) {
      neoPixel.setPixelColor(i, neoPixel.Color(0, 0, 0));
      neoPixel.show();
    }
  }
}

void controlNeoPixelWithButtons() {
  upState = Esplora.readButton(SWITCH_UP);
  downState = Esplora.readButton(SWITCH_DOWN);
  leftState = Esplora.readButton(SWITCH_LEFT);
  rightState = Esplora.readButton(SWITCH_RIGHT);
  if (upState == LOW) {
    if (oldUpState != upState) {
      pixelNumber++;
      if (pixelNumber == neoPixel.numPixels()) {
        pixelNumber = 0;
      }
    }
  }
  else if (downState == LOW) {
    if (oldDownState != downState) {
      pixelNumber--;
      if (pixelNumber < 0) {
        pixelNumber = neoPixel.numPixels() - 1;
      }
    }
  }
  else if (leftState == LOW) {
    if (oldLeftState != leftState) {
      int newPixel = pixelNumber - 8;
      if (newPixel < 0) {
        newPixel += 16;
      }
      pixelNumber = newPixel;
    }
  }
  else if (rightState == LOW) {
    if (oldRightState != rightState) {
      int newPixel = pixelNumber + 8;
      if (newPixel > neoPixel.numPixels() - 1) {
        newPixel -= 16;
      }
      pixelNumber = newPixel;
    }
  }
  neoPixel.setPixelColor(pixelNumber, neoPixel.Color(0, NEOPIXEL_BRIGHTNESS, 0));
  neoPixel.show();
  oldUpState = upState;
  oldDownState = downState;
  oldLeftState = leftState;
  oldRightState = rightState;
  neoPixel.setPixelColor(oldPixelNumber, neoPixel.Color(0, 0, 0));
  oldPixelNumber = pixelNumber;
}
