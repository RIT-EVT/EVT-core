=======
CANopen
=======

Introduction
============

CANopen is a CAN based communication protcol. CANopen is built completely off
of the CAN specification to provide a higher level abstraction for
communicating on the CAN network. CANopen is used commonly in automation
applications and is the network protocol of choice for EVT. This document
will briefly go over the fundamentals of CANopen and how to use CANopen
within EVT-core.

What is CANopen
===============

As mentioned, CANopen is built ontop of CAN. Specifically CANopen uses standard
CAN frames for sending data. CANopen provides a higher level approach to
communicating with various entities on a network. For the purposes of working
with CANopen in EVT-core three main concepts will be covered. Object
dictionaries, Service Data Objects (SDOs), and Process Data Objects (PDOs).
For a more complete overview, refer to `CSS Electronics CANopen Overview <https://www.csselectronics.com/pages/canopen-tutorial-simple-intro>`_.

Object Dictionary
-----------------

CANopen is structured as a series of "nodes" that are connected on a network.
You can think of a node as a single device which has a specific purpose. For
example, in the motorcycle system, one node could be the Temperature Management
System, another could be the Battery Management System. Each node is an
independent system that takes part in the communication on the CANopen network.
Each node conforms to the CANopen standard and understands how to expose
certain information on the CANopen network.

The main representation of a node is through something called the "Object
Dictionary". The Object Dictionary is a effective mapping between indicies
(and sub-indicies) and values. The values in the dictionary can be anything
from CANopen required identifiers, to device specific settings, to data which
can then be exposed on the CANopen network.

For example, take a Temperature Management System (TMS). In its Object Dictionary
would be a series of identifiers required by CANopen (specifically a vendor ID,
product code, revision number, and serial number), potentially a few control
loop setting such as "max pump speed", "minimum threshold temperature" or data
for other entities on the CANopen network to gain information on the state of 
the Temperature Management System such as "current pump speed", "current motor 
controller temperature", etc. An example CANopen Object Dictionary is shown 
in the sample below.

Most of CANopen is built around the idea that the entities on a network work
by reading from and manipulating other Object Dictionaries. For example,
another node on the network may read temperature data from the TMS in order
to determine if the temperature of the system is at a safety critical level.

Service Data Objects
--------------------

Service Data Objects (SDO) are one way for nodes on a network to interact
with other Object Dictionaries. SDO's can be used to either read data from
another node's Object Dictionary or write data into another node's Object
Dictionary. For example, lets say Node A has a setting which controls flow
rate. Node B wants to update the flow rate setting of Node A. Node B can
execute an SDO (which is really just a sequence of CAN messages) to
update the value in Node A's Object Dictionary.

SDO's are commonly used for infrequent data access and modification on a
CANopen network. They boil down to a series of CAN messages which follow
the CANopen specification for SDO messages.

Process Data Objects
--------------------

Process Data Objects (PDO) are used mostly for broadcasting of data. For
example, lets say that Node A has temperature data which may be useful for
other nodes on the CANopen network. Node A can send out that temperature data
at some rate via PDO's. PDO's require less overhead then SDO's and as such
are better for sharing of real-time data.

How Does CANopen Work in EVT-core?
==================================

CANopen is enabled on EVT-core using a library called `CANopen Stack <https://canopen-stack.org/v4.2/>`_.
CANopen stack provides a hardware independent means for allowing a device to
become a node on a CANopen network. As such CANopen needs to be provided with
a few pieces of information. First, it needs the Object Dictionary of the node,
this way the library knows how to process SDO's and PDO's. Second, CANopen
stack needs to be provided with "drivers" to interact with the physical world.
More specifically, users provide a CAN driver, timer driver, and non-volatile
memory driver. EVT-core provides means for passing this information to
CANopen stack.

Looking at an Example
---------------------
The remainder of this document will refer to the sample found `here <https://github.com/RIT-EVT/EVT-core/tree/main/samples/canopen>`_.
What this sample does is create a node which exposes a single integer over
CANopen which can be read/written to via SDO's. The sample has two main
parts. First is the C++ class which represents the node as a whole including
the object dictionary. Second is the main file which setups CANopen and
processes incoming data.

Lets first look at the Object Dictionary contained in ``RPDOCanNode.hpp``.

