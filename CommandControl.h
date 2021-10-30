#ifndef __COMMAND_CONTROL_H__
#define __COMMAND_CONTROL_H__

#include <Arduino.h>

#include "Structs.h"

long lastPingTime = 0;
CCDataPacket frame;
CCCommandPacket command;
Location waypoints[NUM_WAYPOINTS];

void parseCommand(int packet[], int packetSize, int rssi, int snr) {
  switch (packet[CC_TYPE]) {
    case COMMAND_PING:
      lastPingTime = millis();
      break;

    case COMMAND_HALT:
      command.sender = packet[CC_SENDER];
      command.type = packet[CC_TYPE];
      command.rssi = rssi;
      command.snr = snr;
      break;

    case COMMAND_GOTO:
    case COMMAND_WAYPOINT:
      command.sender = packet[CC_SENDER];
      command.type = packet[CC_TYPE];
      command.rssi = rssi;
      command.snr = snr;
      
      command.latitude.binary[0] = packet[5];
      command.latitude.binary[1] = packet[6];
      command.latitude.binary[2] = packet[7];
      command.latitude.binary[3] = packet[8];
      
      command.longitude.binary[0] = packet[9];
      command.longitude.binary[1] = packet[10];
      command.longitude.binary[2] = packet[11];
      command.longitude.binary[3] = packet[12];
      
      break;
  }
}

void handleCommand() {
  switch (command.type) {
    case COMMAND_GOTO:
      // Replace all waypoints
      waypoints[0] = { command.latitude.floatingPoint, command.longitude.floatingPoint, true };
      waypoints[1] = { {0.0}, {0.0}, false };
      command.type = COMMAND_NONE;
      break;

    case COMMAND_WAYPOINT:
      // Try to find a spot to place our new waypoint
      bool added = false;
      for (uint8_t i = 0; i < NUM_WAYPOINTS; i++) {
        if (!waypoints[i].valid) {
          waypoints[i] = { command.latitude.floatingPoint, command.longitude.floatingPoint, true };
          added = true;
          continue;
        }
        if (added) {
          waypoints[i].valid = false;
        }
      }
      if (!added) {
        Serial.println("Command: Failed to add new waypoint to list, no space available");
      }
      command.type = COMMAND_NONE;
      break;
  }
}

#endif
