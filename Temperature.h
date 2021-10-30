#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

extern CCDataPacket frame;

void parseTemperature() {
  frame.temperature.floatingPoint = analogRead(TEMPERATURE_PIN) * 3.3 / 1024.0;
  frame.temperature.floatingPoint -= 0.5;
  frame.temperature.floatingPoint /= 0.01;
}
#endif
