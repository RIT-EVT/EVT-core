/**
 * Contains functions to make CANopen stack versions of drivers. The
 * functions will take in an EVT-core driver which will be used for
 * making the drivers.
 */
#ifndef _EVT_CANopen_
#define _EVT_CANopen_

#include "CANDevice.hpp"
#include "co_core.h"

#include <EVT/dev/RTC.hpp>
#include <EVT/dev/Timer.hpp>
#include <EVT/io/CAN.hpp>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

//Allows for resizable CANOpen queue if needed
#ifndef CANOPEN_QUEUE_SIZE
    #define CANOPEN_QUEUE_SIZE 150
#endif

namespace EVT::core::IO {

/**
 * Get an instance of the CAN driver that can be used with the CANopen
 * stack. This will populate a struct with function pointers that can
 * handle CAN operations.
 *
 * @param[in] can The CAN interface that will be used for the stack driver
 * @param[in] messageQueue Queue that will be read from for receiveing CAN messages
 * @param[out] canDriver The CANopen stack driver to populate
 */
void getCANopenCANDriver(CAN* can,
                         types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* messageQueue,
                         CO_IF_CAN_DRV* canDriver);

/**
 * Get an instance of the timer driver that can be used with the
 * CANopen stack. This will populate a struct with function pointers
 * that will handler timer operations.
 *
 * @param[in] timer The timer interface to use
 * @param[out] timerDriver The timer driver to populate
 */
void getCANopenTimerDriver(DEV::Timer* timer, CO_IF_TIMER_DRV* timerDriver);

/**
 * Get an instance of the NVM (non-volitile memory) that can be used
 * with the CANopen stack. This will populate a struct with
 * function pointers that can handle CAN operations.
// *
 * CURRENTLY ONLY FOR TESTING USE ONLY, NOT ACTUALLY NVM.
 *
 * @param[out] nvmDriver The NVM driver to populate
 */
void getCANopenNVMDriver(CO_IF_NVM_DRV* nvmDriver);

void initializeCANopenDriver(types::FixedQueue<150, IO::CANMessage>* canOpenQueue, CO_IF_DRV* canStackDriver, CO_IF_NVM_DRV* nvmDriver, CO_IF_TIMER_DRV* timerDriver, CO_IF_CAN_DRV* canDriver, CO_NODE* canNode);

void initializeCANopenData(CANDevice* canDevice, uint8_t NODE_ID, uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE], CO_TMR_MEM appTmrMem[16], CO_NODE* canNode, CO_IF_DRV* canStackDriver);

void processCANopenNode(CO_NODE* canNode);
}// namespace EVT::core::IO

#endif
