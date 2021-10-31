#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

extern DroneData data;

void parseTemperature() {
  data.temperature = analogRead(TEMPERATURE_PIN) * 3.3 / 1024.0;
  data.temperature -= 0.5;
  data.temperature /= 0.01;
}
#endif
