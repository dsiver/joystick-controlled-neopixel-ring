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
Adafruit_NeoPixel neoPixel;

void setup() {
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
