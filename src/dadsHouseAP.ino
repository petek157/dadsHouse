/*
 * Project dadsHouseAP
 * Description:
 * Author:
 * Date:
 */

#include "Adafruit_MCP9808.h"


Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

int currentTemp;
int offset;
int warningSent = false;
int lowPoint = 40;

unsigned long lastMillis = 0;
unsigned long delayMillis = 300000;

void setup() {

  Particle.variable("temp", currentTemp);
  Particle.variable("offset", offset);
  Particle.variable("lowPoint", lowPoint);

  Particle.function("chgOffset", changeOffset);
  Particle.function("chgLowPoint", changeLowPoint);
  
  if (!tempsensor.begin()) {
    while (1);
  }

}

void loop() {

unsigned long currentMillis = millis();

  if (lastMillis == 0 || (currentMillis - lastMillis) > delayMillis) {
    lastMillis = currentMillis;

    float temp = (((tempsensor.readTempC() * 1.8) + 32) + offset);
      if (temp > 0 && temp < 100) {
          if (temp > (currentTemp + 3) || temp < (currentTemp - 3)) {
            currentTemp = temp;
            if (temp < lowPoint && !warningSent) {
              //Send warning about low temperature
              warningSent = true;
            }
            if (temp > (lowPoint + 3) && warningSent) {
              //consider an alert the says temp has recovered above low point
              warningSent = false;
            }
          }
      }
  }

}

int changeOffset(String n_offset) {
  offset = n_offset.toInt();
  return 1;
}

int changeLowPoint(String n_lowPoint) {
  lowPoint = n_lowPoint.toInt();
  return 1;
}