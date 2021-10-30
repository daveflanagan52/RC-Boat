#ifndef __LIDAR_H__
#define __LIDAR_H__

#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial lidarSerial(2);
uint16_t lidarIndex = 0;
uint8_t lidarBuffer[36] = {0};
LidarData lidarData;

void setupLidar() {
  Serial.print("Lidar: Initialising");
  lidarSerial.begin(0, SERIAL_8N1, LIDAR_RXPIN, -1);
  unsigned long detectedBaudRate = lidarSerial.baudRate();
  if (detectedBaudRate) {
    Serial.printf("Lidar: Detected baudrate is %lu\n", detectedBaudRate);
  } else {
    Serial.println("Lidar: No baudrate detected, unable to connect to BoatLidar sensor");
    Serial.println("Lidar: Fail");
    while (true);
  }
  Serial.println("Lidar: Success");
}

void parseLidar() {
  if (lidarSerial && lidarSerial.available()) {
    uint8_t data = lidarSerial.read();
    switch (lidarIndex) {
      default:
        lidarBuffer[lidarIndex++] = data;
        break;

      case 0:
        if (data == LIDAR_HEADER1) {
          lidarIndex++;
        }
        break;

      case 1:
        if (data == LIDAR_HEADER2) {
          lidarIndex++;
        } else {
          lidarIndex = 0;
        }
        break;

      case 31:
        lidarBuffer[lidarIndex] = data;
        lidarIndex = 0;

        lidarData.startAngle = (lidarBuffer[LIDAR_STARTANGLE_H] << 8 | lidarBuffer[LIDAR_STARTANGLE_L]) / LIDAR_FLOAT;
        lidarData.stepAngle = (lidarBuffer[LIDAR_ENDANGLE_H] << 8 | lidarBuffer[LIDAR_ENDANGLE_L]) / LIDAR_FLOAT;
        if (lidarData.stepAngle < lidarData.startAngle)
          lidarData.stepAngle += 360.0;
        lidarData.stepAngle = (lidarData.stepAngle - lidarData.startAngle) / 8.0;

        for (uint8_t i = 0; i < 8; i++) {
          lidarData.angle[i] = lidarData.startAngle + (i * lidarData.stepAngle);
          lidarData.quality[i] = lidarBuffer[LIDAR_DATA_OFFSET + (i * 3) + LIDAR_QUALITY];
          if (lidarData.quality[i] == 0) {
            lidarData.quality[i] = -1;
            lidarData.distance[i] = -1;
          } else {
            uint16_t distanceH = lidarBuffer[LIDAR_DATA_OFFSET + (i * 3) + LIDAR_DISTANCE_H];
            uint16_t distanceL = lidarBuffer[LIDAR_DATA_OFFSET + (i * 3) + LIDAR_DISTANCE_L];
            lidarData.distance[i] = distanceH << 8 | distanceL;
          }
        }
        break;
    }
  }
}

#endif
