// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

//#define CS_PIN    9  //CAN shield v2
#define CS_PIN    10 //CAN shield v1.4
#define IRQ_PIN   2

bool readyToSend = true;
bool doSend = true;

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

  Serial.println("CAN Receiver");

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

//  while(!readyToSend) {
    delay(5);
//  }
//  if(Serial.available()){
//    Serial.read();
//    doSend = !doSend;
//    Serial.print("doSend=");
//    Serial.println(doSend ? "true" : "False");
//  }

//  if(doSend && readyToSend){
//    Serial.print("Sending CAN message ");
//  
//    CAN.beginExtendedPacket(0xabcdef);
//    CAN.write(0x01);
//    CAN.write(0x02);
//    CAN.write(0x03);
//    CAN.write(0x04);
//    CAN.write(0x05);
//    CAN.endPacket();
//  
//    readyToSend = false;
//  
//    Serial.println("done");
//  }
}
