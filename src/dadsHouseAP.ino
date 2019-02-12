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

unsigned long lastMillis = 0;
unsigned long delayMillis = 300000;

void setup() {

  Particle.variable("temp", currentTemp);
  Particle.variable("offset", offset);

  Particle.function("chgOffset", changeOffset);
  
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
            Particle.publish("Temperature", sensorData);
            currentTemp = temp;
          }
      }
  }

}

int changeOffset(String n_offset) {
  offset = n_offset.toInt();

  return 1;
}