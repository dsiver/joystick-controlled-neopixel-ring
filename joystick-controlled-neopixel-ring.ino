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

#define DEBUG
#define BAUD_RATE 9600
#define TAN_MIN -90
#define TAN_MAX 90
#define NEOPIXEL_NUM_PIXELS 16
#define NEOPIXEL_PIN_NUMBER 1
#define JOYSTICK_MAX 512
#define JOYSTICK_MIN -512
#define SENSITIVITY 400

int xCenter, yCenter, xReading, yReading, pixelNumber;
Adafruit_NeoPixel neopixelRing;

void setup() {
  xCenter = Esplora.readJoystickX();
  yCenter = Esplora.readJoystickY();
  xReading = 0;
  yReading = 0;
  pixelNumber = -1;
  Serial.begin(BAUD_RATE);
  neopixelRing = Adafruit_NeoPixel(NEOPIXEL_NUM_PIXELS, NEOPIXEL_PIN_NUMBER, NEO_GRB + NEO_KHZ800);
  neopixelRing.begin();
  neopixelRing.show();
}

void loop() {
  xReading = Esplora.readJoystickX();
  yReading = Esplora.readJoystickY();
  double joystickRadians;
  double joystickDegrees;

  if (abs(xReading) > SENSITIVITY || abs(yReading) > SENSITIVITY) {
    joystickDegrees = getDegrees(xReading, yReading);
    pixelNumber = map(joystickDegrees, 0, 330, 0, 15);
    
#ifdef DEBUG
    Serial.println("joystickDegrees: " + String(joystickDegrees) + " " +
                   "pixelNumber: " + String(pixelNumber));
#endif

  }
  else {
    pixelNumber = -1;
  }
  delay(2000);
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

#ifdef DEBUG
  Serial.println("JoystickX: " + String(xReading) + "\t" + "JoystickY: " + String(yReading) + "\t" + "calculation = " + String(calculation));
#endif

  return calculation;
}

