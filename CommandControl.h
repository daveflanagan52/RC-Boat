#ifndef __COMMAND_CONTROL_H__
#define __COMMAND_CONTROL_H__

#include <Arduino.h>

#include "Structs.h"

long lastPingTime = 0;
DroneData data;
Location waypoints[NUM_WAYPOINTS];
extern void sendPacket(int recipient, int sender, int type, const uint8_t* packet);
extern bool haltCommanded;

void parseCommand(uint8_t* packet, int packetSize, int rssi, int snr) {
  switch (packet[CC_TYPE]) {
    case COMMAND_PING: {
      lastPingTime = millis();
      sendPacket(LORA_BASESTATION, LORA_ID, COMMAND_PONG, (const uint8_t*)&data);
      break;
    }

    case COMMAND_HALT: {
      haltCommanded = true;
      sendPacket(LORA_BASESTATION, LORA_ID, COMMAND_ACKNOWLEDGE, NULL);
      break;
    }
    
    case COMMAND_GOTO: {
      float latitude, longitude;
      ((uint8_t*)&latitude)[0] = packet[3];
      ((uint8_t*)&latitude)[1] = packet[4];
      ((uint8_t*)&latitude)[2] = packet[5];
      ((uint8_t*)&latitude)[3] = packet[6];
      ((uint8_t*)&longitude)[0] = packet[7];
      ((uint8_t*)&longitude)[1] = packet[8];
      ((uint8_t*)&longitude)[2] = packet[9];
      ((uint8_t*)&longitude)[3] = packet[10];
      waypoints[0] = { latitude, longitude, true };
      waypoints[1] = { 0, 0, false };
      haltCommanded = false;
      sendPacket(LORA_BASESTATION, LORA_ID, COMMAND_ACKNOWLEDGE, NULL);
      break;
    }
      
    case COMMAND_WAYPOINT: {
      // Try to find a spot to place our new waypoint
      float latitude, longitude;
      ((uint8_t*)&latitude)[0] = packet[3];
      ((uint8_t*)&latitude)[1] = packet[4];
      ((uint8_t*)&latitude)[2] = packet[5];
      ((uint8_t*)&latitude)[3] = packet[6];
      ((uint8_t*)&longitude)[0] = packet[7];
      ((uint8_t*)&longitude)[1] = packet[8];
      ((uint8_t*)&longitude)[2] = packet[9];
      ((uint8_t*)&longitude)[3] = packet[10];
      bool added = false;
      for (uint8_t i = 0; i < NUM_WAYPOINTS; i++) {
        if (!waypoints[i].valid) {
          waypoints[i] = { latitude, longitude, true };
          added = true;
          continue;
        }
        if (added) {
          waypoints[i].valid = false;
        }
      }
      if (!added) {
        Serial.println("Command: Failed to add new waypoint to list, no space available");
        sendPacket(LORA_BASESTATION, LORA_ID, COMMAND_FAIL, NULL);
      } else {        
        sendPacket(LORA_BASESTATION, LORA_ID, COMMAND_ACKNOWLEDGE, NULL);
      }
      haltCommanded = false;
      break;
    }
  }
}

#endif
