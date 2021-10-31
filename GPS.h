#ifndef __GPS_H__
#define __GPS_H__

#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);
extern DroneData data;

void setupGPS() {
  Serial.print("GPS: Initialising");
  gpsSerial.begin(4800, SERIAL_8N1, GPS_RXPIN, GPS_TXPIN);
  Serial.println("GPS: Success");
}

void parseGPS() {
  while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  if (gps.location.isValid()) {
    data.latitude = gps.location.lat();
    data.longitude = gps.location.lng();
    data.speed = gps.speed.isValid() ? gps.speed.kmph() : 0;
    data.bearing = gps.course.isValid() ? gps.course.deg() : 0;
    data.altitude = gps.altitude.isValid() ? gps.altitude.meters() : 0;
  }
}
#endif
