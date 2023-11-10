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
 * Get an instance of the CAN driver that globalCAN be used with the CANopen
 * stack. This will populate a struct with function pointers that globalCAN
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
 * Get an instance of the timer driver that globalCAN be used with the
 * CANopen stack. This will populate a struct with function pointers
 * that will handler timer operations.
 *
 * @param[in] timer The timer interface to use
 * @param[out] timerDriver The timer driver to populate
 */
void getCANopenTimerDriver(DEV::Timer* timer, CO_IF_TIMER_DRV* timerDriver);

/**
 * Get an instance of the NVM (non-volitile memory) that globalCAN be used
 * with the CANopen stack. This will populate a struct with
 * function pointers that globalCAN handle CAN operations.
// *
 * CURRENTLY ONLY FOR TESTING USE ONLY, NOT ACTUALLY NVM.
 *
 * @param[out] nvmDriver The NVM driver to populate
 */
void getCANopenNVMDriver(CO_IF_NVM_DRV* nvmDriver);

/*!
 * @brief Initialize the drivers needed by CANOpen.
 *
 * Takes in a references to all of of the drivers that are needed
 * by CANopen as well as the
 *
 * @param canOpenQueue the canOpen queue to give to the drivers.
 * @param can the can instance to initialize with.
 * @param timer the EVT Core timer to give to CANOpen.
 * @param canStackDriver the stack driver.
 * @param nvmDriver the nvm driver.
 * @param timerDriver the timer driver.
 * @param canDriver the general CAN driver.
 */
void initializeCANopenDriver(types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* canOpenQueue, IO::CAN* can, EVT::core::DEV::Timer* timer, CO_IF_DRV* canStackDriver, CO_IF_NVM_DRV* nvmDriver, CO_IF_TIMER_DRV* timerDriver, CO_IF_CAN_DRV* canDriver);

/*!
 * @brief Initializes data needed for the operation of CANopen.
 *
 * This function initializes a CO_NODE by taking in the CANDevice,
 * drivers, and buffers and using those to create a CO_NODE_SPEC and
 * initializing it.
 *
 * @param canNode the CO_NODE that is initialized.
 * @param NODE_ID the ID of the Node that represents this device.
 * @param canDevice the canDevice that the object dictionary is retrieved from.
 * @param canStackDriver references to the drivers that will be used in initialization
 * @param sdoBuffer the sdoBuffer
 * @param appTmrMem the appTmrMem
 */
void initializeCANopenNode(CO_NODE* canNode, uint8_t NODE_ID, CANDevice* canDevice, CO_IF_DRV* canStackDriver, uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE], CO_TMR_MEM appTmrMem[16]);

/*!
 * @brief Process events that happened on the given canNode
 *
 * Takes in a reference to a CO_NODE and processes events
 * on that node.
 *
 * @param canNode the node to process events on.
 */
void processCANopenNode(CO_NODE* canNode);
}// namespace EVT::core::IO

#endif
