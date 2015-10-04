/*
 * Joystick Controlled Neopixel Ring
 * Board: Arduino Esplora
 *
 * David Siver
 * CS 4985
 * Fall 2015
 */
#include <Esplora.h>

#define DEBUG
#define BAUD_RATE 9600
#define TAN_MIN -90
#define TAN_MAX 90
#define NUM_PIXELS 16
#define JOYSTICK_MAX 512
#define JOYSTICK_MIN -512
#define SENSITIVITY 400

int xCenter, yCenter, xReading, yReading;

void setup() {
  xCenter = Esplora.readJoystickX();
  yCenter = Esplora.readJoystickY();
  xReading = 0;
  yReading = 0;
  Serial.begin(BAUD_RATE);
}

void loop() {
  xReading = Esplora.readJoystickX();
  yReading = Esplora.readJoystickY();
  double joystickToRadians;
  if (abs(xReading) > SENSITIVITY || abs(yReading) > SENSITIVITY){
    joystickToRadians = calculateRadians(xReading, yReading);
  }
   
  delay(500);
}

double calculateRadians(int x, int y){
  int xCalibrated = x - xCenter;
  int yCalibrated = y - yCenter;
  xCalibrated = -xCalibrated;
  yCalibrated = -yCalibrated;
  double calculation = atan2(yCalibrated, xCalibrated);
  calculation = constrain(calculation, -PI, PI);

  Serial.println("JoystickX: " + String(xReading) + "\t" +
                 "JoystickY: " + String(yReading) + "\t" + 
                 "atan2(" + String(yCalibrated) + ", " + String(xCalibrated) + ") = " +
                 String(calculation));
  
  return calculation;
}

