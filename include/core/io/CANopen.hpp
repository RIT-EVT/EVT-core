/**
 * Contains functions to make CANopen stack versions of drivers. The
 * functions will take in an EVT-core driver which will be used for
 * making the drivers.
 */
#ifndef _EVT_CANopen_
#define _EVT_CANopen_

#include "CANDevice.hpp"
#include "co_core.h"

#include <core/dev/RTC.hpp>
#include <core/dev/Timer.hpp>
#include <core/io/CAN.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/utils/types/FixedQueue.hpp>

// Allows for resizable CANOpen queue if needed
#ifndef CANOPEN_QUEUE_SIZE
    #define CANOPEN_QUEUE_SIZE 150
#endif

namespace core::io {

/**
 * Get an instance of the CAN driver that can be used with the CANopen
 * stack. This will populate a struct with function pointers that can
 * handle CAN operations.
 *
 * @param[in] can The CAN interface that will be used for the stack driver
 * @param[in] messageQueue Queue that will be read from for receiveing CAN messages
 * @param[out] canDriver The CANopen stack driver to populate
 */
void getCANopenCANDriver(CAN* can, types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>* messageQueue,
                         CO_IF_CAN_DRV* canDriver);

/**
 * Get an instance of the timer driver that can be used with the
 * CANopen stack. This will populate a struct with function pointers
 * that will handler timer operations.
 *
 * @param[in] timer The timer interface to use
 * @param[out] timerDriver The timer driver to populate
 */
void getCANopenTimerDriver(dev::Timer* timer, CO_IF_TIMER_DRV* timerDriver);

/**
 * Get an instance of the NVM (non-volitile memory) that can be used
 * with the CANopen stack. This will populate a struct with
 * function pointers that can handle CAN operations.
 *
 * CURRENTLY ONLY FOR TESTING USE ONLY, NOT ACTUALLY NVM.
 *
 * @param[out] nvmDriver The NVM driver to populate
 */
void getCANopenNVMDriver(CO_IF_NVM_DRV* nvmDriver);

/**
 * Takes in a references to objects that will be populated
 * with all of the needed CAN drivers. Once these drivers
 * are populated, they are passed to the overall canDriver.
 *
 * @param canOpenQueue[in,out] the canOpen queue to give to the drivers.
 * @param can[in,out] the can instance to initialize with.
 * @param timer[in] the EVT Core timer to give to CANOpen.
 * @param canStackDriver[in,out] the stack driver.
 * @param nvmDriver[in,out] the nvm driver.
 * @param timerDriver[in,out] the timer driver.
 * @param canDriver[in,out] the general CAN driver.
 */
void initializeCANopenDriver(types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>* canOpenQueue, io::CAN* can,
                             core::dev::Timer* timer, CO_IF_DRV* canStackDriver, CO_IF_NVM_DRV* nvmDriver,
                             CO_IF_TIMER_DRV* timerDriver, CO_IF_CAN_DRV* canDriver);

/**
 * This function initializes a CO_NODE by taking in the CANDevice,
 * drivers, and buffers and using those to create a CO_NODE_SPEC and
 * initializing it.
 *
 * @param canNode[in,out] the CO_NODE that is initialized.
 * @param canDevice[in] the canDevice that the object dictionary is Getd from.
 * @param canStackDriver[in] references to the drivers that will be used in initialization
 * @param sdoBuffer[in] the sdoBuffer
 * @param appTmrMem[in] the appTmrMem
 */
void initializeCANopenNode(CO_NODE* canNode, CANDevice* canDevice, CO_IF_DRV* canStackDriver,
                           uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE], CO_TMR_MEM appTmrMem[16]);

/**
 * Takes in a reference to a CO_NODE and processes events
 * on that node.
 *
 * @param canNode[in,out] the node to process events on.
 */
void processCANopenNode(CO_NODE* canNode);

/**
 * This function sets up and starts an SDO download (write) request to transfer data
 * to the specified object dictionary entry on the target CANopen node.
 *
 * @param node[in]        Reference to the CANopen node object
 * @param data[in]        Pointer to the data buffer that holds the data to send
 * @param size[in]        Size of the data to transfer in bytes
 * @param entry[in]       Object dictionary entry (index + subindex) to write to
 * @return CO_ERR[out]    Returns the result of the transfer operation
 */
CO_ERR SDOTransfer(CO_NODE& node, uint8_t* data, uint8_t size, uint32_t entry);

/**
 * This function starts an SDO upload (read) request to fetch data from the specified
 * object dictionary entry on the target CANopen node
 *
 * @param node[in]        Reference to the CANopen node object
 * @param data[in]        Pointer to the buffer where received data will be stored
 * @param size[in]        Size of the buffer provided to receive data
 * @param entry[in]       Object dictionary entry (index + subindex) to read from
 * @return CO_ERR[out]    Returns the result of the receive operation
 */
CO_ERR SDOReceive(CO_NODE& node, uint8_t* data, uint8_t size, uint32_t entry);

/**
 * This function assigns the user-provided callback function and AppContext to be
 * used when an SDO operation completes
 *
 * @param AppCallback[in] Pointer to the callback function to register
 * @param AppContext[in] Context to be passed to the callback
 */
void registerCallBack(void (*AppCallback)(CO_CSDO* csdo, uint16_t index, uint8_t sub, uint32_t code), void* AppContext);
} // namespace core::io

#endif
