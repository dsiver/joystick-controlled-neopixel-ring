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
#define serialBaudRate 9600

int xAxis, yAxis;

void setup() {
  xAxis = 0;
  yAxis = 0;
  Serial.begin(serialBaudRate);
}

void loop() {
  xAxis = Esplora.readJoystickX();
  yAxis = Esplora.readJoystickY();

  Serial.println("JoystickX: " + String(xAxis) + " " +
               "JoystickY: " + String(yAxis));
  delay(500);
}
