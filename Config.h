#ifndef __CONFIG_H__
#define __CONFIG_H__

#define LED_PIN             40

#define IMU_INTERVAL        200

#define TEMPERATURE_PIN     18

// GPS Definitions
#define GPS_RXPIN           44
#define GPS_TXPIN           43

// LiDAR definitions
#define LIDAR_RXPIN         3
#define LIDAR_HEADER1       0x03
#define LIDAR_HEADER2       0x08
#define LIDAR_FLOAT         64.0 - 640.0
#define LIDAR_STARTANGLE_H  5
#define LIDAR_STARTANGLE_L  4
#define LIDAR_ENDANGLE_H    31
#define LIDAR_ENDANGLE_L    30
#define LIDAR_DATA_OFFSET   6
#define LIDAR_DISTANCE_H    1
#define LIDAR_DISTANCE_L    0
#define LIDAR_QUALITY       2

// LoRa definitions
#define LORA_BASESTATION    0x00
#define LORA_BROADCAST      0xFF
#define LORA_ID             0x10 // Needs to be unique and between 0x01 & 0xFE
#define LORA_SS             34
#define LORA_RST            38
#define LORA_IRQ            39
#define LORA_FREQ           866E6
#define LORA_SYNC           0xB0

// Command definitions
#define MIN_DISTANCE        50
#define TARGET_SPEED        2
#define NUM_WAYPOINTS       50
#define COMMAND_FAIL        -1
#define COMMAND_NONE        0
#define COMMAND_ACKNOWLEDGE 1
#define COMMAND_PING        2
#define COMMAND_PONG        3
#define COMMAND_HALT        4
#define COMMAND_GOTO        5
#define COMMAND_WAYPOINT    6

// Movement PID tuning variables
#define BEARING_P           2
#define BEARING_I           5
#define BEARING_D           1
#define SPEED_P             2
#define SPEED_I             5
#define SPEED_D             2

// Command Control variables
#define CC_RECIPIENT        0
#define CC_SENDER           1
#define CC_TYPE             2

#endif
