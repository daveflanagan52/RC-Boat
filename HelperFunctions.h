#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__

#include <Arduino.h>

#ifndef M_PI
#define M_PI    3.1415926535897932384626433832795
#endif

#define EARTH_RADIUS 6371.0

float deg2rad(float deg) {
  return (deg * M_PI / 180.0);
}

float distance(float latitude1, float longitude1, float latitude2, float longitude2) {
  float lat1 = deg2rad(latitude1);
  float lon1 = deg2rad(longitude1);
  float lat2 = deg2rad(latitude2);
  float lon2 = deg2rad(longitude2);
  float d_lat = fabs(lat1 - lat2);
  float d_lon = fabs(lon1 - lon2);
  float a = pow(sin(d_lat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(d_lon / 2), 2);
  float d_sigma = 2 * asin(sqrt(a));
  return EARTH_RADIUS * d_sigma * 100000;
}

float bearing(float latitude1, float longitude1, float latitude2, float longitude2){
  float teta1 = radians(latitude1);
  float teta2 = radians(latitude2);
  float delta1 = radians(latitude2 - latitude1);
  float delta2 = radians(longitude2 - longitude1);
  float y = sin(delta2) * cos(teta2);
  float x = cos(teta1)*sin(teta2) - sin(teta1) * cos(teta2) * cos(delta2);
  float brng = atan2(y,x);
  brng = degrees(brng);
  brng = ( ((int)brng + 360) % 360 ); 
  return brng;
}

#endif
