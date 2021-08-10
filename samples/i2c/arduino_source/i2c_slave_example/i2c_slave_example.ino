/*
 * file: i2c_slave.ino
 * purpose: Slave code that goes with the test_i2c.cpp code.
 *          This code is meant to run on an Arduino.
 */

#include <Wire.h>

void receive_handler(int count);
void request_handler();

void setup()
{
    Wire.begin(4);  // Join the I2C bus with an address of 4
    Wire.onReceive(receive_handler);    // Register the receive handler
    Wire.onRequest(request_handler);    // Register the request handler
    Serial.begin(9600); // Start serial communications for debugging
    Serial.println("I2C test...");
}

void loop()
{
    delay(50);  // We're waiting for events so do nothing here.
}

void receive_handler(int count)
{
    char temp;
    while (Wire.available())    // While there are chars to read
    {
        temp = Wire.read();     // Read a char
        Serial.print(temp);     // Print it to the terminal
    }
    Serial.println();           // Move to the next line
}

void request_handler()
{
    Wire.write("OK");   // Respond with the message expected by the master.
}
