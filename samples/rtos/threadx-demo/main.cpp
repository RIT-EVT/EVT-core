/**
 * This sample shows off Threadx support from Evt-core.
 * This will make 4 threads and send data between them.
 */

#include <core/dev/LED.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>

///rtos includes

#include <core/rtos/Semaphore.hpp>
#include <core/rtos/Enums.hpp>
#include <core/rtos/EventFlags.hpp>
#include <core/rtos/BytePool.hpp>
#include <core/rtos/Queue.hpp>
#include <core/rtos/Thread.hpp>
#include <core/rtos/UARTTX.hpp>

///Namespaces
namespace IO = core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;
namespace log = EVT::core::log;
namespace rtos = core::rtos;

///Defines
#define DEMO_STACK_SIZE 1024
#define DEMO_QUEUE_SIZE 100
#define TX_APP_MEM_POOL_SIZE 65536

/**
 * Struct definition that holds all the counters for each thread.
 */
typedef struct counters {
    uint32_t global_count; //Times a random number has been sent and received
    uint32_t global_sum;  //Sum of random numbers
    uint32_t thread0_count;
    uint32_t thread0_sum;
    uint32_t thread1_count;
    uint32_t thread1_sum;
    uint32_t thread2_count;
    uint32_t thread2_sum;
} counters_t;

typedef struct thread_0_args {
    rtos::Queue* queue;
    rtos::Semaphore* semaphore;
    rtos::wrapper::UARTTX* uarttx;
    counters_t* counters;
} controller_thread_args_t;

typedef struct other_thread_args {
    rtos::Queue* queue;
    rtos::Semaphore* semaphore;
    rtos::wrapper::UARTTX* uarttx;
    rtos::EventFlags* eventFlags;
    uint8_t num;
    counters_t* counters;
} other_thread_args_t;

///Function Prototypes
void controllerThreadEntry(controller_thread_args_t* args);
void otherThreadEntry(other_thread_args_t* args);
void semaphoreThreadEntry(other_thread_args_t* args);


int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    //create all the initializable objects
    rtos::wrapper::UARTTX uarttx(uart);
    rtos::Queue q1((char*)"queue", 16, 8);
    rtos::BytePool<TX_APP_MEM_POOL_SIZE> txPool((char*)"txBytePool");
    rtos::Semaphore semaphore((char*)"Semaphore 1", 1);
    rtos::EventFlags eventFlags((char*)"Event Flags");

    //create the counters (they all start at 0)
    counters_t counters = {
        0,0,0,0,0,0,0,0
    };

    //create the thread 0 argument struct
    controller_thread_args_t controllerThreadArgs = {&q1, &semaphore, &uarttx, &counters};

    //create thread 0
    rtos::Thread<controller_thread_args_t*> controllerThread((char*)"Controller Thread", controllerThreadEntry, &controllerThreadArgs,
                                           DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);

    //create the structs that holds the other thread arguments
    other_thread_args_t thread_1_args = {&q1, &semaphore, &uarttx, &eventFlags, 1, &counters};

    other_thread_args_t thread_2_args = {&q1, &semaphore, &uarttx, &eventFlags, 2, &counters};

    other_thread_args_t thread_3_args = {&q1, &semaphore, &uarttx, &eventFlags, 3, &counters};

    //create thread1
    rtos::Thread<other_thread_args_t *> thread1((char*)"Thread 1", otherThreadEntry, &thread_1_args,
                                               DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);
    //create thread2
    rtos::Thread<other_thread_args_t *> thread2((char*)"Thread 2", otherThreadEntry, &thread_2_args,
                                               DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);
    //create thread3
    rtos::Thread<other_thread_args_t *> semaphoreThread((char*)"Thread 3", semaphoreThreadEntry, &thread_3_args,
                                               DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);

    uart.printf("About to start the kernel.\n\r");


    //create the initializable array- this must include all the initializable objects
    rtos::Initializable *arr[] = {
        &controllerThread, &uarttx,&q1,&semaphore, &eventFlags, &thread1, &thread2, &semaphoreThread
    };

    //start the kernel (the kernel takes in the array of initializables and initializes them when the threadx kernel starts)
    rtos::startKernel(arr, 8, txPool);

    //the startKernel method doesn't actually return so this will never happen.
    return 0;
}

