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
#define DELAY 50
#define BAUD_RATE 9600
#define TAN_MIN -90
#define TAN_MAX 90
#define NEOPIXEL_NUM_PIXELS 16
#define NEOPIXEL_NO_PIXEL -1
#define NEOPIXEL_PIN_NUMBER 1
#define NEOPIXEL_BRIGHTNESS 20
#define JOYSTICK_MAX 512
#define JOYSTICK_MIN -512
#define SENSITIVITY 400

int xCenter, yCenter, xReading, yReading, pixelNumber, oldPixelNumber;
Adafruit_NeoPixel neoPixelRing;
#ifdef DEBUG
int up, down, left, right, oldUp, oldDown, oldLeft, oldRight;
#endif

void setup() {
  xCenter = Esplora.readJoystickX();
  yCenter = Esplora.readJoystickY();
  xReading = 0;
  yReading = 0;
  Serial.begin(BAUD_RATE);
  pinMode(NEOPIXEL_PIN_NUMBER, OUTPUT);
  neoPixelRing = Adafruit_NeoPixel(NEOPIXEL_NUM_PIXELS, NEOPIXEL_PIN_NUMBER), NEO_GRB + NEO_KHZ800;
  neoPixelRing.begin();
  neoPixelRing.show();
}

void loop() {
  controlNeoPixelWithJoystick();
  //spinner();
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
    neoPixelRing.setPixelColor(pixelNumber, neoPixelRing.Color(0, NEOPIXEL_BRIGHTNESS, 0));
    neoPixelRing.show();

#ifdef DEBUG
    Serial.println("joystickDegrees: " + String(joystickDegrees) + " " +
                   "pixelNumber: " + String(pixelNumber));
#endif

  }
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

void spinner() {
  for (int i = 0; i < neoPixelRing.numPixels(); i++) {
    if (Esplora.readButton(SWITCH_UP) == LOW) {
      neoPixelRing.setPixelColor(i, neoPixelRing.Color(NEOPIXEL_BRIGHTNESS, 0, 0));
    }
    else {
      neoPixelRing.setPixelColor(i, neoPixelRing.Color(0, NEOPIXEL_BRIGHTNESS, 0));
    }
    neoPixelRing.show();
    delay(20);
    for (int j = neoPixelRing.numPixels() - 1; j >= 0; j--) {
      neoPixelRing.setPixelColor(i, neoPixelRing.Color(0, 0, 0));
      neoPixelRing.show();
    }
  }
}

