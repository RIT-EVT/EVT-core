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

/// rtos includes
#include <core/rtos/BytePool.hpp>
#include <core/rtos/Enums.hpp>
#include <core/rtos/EventFlags.hpp>
#include <core/rtos/Queue.hpp>
#include <core/rtos/Semaphore.hpp>
#include <core/rtos/Thread.hpp>
#include <core/rtos/UARTTX.hpp>

/// Namespaces
namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;
namespace log  = core::log;
namespace rtos = core::rtos;

/// Defines
#define DEMO_STACK_SIZE      1024
#define DEMO_QUEUE_SIZE      100
#define TX_APP_MEM_POOL_SIZE 65536

/**
 * Struct definition that holds all the counters for each thread.
 */
typedef struct counters {
    uint32_t numThreads;
    uint32_t global_count; // Times a random number has been sent and received
    uint32_t global_sum;   // Sum of random numbers
    uint32_t* count_array;
    uint32_t* sum_array;
} counters_t;

/**
 * Struct that holds the arguments for the controller thread
 */
typedef struct controller_thread_args {
    rtos::Queue* queue;
    rtos::Semaphore* semaphore;
    rtos::wrapper::UARTTX* uarttx;
    counters_t* counters;
} controller_thread_args_t;

/**
 * Struct that holds the arguments for all other threads
 */
typedef struct worker_thread_args {
    rtos::Queue* queue;
    rtos::Semaphore* semaphore;
    rtos::wrapper::UARTTX* uarttx;
    rtos::EventFlags* eventFlags;
    uint8_t num;
    counters_t* counters;
} worker_thread_args_t;

/// Function Prototypes (allows us to actually implement the functions below main)
void controllerThreadEntry(controller_thread_args_t* args);
void workerThreadEntry(worker_thread_args_t* args);
void eventFlagThreadEntry(worker_thread_args_t* args);

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    rtos::wrapper::UARTTX uarttx(uart);
    rtos::Queue q1((char*) "queue", 16, 20);
    rtos::BytePool<TX_APP_MEM_POOL_SIZE> txPool((char*) "txBytePool");
    rtos::Semaphore semaphore((char*) "Semaphore 1", 1);
    rtos::EventFlags eventFlags((char*) "Event Flags");

    // create the counters (they all start at 0)
    uint32_t counter_array[] = {0,0,0};
    uint32_t sum_array[] = {0,0,0};
    counters_t counters = {3, 0, 0, counter_array, sum_array};

    // create the thread 0 argument struct
    controller_thread_args_t controllerThreadArgs = {&q1, &semaphore, &uarttx, &counters};

    // create thread 0
    rtos::Thread<controller_thread_args_t*> controllerThread((char*) "Controller Thread",
                                                             controllerThreadEntry,
                                                             &controllerThreadArgs,
                                                             DEMO_STACK_SIZE,
                                                             1,
                                                             1,
                                                             MS_TO_TICKS(50),
                                                             true);

    // create the structs that holds the other thread arguments
    worker_thread_args_t thread_1_args = {&q1, &semaphore, &uarttx, &eventFlags, 1, &counters};

    worker_thread_args_t thread_2_args = {&q1, &semaphore, &uarttx, &eventFlags, 2, &counters};

    worker_thread_args_t thread_3_args = {&q1, &semaphore, &uarttx, &eventFlags, 3, &counters};

    // create thread1
    rtos::Thread<worker_thread_args_t*> thread1(
        (char*) "Thread 1", workerThreadEntry, &thread_1_args, DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);
    // create thread2
    rtos::Thread<worker_thread_args_t*> thread2(
        (char*) "Thread 2", workerThreadEntry, &thread_2_args, DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);
    // create thread3
    rtos::Thread<worker_thread_args_t*> eventFlagThread(
        (char*) "Thread 3", eventFlagThreadEntry, &thread_3_args, DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);

    uart.printf("About to start the kernel.\n\r");

    /*
     * Most RTOS objects are initializable, which means we need to register them with the threadx kernel for them
     * to function. This is done by putting all these initializable objects into an array (see below),
     * and then passing that array into the startKernel method.
     * If an object neeeds to be created and initialized after the kernel has been started, it's initialize method must
     * be manually called. Also, make sure the length of the array is correct in startKernel.
     */
    rtos::Initializable* arr[] = {
        &controllerThread, &uarttx, &q1, &semaphore, &eventFlags, &thread1, &thread2, &eventFlagThread};

    // start the kernel (the kernel takes in the array of initializables and initializes them when the threadx kernel
    // starts)
    rtos::startKernel(arr, 8, txPool);

    // the startKernel method doesn't actually return so this will never happen.
    return 0;
}