.. code-block:: C++

   CO_OBJ_T objectDictionary[OBJECT_DIRECTIONARY_SIZE + 1] = {
        // Sync ID, defaults to 0x80
        {CO_KEY(0x1005, 0, CO_UNSIGNED32 | CO_OBJ_D__R_), 0, (uintptr_t) 0x80},

        // Information about the hardware, hard coded sample values for now
        // 1: Vendor ID
        // 2: Product Code
        // 3: Revision Number
        // 4: Serial Number
        {
            .Key = CO_KEY(0x1018, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x10,
        },
        {
            .Key = CO_KEY(0x1018, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x11,
        },
        {
            .Key = CO_KEY(0x1018, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x12,
        },
        {
            .Key = CO_KEY(0x1018, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x13,
        },

        // SDO CAN message IDS.
        // 1: Client -> Server ID, default is 0x600 + NODE_ID
        // 2: Server -> Client ID, default is 0x580 + NODE_ID
        {
            .Key = CO_KEY(0x1200, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x600 + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1200, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x580 + NODE_ID,
        },

        // TPDO0 settings
        // 0: The TPDO number, default 0
        // 1: The COB-ID used by TPDO0, provided as a function of the TPDO number
        // 2: How the TPO is triggered, default to manual triggering
        // 3: Inhibit time, defaults to 0
        // 5: Timer trigger time in 1ms units, 0 will disable the timer based triggering
        {
            .Key = CO_KEY(0x1800, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0,
        },
        {
            .Key = CO_KEY(0x1800, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(0),
        },
        {
            .Key = CO_KEY(0x1800, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0xFE,
        },
        {
            .Key = CO_KEY(0x1800, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0,
        },
        {
            .Key = CO_KEY(0x1800, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 2000,
        },

        // TPDO0 mapping, determins the PDO messages to send when TPDO1 is triggered
        // 0: The number of PDO message associated with the TPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            .Key = CO_KEY(0x1A00, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 1,
        },
        {
            .Key = CO_KEY(0x1A00, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = CO_LINK(0x2100, 0, 8),// Link to sample data position in dictionary
        },

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depeneding on configuration PDO
        {
            .Key = CO_KEY(0x2100, 0, CO_UNSIGNED8 | CO_OBJ___PRW),
            .Type = 0,
            .Data = (uintptr_t) &sampleData,
        },

        // End of dictionary marker
        CO_OBJ_DIR_ENDMARK};


Entries
^^^^^^^

Each item in the array represents a single entry in the Object Dictionary.
Majority of the entries are required based on CANopen specification. For
example, the information about the hardware is required to exist, but
for our usage, is mostly arbitrary. For the most part, this Object Dictionary
can be copied and used in other Nodes with only minor tweaking.

Lets look at a single entry in the CANopen Object Dictionary.

.. code-block:: cpp

   {
       .Key = CO_KEY(0x2100, 0, CO_UNSIGNED8 | CO_OBJ___PRW),
       .Type = 0,
       .Data = (uintptr_t) &sampleData,
   },


The ``Key`` is how the data is accessed in the Object Dictionary. The first
value in the key is the index, so in this case, this entry is at index
``0x2100`` in the Object Dictionary. The second value is the subindex, this
is the second piece for how data is accessed in an Object Dictionary. So
if an entity wants to access this piece of data, they need to make an SDO
for index ``0x2100`` subindex ``0x0`` of this Object Dictionary. The final
piece of the key is information about the data. The first value represents
the size of the data, in this case we let CANopen stack know that the data
stored is a unsigned 8 bit value. The second part represents the access
settings. These define how people can interact with this data. ``P`` means
that this piece of data is accessable over PDO's. The ``R`` means that entities
can read this data via SDO's. The ``W`` mean that entities can modify this
value through an SDO. If one of those operations should not be allowed, that
letter can be substituted out for an underscore.

Below is a different entry.

.. code-block:: cpp

   {
       .Key = CO_KEY(0x1018, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
       .Type = 0,
       .Data = (uintptr_t) 0x13,
   },


From what we know, this data is accessible at index ``0x1018`` subindex ``0x4``,
the data itself if an unsigned 32 bit number, and users are only allowed to
read this data over SDO's, but cannot write in a new data. The new piece of
information is the ``D``. In this case ``D`` means "direct" which indicates
that the value in questions is directly stored in the Object Dictionary. So
in the example above, the value ``0x13`` is directly stored in the Object
Dictionary. In the first example, the ``D`` is not present because we instead
provide the address of the value instead of the value itself.

TPDOs
^^^^^

Now that we can reason through an entry, lets look at what this Object
Dictionary is doing for us. The main functionality presented is the idea of a
TPDO. A TPDO is just a PDO that is sent out at a specific interval. In this
case, the Object Dictionary is setup to transmit the sample data very 2
seconds. Below is the TPDO settings expressed in the Object Dictionary.

.. code-block:: cpp

   // TPDO0 settings
   // 0: The TPDO number, default 0
   // 1: The COB-ID used by TPDO0, provided as a function of the TPDO number
   // 2: How the TPO is triggered, default to manual triggering
   // 3: Inhibit time, defaults to 0
   // 5: Timer trigger time in 1ms units, 0 will disable the timer based triggering
   {
       .Key = CO_KEY(0x1800, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
       .Type = 0,
       .Data = (uintptr_t) 0,
   },
   {
       .Key = CO_KEY(0x1800, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
       .Type = 0,
       .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(0),
   },
   {
       .Key = CO_KEY(0x1800, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
       .Type = 0,
       .Data = (uintptr_t) 0xFE,
   },
   {
       .Key = CO_KEY(0x1800, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
       .Type = 0,
       .Data = (uintptr_t) 0,
   },
   {
       .Key = CO_KEY(0x1800, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
       .Type = CO_TEVENT,
       .Data = (uintptr_t) 2000,
   },


In this case the value of interest is the last entry. That last entry is
what specifices when the TPDO is triggered in milliseconds. In this case
2000 milliseconds or 2 seconds. The next piece of information links the TPDO
settings to the information that should be transmitted.

.. code-block:: cpp

   // TPDO0 mapping, determins the PDO messages to send when TPDO1 is triggered
   // 0: The number of PDO message associated with the TPDO
   // 1: Link to the first PDO message
   // n: Link to the nth PDO message
   {
       .Key = CO_KEY(0x1A00, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
       .Type = 0,
       .Data = (uintptr_t) 1,
   },
   {
       .Key = CO_KEY(0x1A00, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
       .Type = 0,
       .Data = CO_LINK(0x2100, 0, 8),// Link to sample data position in dictionary
   },

In the above example, the first entry states we will only have 1 value to
transmit as part of this PDO. The second entry points out where in the Object
Dictionary is the value itself to be transmitted. In this case, we say that
for this TPDO we want to transmit the data stored at index ``0x2100`` and
subindex ``0x0`` which is 8 bits in size. Later in the Object Dictionary, the
entry below is included.

.. code-block:: cpp

   // User defined data, this will be where we put elements that can be
   // accessed via SDO and depeneding on configuration PDO
   {
       .Key = CO_KEY(0x2100, 0, CO_UNSIGNED8 | CO_OBJ___PRW),
       .Type = 0,
       .Data = (uintptr_t) &sampleData,
   },


As you can see, the key matched the link value in the previous entry. That is
how the TPDO know what data to actually transmit.

Main
^^^^

Below is the whole code for ``main.cpp``. While it looks like a lot, the good
news is that all of it is essentially boiler plate code.

.. code-block:: cpp

   /**
    * This sample shows off CANopen support from EVT-core. This will
    * setup a CANopen node and attempt to make back and forth communication.
    */
   #include <stdint.h>

   #include <EVT/dev/platform/f3xx/f302x8/Timerf302x8.hpp>
   #include <EVT/io/ADC.hpp>
   #include <EVT/io/CAN.hpp>
   #include <EVT/io/UART.hpp>
   #include <EVT/io/manager.hpp>
   #include <EVT/io/types/CANMessage.hpp>
   #include <EVT/utils/time.hpp>
   #include <EVT/utils/types/FixedQueue.hpp>

   #include <EVT/io/CANopen.hpp>

   #include <Canopen/co_core.h>
   #include <Canopen/co_if.h>
   #include <Canopen/co_tmr.h>

   #include "RPDOCanNode.hpp"

   namespace IO = EVT::core::IO;
   namespace DEV = EVT::core::DEV;
   namespace time = EVT::core::time;

   ///////////////////////////////////////////////////////////////////////////////
   // EVT-core CAN callback and CAN setup. This will include logic to set
   // aside CANopen messages into a specific queue
   ///////////////////////////////////////////////////////////////////////////////

   /**
    * Interrupt handler to get CAN messages. A function pointer to this function
    * will be passed to the EVT-core CAN interface which will in turn call this
    * function each time a new CAN message comes in.
    *
    * NOTE: For this sample, every non-extended (so 11 bit CAN IDs) will be
    * assummed to be intended to be passed as a CANopen message.
    *
    * @param message[in] The passed in CAN message that was read.
    */
   void canInterrupt(IO::CANMessage& message, void* priv) {
       EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* queue =
           (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;
       if (queue != nullptr)
           queue->append(message);
   }

   ///////////////////////////////////////////////////////////////////////////////
   // CANopen specific Callbacks. Need to be defined in some location
   ///////////////////////////////////////////////////////////////////////////////
   extern "C" void CONodeFatalError(void) {}

   extern "C" void COIfCanReceive(CO_IF_FRM* frm) {}

   extern "C" int16_t COLssStore(uint32_t baudrate, uint8_t nodeId) { return 0; }

   extern "C" int16_t COLssLoad(uint32_t* baudrate, uint8_t* nodeId) { return 0; }

   extern "C" void CONmtModeChange(CO_NMT* nmt, CO_MODE mode) {}

   extern "C" void CONmtHbConsEvent(CO_NMT* nmt, uint8_t nodeId) {}

   extern "C" void CONmtHbConsChange(CO_NMT* nmt, uint8_t nodeId, CO_MODE mode) {}

   extern "C" int16_t COParaDefault(CO_PARA* pg) { return 0; }

   extern "C" void COPdoTransmit(CO_IF_FRM* frm) {}

   extern "C" int16_t COPdoReceive(CO_IF_FRM* frm) { return 0; }

   extern "C" void COPdoSyncUpdate(CO_RPDO* pdo) {}

   extern "C" void COTmrLock(void) {}

   extern "C" void COTmrUnlock(void) {}

   int main() {
       // Initialize system
       IO::init();

       // Will store CANopen messages that will be populated by the EVT-core CAN
       // interrupt
       EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

       // Intialize CAN, add an IRQ which will add messages to the queue above
       IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
       can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

       // Initialize the timer
       DEV::Timerf302x8 timer(TIM2, 100);

       // UART for testing
       IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
       timer.stopTimer();

       RPDOCanNode testCanNode;

       // Reserved memory for CANopen stack usage
       uint8_t sdoBuffer[1][CO_SDO_BUF_BYTE];
       CO_TMR_MEM appTmrMem[4];

       // Attempt to join the CAN network
       IO::CAN::CANStatus result = can.connect();

       if (result != IO::CAN::CANStatus::OK) {
           uart.printf("Failed to connect to CAN network\r\n");
           return 1;
       }

       ///////////////////////////////////////////////////////////////////////////
       // Setup CAN configuration, this handles making drivers, applying settings.
       // And generally creating the CANopen stack node which is the interface
       // between the application (the code we write) and the physical CAN network
       ///////////////////////////////////////////////////////////////////////////
       // Make drivers
       CO_IF_DRV canStackDriver;

       CO_IF_CAN_DRV canDriver;
       CO_IF_TIMER_DRV timerDriver;
       CO_IF_NVM_DRV nvmDriver;

       IO::getCANopenCANDriver(&can, &canOpenQueue, &canDriver);
       IO::getCANopenTimerDriver(&timer, &timerDriver);
       IO::getCANopenNVMDriver(&nvmDriver);

       canStackDriver.Can = &canDriver;
       canStackDriver.Timer = &timerDriver;
       canStackDriver.Nvm = &nvmDriver;

       CO_NODE_SPEC canSpec = {
           .NodeId = 0x01,
           .Baudrate = IO::CAN::DEFAULT_BAUD,
           .Dict = testCanNode.getObjectDictionary(),
           .DictLen = testCanNode.getNumElements(),
           .EmcyCode = NULL,
           .TmrMem = appTmrMem,
           .TmrNum = 16,
           .TmrFreq = 100,
           .Drv = &canStackDriver,
           .SdoBuf = reinterpret_cast<uint8_t*>(&sdoBuffer[0]),
       };

       CO_NODE canNode;

       CONodeInit(&canNode, &canSpec);
       CONodeStart(&canNode);
       CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

       time::wait(500);

       uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));

       while (1) {
           uart.printf("Value of my number: %d\n\r", testCanNode.getSampleData());
           // Process incoming CAN messages
           CONodeProcess(&canNode);
           // Update the state of timer based events
           COTmrService(&canNode.Tmr);
           // Handle executing timer events that have elapsed
           COTmrProcess(&canNode.Tmr);
           // Wait for new data to come in
           time::wait(10);
       }
   }


At the top is a function which handles the incoming CAN messages, this function
simple adds all CAN messages to a queue for CANopen stack to read through.

Next are a series of functions marked as ``extern`` these functions have to exist
for CANopen stack and are callbacks that may be run by CANopen stack when certain
events take place. These are simply stubbed out since they are not used in this
case.

Inside the main function, first the various EVT-core components are
initialized. In this case the EVT-core CAN driver, timer, and UART. Then
the CANopen stack drivers are created from the EVT-core components.

After the drivers are created, a ``CO_NODE_SPEC`` instance is made that
contains the settings for CANopen stack. This includes the drivers,
CANopen Object Dictionary, and other settings.

The CANopen stack instance is then initialized and started up, at this point
CANopen stack is able to handle and respond to incoming CAN messages.

The final while loop prints the value of the data that is exposed by the
Object Dictionary and calls a few CANopen stack functions which update the
internal state of the CANopen stack logic.

As mentioned, the vast majority of this code is boiler plate and can be
copied, pasted, and modified for specific applications. Most notably, the
IO pins may need to be changed, the Object Dictionary, and additional logic
is probably needed in the main loop for whatever application the code is
being used for.
