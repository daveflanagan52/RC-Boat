#include "Structs.h"
#include "HelperFunctions.h"
#include "Config.h"
#include "LoRa.h"
#include "LiDAR.h"
#include "CommandControl.h"
#include "GPS.h"
#include "Movement.h"
#include "Temperature.h"
#include "IMU.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(LED_PIN, OUTPUT);
  setupGPS();
  setupLora();
  //setupLidar();
  setupMovement();

  data.id = LORA_ID;
}

void loop() {
  parseIMU();
  parseTemperature();
  parseGPS();
  //parseLidar();

  loopMovement();
}
