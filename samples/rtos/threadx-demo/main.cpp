/**
 * This sample shows off Threadx support from Evt-core.
 * This will make 4 threads and send data between them.
 * The first thread Generates random numbers and sends them to a Queue.
 * The second and third threads read those random numbers and interpret them in some way,
 * raising certain flags of an eventFlags once they have recieved a message.
 * The fourth thread looks for certain eventFlags being raised and outputs based on that.
 */

/// core includes
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
#include <core/rtos/tsio/ThreadUART.hpp>

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
    uint32_t numThreads;   // How many threads have been generated
    uint32_t global_count; // Times a random number has been sent and received
    uint32_t global_sum;   // Sum of random numbers
    uint32_t* count_array; // The array of the count of how many times each thread has gotten a number
    uint32_t* sum_array;   // The array of the sums of the counts of each thread.
} counters_t;

/**
 * Struct that holds the arguments for the thread that generates the numbers
 */
typedef struct number_gen_thread_args {
    rtos::Queue* queue; // The queue of numbers that the number gen thread add it's generated number to
    rtos::Semaphore*
        semaphore; // A semaphore that mutually excludes the threads from accessing the queue or printing to uart at the
                   // same time these shouldn't necessarily need to be mutually exclusive, as they are generally
                   // thread-safe, but it also shows an example implementation of a semaphore.
    rtos::tsio::ThreadUART* threadUART; // The instance of ThreadUART that this thread will use to print.
    counters_t* counters; // The struct of counters for the number gen thread to print out the counts of each thread
} number_gen_thread_args_t;

/**
 * Struct that holds the arguments for all other threads
 */
typedef struct number_consumer_thread_args {
    rtos::Queue* queue; // The queue that the threads will pull values that the number gen thread adds
    rtos::Semaphore*
        semaphore; // A semaphore that mutually excludes the threads from accessing the queue or printing to uart
    rtos::tsio::ThreadUART* threadUART; // The instance of ThreadUART that this thread will use to print.
    rtos::EventFlags* eventFlags; // the EventFlags that the number consumer threads will use to show they have gotten a
                                  // number, which will trigger the eventFlagThread to run it's own method.
    uint8_t num;          // What number this thread is
    counters_t* counters; // the struct of counters for the number consumer threads to increase their counters when they
                          // get a number
} number_consumer_thread_args_t;

/// Function Prototypes (allows us to actually implement the functions below main)
void generatorThreadEntry(number_gen_thread_args_t* args);
void consumerThreadEntry(number_consumer_thread_args_t* args);
void eventFlagThreadEntry(number_consumer_thread_args_t* args);

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    rtos::tsio::ThreadUART threadUART(uart);

    log::LOGGER.setUART(&threadUART);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    rtos::Queue q1((char*) "queue", 16, 20);
    rtos::BytePool<TX_APP_MEM_POOL_SIZE> txPool((char*) "txBytePool");
    rtos::Semaphore semaphore((char*) "Semaphore 1", 1);
    rtos::EventFlags eventFlags((char*) "Event Flags");

    // create the counters (they all start at 0)
    uint32_t counter_array[] = {0, 0, 0};
    uint32_t sum_array[]     = {0, 0, 0};
    counters_t counters      = {3, 0, 0, counter_array, sum_array};

    // create the thread 0 argument struct
    number_gen_thread_args_t generatorThreadArgs = {&q1, &semaphore, &threadUART, &counters};

    // create thread 0
    rtos::Thread<number_gen_thread_args_t*> generatorThread((char*) "Controller Thread",
                                                            generatorThreadEntry,
                                                            &generatorThreadArgs,
                                                            DEMO_STACK_SIZE,
                                                            1,
                                                            1,
                                                            MS_TO_TICKS(50),
                                                            true);

    // create the structs that holds the other thread arguments
    number_consumer_thread_args_t thread_1_args = {&q1, &semaphore, &threadUART, &eventFlags, 1, &counters};

    number_consumer_thread_args_t thread_2_args = {&q1, &semaphore, &threadUART, &eventFlags, 2, &counters};

    number_consumer_thread_args_t thread_3_args = {&q1, &semaphore, &threadUART, &eventFlags, 3, &counters};

    // create thread1
    rtos::Thread<number_consumer_thread_args_t*> thread1(
        (char*) "Thread 1", consumerThreadEntry, &thread_1_args, DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);
    // create thread2
    rtos::Thread<number_consumer_thread_args_t*> thread2(
        (char*) "Thread 2", consumerThreadEntry, &thread_2_args, DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);
    // create thread3
    rtos::Thread<number_consumer_thread_args_t*> eventFlagThread(
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
        &generatorThread, &threadUART, &q1, &semaphore, &eventFlags, &thread1, &thread2, &eventFlagThread};

    // start the kernel (the kernel takes in the array of initializables and initializes them when the threadx kernel
    // starts)
    rtos::startKernel(arr, 8, txPool);

    // the startKernel method doesn't actually return so this will never happen.
    return 0;
}

