#ifndef __LORA_H__
#define __LORA_H__

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

extern DroneData data;
extern void parseCommand(uint8_t* packet, int packetSize, int rssi, int snr);

void onRecieve(int packetSize) {
  if (packetSize < 3) // We expect at least a sender, recipent and command
    return;

  int recipient = LoRa.peek();

  // Check if the message is for me
  if (recipient != LORA_ID && recipient != LORA_BROADCAST) {
    return;
  }

  uint8_t packet[packetSize];
  LoRa.readBytes((char*)&packet, packetSize);  
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

void sendPacket(int recipient, int sender, int type, const uint8_t* packet) {
  LoRa.beginPacket();
  LoRa.write(recipient);
  LoRa.write(sender);
  LoRa.write(type);
  LoRa.write(packet, sizeof(packet));
  LoRa.endPacket();
  LoRa.receive();
}

#endif
