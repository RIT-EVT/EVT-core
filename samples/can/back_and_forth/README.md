# CAN Back and Forth

This sample is intended for use between two CAN devices. The two devices need
to be connected on a CAN network (with transcievers). Two devices with this
code on board can communicate with each other. Also included is an Arduino
sample code that should work with most CAN shields. The Arduino code operates
similar to the EVT-core code spitting out CAN messages and waiting for
a CAN message to come in before writing another message.