///Function definitions
void controllerThreadEntry(controller_thread_args_t* args) {
    srand(77);
    rtos::TXError queue_status;
    uint32_t num;

    //IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    //uart.printf("test\n\r");
    args->uarttx->printf("\n\rThread 0 Created\n\r");


    //this is to test that the uarttx handles long messages correctly
    args->uarttx->printf("\n\rThis is a very long message wow it is so long that's so crazy how long this is wowee\n\r");
    while (1) {
        num = rand() % 25 + 1;

        /* Send message to queue 0. */
        queue_status = args->queue->send(&num, rtos::TXWait::WaitForever);
        if (queue_status != rtos::TXError::Success) {
            args->uarttx->printf("Error on Thread 0 send to queue: %u", queue_status);
        }

        /* Take semaphore when it is released. */
        args->semaphore->get(rtos::WaitForever);

        args->uarttx->printf("\n\rThread 0: %lu\n\r", num);

        if (queue_status == rtos::TXError::Success) {
            args->counters->thread0_count++;
            args->counters->thread0_sum += num;
        }

        if (args->counters->thread0_count % 10 == 0) {
            args->uarttx->printf("Global count: %lu\r\n"
                        "Global sum: %lu\r\n"
                        "Global average: %lu\r\n",
                                 args->counters->global_count,
                                 args->counters->global_sum,
                                 args->counters->global_sum / args->counters->global_count);

            args->uarttx->printf("Thread 0 count: %lu\r\n"
                        "Thread 0 sum: %lu\r\n"
                        "Thread 0 average: %lu\r\n",
                                 args->counters->thread0_count,
                                 args->counters->thread0_sum,
                                 args->counters->thread0_sum / args->counters->thread0_count);

            args->uarttx->printf("Thread 1 count: %lu\r\n"
                        "Thread 1 sum: %lu\r\n"
                        "Thread 1 average: %lu\r\n",
                                 args->counters->thread1_count,
                                 args->counters->thread1_sum,
                                 args->counters->thread1_sum / args->counters->thread1_count);

            args->uarttx->printf("Thread 2 count: %lu\r\n"
                        "Thread 2 sum: %lu\r\n"
                        "Thread 2 average: %lu\r\n"
                        "\r\n",
                                 args->counters->thread2_count,
                                 args->counters->thread2_sum,
                                 args->counters->thread2_sum / args->counters->thread2_count);
        }

        args->semaphore->put();
        rtos::sleep(S_TO_TICKS(1));
    }
}

void otherThreadEntry(other_thread_args_t* args) {
    ULONG received_message;
    rtos::TXError queue_status;
    rtos::TXError semaphore_status;
    rtos::TXError flag_status;

    args->uarttx->printf("Thread %u Created\n\r", args->num);

    while (1) {
        /* Retrieve a message from the queue. */
        queue_status = args->queue->receive(&received_message, rtos::WaitForever);

        /* Get the semaphore with suspension. */
        semaphore_status = args->semaphore->get(rtos::WaitForever);

        if (queue_status == rtos::Success) {
            if (received_message >= 20) {
                //flag_status = tx_event_flags_set(&event_flags_0, 0x1, TX_OR);
                flag_status = args->eventFlags->set(0x1, false);
            }

            args->counters->global_count++;
            args->counters->global_sum += received_message;

            if (args->num == 1) {
                args->counters->thread1_count++;
                args->counters->thread1_sum += received_message;
            } else if (args->num == 2) {
                args->counters->thread2_count++;
                args->counters->thread1_sum += received_message;
            }

        }

        args->uarttx->printf("Thread %u received message: %lu\r\n"
                    "Thread %u count: %lu\r\n"
                    "Thread %u sum: %lu\r\n"
                    "\r\n",
                    args->num, received_message, args->num, args->counters->thread1_count, args->num, args->counters->thread1_sum);

        semaphore_status = args->semaphore->put();
        rtos::sleep(S_TO_TICKS(1));
    }
}

void semaphoreThreadEntry(other_thread_args_t* args) {

    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    rtos::TXError flag_status;
    ULONG actual_flag;

    args->uarttx->printf("Thread 3 Created\n\r\n\r");

    while (1) {
        flag_status = args->eventFlags->get(0x1, false, true, &actual_flag, rtos::WaitForever);

        if (flag_status == rtos::Success) {
            args->uarttx->printf("Thread 3 flag set\n\r\n\r");
            led.toggle();
            rtos::sleep(S_TO_TICKS(1));
            led.toggle();
        }

        rtos::sleep(S_TO_TICKS(1));
    }
}