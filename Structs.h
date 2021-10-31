#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <Arduino.h>

struct DroneTarget {
  uint16_t distance;
  uint8_t quality;
  float bearing;
};

struct DroneData {
  uint8_t id;

  float latitude;
  float longitude;
  float speed;
  float bearing;
  float altitude;
  
  float temperature;
  
  float roll;
  float pitch;
  float yaw;

  uint8_t numTargets;
  DroneTarget* targets;
};

struct LidarData {
  float startAngle;
  float stepAngle;
  float angle[8];
  uint16_t distance[8];
  uint8_t quality[8];
};

struct Location {
  float latitude;
  float longitude;
  bool valid;
};
#endif
