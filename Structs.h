#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <Arduino.h>

typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;

struct CCTarget {
  uint16_t distance;
  uint8_t quality;
  float bearing;
};

struct CCDataPacket {
  uint8_t id;

  binaryFloat latitude;
  binaryFloat longitude;
  binaryFloat speed;
  binaryFloat bearing;
  binaryFloat altitude;
  
  binaryFloat temperature;
  
  binaryFloat roll;
  binaryFloat pitch;
  binaryFloat yaw;

  uint8_t numTargets;
  CCTarget* targets;
};

struct CCCommandPacket {
  uint8_t sender;
  uint16_t type;
  int rssi;
  int snr;
  binaryFloat latitude;
  binaryFloat longitude;
};

struct LidarData {
  float startAngle;
  float stepAngle;
  float angle[8];
  uint16_t distance[8];
  uint8_t quality[8];
};

struct Location {
  binaryFloat latitude;
  binaryFloat longitude;
  bool valid;

  Location() {
    latitude.floatingPoint = 0;
    longitude.floatingPoint = 0;
    valid = false;
  }

  Location(float _latitude, float _longitude, bool _valid) {
    latitude.floatingPoint = _latitude;
    longitude.floatingPoint = _longitude;
    valid = _valid;
  }
};
#endif
