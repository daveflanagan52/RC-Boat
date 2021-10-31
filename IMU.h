#ifndef __IMU_H__
#define __IMU_H__

#include <Arduino.h>
#include <TinyMPU6050.h>

MPU6050 mpu(Wire);
long lastUpdateTime = 0;
extern CCDataPacket frame;

void setupIMU() {
  Serial.print("IMU: Initialising");
  mpu.Initialize();
  Serial.print("IMU: Starting calibration");
  mpu.Calibrate();
  Serial.println("IMU: Success");
}

void parseIMU() {
  if (millis() - lastUpdateTime > IMU_INTERVAL) {
    mpu.Execute();
    frame.pitch.floatingPoint = mpu.GetAngX();
    frame.roll.floatingPoint = mpu.GetAngY();
    frame.yaw.floatingPoint = mpu.GetAngZ();
    lastUpdateTime = millis();
  }
}

#endif
