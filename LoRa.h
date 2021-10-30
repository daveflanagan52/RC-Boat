#ifndef __LORA_H__
#define __LORA_H__

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

long lastSendTime = 0;
extern CCDataPacket frame;
extern void parseCommand(int packet[], int packetSize, int rssi, int snr);

void rebroadcast(int packet[], int packetSize) {
  LoRa.beginPacket();
  for (uint8_t i = 0; i < packetSize; i++)
    LoRa.write(packet[i]);
  LoRa.endPacket();
  LoRa.receive();
}

void onRecieve(int packetSize) {
  if (packetSize == 0)
    return;

  int packet[packetSize] = {};
  for (uint8_t i = 0; i < packetSize; i++)
    packet[i] = LoRa.read();

  // Check if the message is for me
  if (packet[CC_RECIPIENT] != LORA_ID && packet[CC_RECIPIENT] != LORA_BROADCAST) {
    // If not, we can re-broadcast it incase the actual recipent is out of range
    rebroadcast(packet, packetSize);
    return;
  }
  
  parseCommand(packet, packetSize, LoRa.packetRssi(), (int)LoRa.packetSnr());
}

void setupLora() {
  Serial.print("LoRa: Initialising");
  LoRa.setPins(LORA_SS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa: Begin call returned false");
    Serial.println("LoRa: Fail");
    while (true);
  }
  LoRa.setSyncWord(LORA_SYNC);
  LoRa.onReceive(onRecieve);
  LoRa.receive();
  Serial.println("LoRa: Success");
}

void sendData() {
  LoRa.beginPacket();
  LoRa.write(random(0xFF));
  LoRa.write(LORA_BASESTATION);
  LoRa.write(LORA_ID);
  LoRa.write(frame.latitude.binary, 4);
  LoRa.write(frame.longitude.binary, 4);
  LoRa.write(frame.speed.binary, 4);
  LoRa.write(frame.bearing.binary, 4);
  LoRa.endPacket();

  lastSendTime = millis();
  LoRa.receive();
}

#endif
