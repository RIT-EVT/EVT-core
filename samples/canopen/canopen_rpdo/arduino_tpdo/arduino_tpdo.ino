// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

//#define CS_PIN    9  //CAN shield v2
#define CS_PIN    10 //CAN shield v1.4
#define IRQ_PIN   2

uint8_t counter1 = 0;
uint16_t counter2 = 0;
long timeSinceLastReceive = 0;
long lastSend = 0;

void onReceive(int packetSize) {
  if(CAN.packetId() != 0x180)
    return;

  long timeDelta = millis() - timeSinceLastReceive;
  timeSinceLastReceive = millis();
  
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
    Serial.print(packetSize);

    Serial.print(" and time delta ");
    Serial.println(timeDelta);

    // only print packet data for non-RTR packets
    while (CAN.available()) {
      Serial.print(" ");
      Serial.print(CAN.read(), HEX);
    }
    Serial.println();
  }

  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Sender");

  CAN.setPins(CS_PIN, IRQ_PIN);
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // register the receive callback
  CAN.onReceive(onReceive);
}

void loop() {
  //Serial.print("Sending CAN message ");

  if(lastSend + 1000 < millis()){
    lastSend = millis();

    Serial.print("Sending CAN message: ");
    Serial.print(counter1, HEX);
    Serial.print(" ");
    Serial.println(counter2, HEX);

    //manually create TPDO message
    CAN.beginPacket(0x181);
    CAN.write(counter1);
    CAN.write((uint8_t)(counter2 & 0x00FF));
    CAN.write((uint8_t)(counter2 >> 8));
    CAN.endPacket();
  }

  counter1++;
  if(counter1 % 10 == 0){
    counter2 = (counter2+1)*3;
  }

  delay(10);
}