// Function definitions

/**
 * Generator Thread Entry Function, generates a random number then waits for the semaphore and writes stats to UART.
 *
 * @param args the argument struct this thread needs.
 */
void generatorThreadEntry(number_gen_thread_args_t* args) {
    srand(77);
    rtos::TXError queue_status;
    uint32_t num;

    args->threadUART->printf("\n\rThread 0 Created\n\r");

    // Test that the threadUART handles long messages correctly
    args->threadUART->printf(
        "\n\rThis is a very long message wow it is so long that's so crazy how long this is wowee\n\r");
    while (1) {
        num = rand() % 25 + 1;

        /* Send message to queue 0. */
        queue_status = args->queue->send(&num, rtos::TXWait::WAIT_FOREVER);
        if (queue_status != rtos::TXError::SUCCESS) {
            args->threadUART->printf("Error on Thread 0 send to queue: %u", queue_status);
        }

        /* Take semaphore when it is released. */
        args->semaphore->get(rtos::WAIT_FOREVER);

        args->threadUART->printf("\n\rThread 0: %lu\n\r", num);

        if (queue_status == rtos::TXError::SUCCESS) {
            args->counters->count_array[0]++;
            args->counters->sum_array[0] += num;
        }

        if (args->counters->count_array[0] % 10 == 0) {
            args->threadUART->printf("Global count: %lu\r\n"
                                     "Global sum: %lu\r\n"
                                     "Global average: %lu\r\n",
                                     args->counters->global_count,
                                     args->counters->global_sum,
                                     args->counters->global_sum / args->counters->global_count);
            for (uint32_t i = 0; i < args->counters->numThreads; i++) {
                args->threadUART->printf("Thread %lu count: %lu\r\n"
                                         "Thread %lu sum: %lu\r\n"
                                         "Thread %lu average: %lu\r\n",
                                         i,
                                         args->counters->count_array[i],
                                         i,
                                         args->counters->sum_array[i],
                                         i,
                                         args->counters->sum_array[i] / args->counters->count_array[i]);
            }
        }

        args->semaphore->put();
        rtos::sleep(S_TO_TICKS(1));
    }
}

/**
 * Entry Function for Threads 1 and 2. Waits to get a message in the queue from the controller thread, then
 * increases its specific value in the counter struct and prints information about this thread.
 *
 * @param args the argument struct this thread needs.
 */
void consumerThreadEntry(number_consumer_thread_args_t* args) {
    ULONG received_message;
    rtos::TXError queue_status;
    rtos::TXError semaphore_status;
    rtos::TXError flag_status;

    args->threadUART->printf("Thread %u Created\n\r", args->num);

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

            // update arrays
            args->counters->count_array[args->num]++;
            args->counters->sum_array[args->num] += received_message;
        }

        args->threadUART->printf("Thread %u received message: %lu\r\n"
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
 * set
 *
 * @param args the arguments this thread needs.
 */
void eventFlagThreadEntry(number_consumer_thread_args_t* args) {

    io::GPIO& ledGPIO = io::getGPIO<io::Pin::LED>();
    dev::LED led(ledGPIO, dev::LED::ActiveState::HIGH);

    rtos::TXError flag_status;
    ULONG actual_flag;

    args->threadUART->printf("Thread 3 Created\n\r\n\r");

    while (1) {
        flag_status = args->eventFlags->get(0x1, false, true, rtos::WAIT_FOREVER, &actual_flag);

        if (flag_status == rtos::SUCCESS) {
            args->threadUART->printf("Thread 3 flag set\n\r\n\r");
            led.toggle();
            rtos::sleep(S_TO_TICKS(1));
            led.toggle();
        }

        rtos::sleep(S_TO_TICKS(1));
    }
}