// Function definitions

/**
 * Controller Thread Entry Function, generates a random number then waits for the semaphore and writes stats to UART.
 * @param args the argument struct this thread needs.
 */
void controllerThreadEntry(controller_thread_args_t* args) {
    srand(77);
    rtos::TXError queue_status;
    uint32_t num;

    // IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    // uart.printf("test\n\r");
    args->uarttx->printf("\n\rThread 0 Created\n\r");

    // this is to test that the uarttx handles long messages correctly
    args->uarttx->printf(
        "\n\rThis is a very long message wow it is so long that's so crazy how long this is wowee\n\r");
    while (1) {
        num = rand() % 25 + 1;

        /* Send message to queue 0. */
        queue_status = args->queue->send(&num, rtos::TXWait::WAIT_FOREVER);
        if (queue_status != rtos::TXError::SUCCESS) {
            args->uarttx->printf("Error on Thread 0 send to queue: %u", queue_status);
        }

        /* Take semaphore when it is released. */
        args->semaphore->get(rtos::WAIT_FOREVER);

        args->uarttx->printf("\n\rThread 0: %lu\n\r", num);

        if (queue_status == rtos::TXError::SUCCESS) {
            args->counters->count_array[0]++;
            args->counters->sum_array[0] += num;
        }

        if (args->counters->count_array[0] % 10 == 0) {
            args->uarttx->printf("Global count: %lu\r\n"
                                 "Global sum: %lu\r\n"
                                 "Global average: %lu\r\n",
                                 args->counters->global_count,
                                 args->counters->global_sum,
                                 args->counters->global_sum / args->counters->global_count);
            for (uint32_t i = 0; i < args->counters->numThreads; i++) {
                args->uarttx->printf("Thread %lu count: %lu\r\n"
                                     "Thread %lu sum: %lu\r\n"
                                     "Thread %lu average: %lu\r\n",
                                     i, args->counters->count_array[i],
                                     i, args->counters->sum_array[i],
                                     i, args->counters->sum_array[i] / args->counters->count_array[i]);
            }
        }

        args->semaphore->put();
        rtos::sleep(S_TO_TICKS(1));
    }
}

/**
 * Entry Function for Threads 1 and 2. Waits to get a message in the queue from the controller thread, then
 * increases its specific value in the counter struct and prints some stuff.
 * @param args the argument struct this thread needs.
 */
void workerThreadEntry(worker_thread_args_t* args) {
    ULONG received_message;
    rtos::TXError queue_status;
    rtos::TXError semaphore_status;
    rtos::TXError flag_status;

    args->uarttx->printf("Thread %u Created\n\r", args->num);

    while (1) {
        /* Retrieve a message from the queue. */
        queue_status = args->queue->receive(&received_message, rtos::WAIT_FOREVER);

        /* Get the semaphore with suspension. */
        semaphore_status = args->semaphore->get(rtos::WAIT_FOREVER);

        if (queue_status == rtos::SUCCESS) {
            if (received_message >= 20) {
                flag_status = args->eventFlags->set(0x1, false);
            }

            args->counters->global_count++;
            args->counters->global_sum += received_message;

            //update arrays
            args->counters->count_array[args->num]++;
            args->counters->sum_array[args->num] += received_message;

        }

        args->uarttx->printf("Thread %u received message: %lu\r\n"
                             "Thread %u count: %lu\r\n"
                             "Thread %u sum: %lu\r\n"
                             "\r\n",
                             args->num,
                             received_message,
                             args->num,
                             args->counters->count_array[args->num],
                             args->num,
                             args->counters->sum_array[args->num]);

        semaphore_status = args->semaphore->put();
        rtos::sleep(S_TO_TICKS(1));
    }
}

/**
 * Event Flag Thread Entry Function. Literally just waits for the event flag 0x1 to be up and then prints that it was
 * set.
 * @param args the arguments this thread needs.
 */
void eventFlagThreadEntry(worker_thread_args_t* args) {

    io::GPIO& ledGPIO = io::getGPIO<io::Pin::LED>();
    dev::LED led(ledGPIO, dev::LED::ActiveState::HIGH);

    rtos::TXError flag_status;
    ULONG actual_flag;

    args->uarttx->printf("Thread 3 Created\n\r\n\r");

    while (1) {
        flag_status = args->eventFlags->get(0x1, false, true, rtos::WAIT_FOREVER, &actual_flag);

        if (flag_status == rtos::SUCCESS) {
            args->uarttx->printf("Thread 3 flag set\n\r\n\r");
            led.toggle();
            rtos::sleep(S_TO_TICKS(1));
            led.toggle();
        }

        rtos::sleep(S_TO_TICKS(1));
    }
}