#include "Structs.h"
#include "HelperFunctions.h"
#include "Config.h"
#include "LoRa.h"
#include "LiDAR.h"
#include "CommandControl.h"
#include "GPS.h"
#include "Movement.h"
#include "Temperature.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);

  setupGPS();
  setupLora();
  //setupLidar();
  setupMovement();

  frame.id = LORA_ID;
}

void loop() {
  parseTemperature();
  parseGPS();
  //parseLidar();

  handleCommand();
  drive();

  // If we haven't sent data for the defined time, lets do that
  if (millis() - lastSendTime > LORA_INTERVAL) {
    sendData();
  }

  // If we haven't heard from the base station in a while
  if (millis() - lastPingTime > 5000) {

  }
}