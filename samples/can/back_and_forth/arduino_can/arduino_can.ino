// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

bool readyToSend = true;
int counter = 0;

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received ");

  if (CAN.packetExtended()) {
    Serial.print("extended ");
  }

  if (CAN.packetRtr()) {
    // Remote transmission request, packet contains no data
    Serial.print("RTR ");
  }

  Serial.print("packet with id 0x");
  Serial.print(CAN.packetId(), HEX);

  if (CAN.packetRtr()) {
    Serial.print(" and requested length ");
    Serial.println(CAN.packetDlc());
  } else {
    Serial.print(" and length ");
    Serial.println(packetSize);

    // only print packet data for non-RTR packets
    while (CAN.available()) {
      Serial.print(" ");
      Serial.print(CAN.read(), HEX);
    }
    Serial.println();
  }

  Serial.println();

  readyToSend = true;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Sender");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // register the receive callback
  CAN.onReceive(onReceive);
}

void loop() {


  // Serial.print("Sending CAN message ");

  CAN.beginPacket(0x601);
  
  // Command byte
  CAN.write(0x2F);

  // Index access
  CAN.write(0x00);
  CAN.write(0x21);

  // Subindex
  CAN.write(0x00);

  // Data
  CAN.write(counter);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x05);
  
  CAN.endPacket();

  readyToSend = false;

  counter++;
  if(counter > 100)
    counter = 0;

  delay(500);
  // Serial.println("done");
}
