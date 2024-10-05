/**
 * This sample shows off Threadx support from Evt-core.
 * This will make 4 threads and send data between them.
 */

#include <EVT/dev/LED.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/log.hpp>


#include <EVT/rtos/Semaphore.hpp>
#include <EVT/rtos/Enums.hpp>
#include <EVT/rtos/EventFlags.hpp>
#include <EVT/rtos/BytePool.hpp>
#include <EVT/rtos/Queue.hpp>
#include <EVT/rtos/Thread.hpp>
#include <EVT/rtos/UARTTX.hpp>

///Namespaces
namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;
namespace log = EVT::core::log;
namespace rtos = core::rtos;

///Defines
#define DEMO_STACK_SIZE 1024
#define DEMO_QUEUE_SIZE 100
#define TX_APP_MEM_POOL_SIZE 65536

typedef struct thread_0_args {
    rtos::Queue* queue;
    rtos::Semaphore* semaphore;
    rtos::wrapper::UARTTX* uarttx;
} thread_0_args_t;

typedef struct other_thread_args {
    rtos::Queue* queue;
    rtos::Semaphore* semaphore;
    rtos::wrapper::UARTTX* uarttx;
    rtos::EventFlags* eventFlags;
} other_thread_args_t;

///Function Prototypes
///TODO: UART thread test
void thread_0_entry(thread_0_args_t* args);
void thread_1_entry(other_thread_args_t* args);
void thread_2_entry(other_thread_args_t* args);
void thread_3_entry(other_thread_args_t* args);


int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    //create all the initializable methods
    rtos::wrapper::UARTTX uarttx(uart);
    rtos::Queue q1("queue", 16, 8);
    rtos::BytePool<TX_APP_MEM_POOL_SIZE> txPool("txBytePool");
    rtos::Semaphore semaphore("Semaphore 1", 1);
    rtos::EventFlags eventFlags("Event Flags");

    //create the thread 0 argument struct
    thread_0_args_t thread_0_args = {&q1, &semaphore, &uarttx};

    //create thread 0
    rtos::Thread<thread_0_args_t*> thread0("Worker Thread 0", thread_0_entry, &thread_0_args,
                                           DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);

    //create the struct that holds the other thread arguments
    other_thread_args_t other_thread_args = {&q1, &semaphore, &uarttx, &eventFlags};

    //create thread1
    rtos::Thread<other_thread_args_t *> thread1("Thread 1", thread_1_entry, &other_thread_args,
                                               DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);
    //create thread2
    rtos::Thread<other_thread_args_t *> thread2("Thread 2", thread_2_entry, &other_thread_args,
                                               DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);
    //create thread3
    rtos::Thread<other_thread_args_t *> thread3("Thread 3", thread_3_entry, &other_thread_args,
                                               DEMO_STACK_SIZE, 1, 1, MS_TO_TICKS(50), true);

    uart.printf("About to start the kernel.\n\r");


    //create the initializable array
    rtos::Initializable *arr[] = {
        &thread0, &uarttx,&q1,&semaphore, &eventFlags, &thread1, &thread2, &thread3
    };

    //start the kernel
    rtos::startKernel(arr, 8, txPool);

    //the startKernel method doesn't actually return so this will never happen.
    return 0;
}

ULONG global_count = 0; //Times a random number has been sent and received
ULONG global_sum = 0;  //Sum of random numbers
ULONG thread0_count = 0;
ULONG thread0_sum = 0;
ULONG thread1_count = 0;
ULONG thread1_sum = 0;
ULONG thread2_count = 0;
ULONG thread2_sum = 0;

///Function definitions
void thread_0_entry(thread_0_args_t* args) {
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
            thread0_count++;
            thread0_sum += num;
        }

        if (thread0_count % 10 == 0) {
            args->uarttx->printf("Global count: %lu\r\n"
                        "Global sum: %lu\r\n"
                        "Global average: %lu\r\n",
                        global_count, global_sum, global_sum / global_count);

            args->uarttx->printf("Thread 0 count: %lu\r\n"
                        "Thread 0 sum: %lu\r\n"
                        "Thread 0 average: %lu\r\n",
                        thread0_count, thread0_sum, thread0_sum / thread0_count);

            args->uarttx->printf("Thread 1 count: %lu\r\n"
                        "Thread 1 sum: %lu\r\n"
                        "Thread 1 average: %lu\r\n",
                        thread1_count, thread1_sum, thread1_sum / thread1_count);

            args->uarttx->printf("Thread 2 count: %lu\r\n"
                        "Thread 2 sum: %lu\r\n"
                        "Thread 2 average: %lu\r\n"
                        "\r\n",
                        thread2_count, thread2_sum, thread2_sum / thread2_count);
        }

        args->semaphore->put();
        rtos::sleep(S_TO_TICKS(1));
    }
}

void thread_1_entry(other_thread_args_t* args) {
    ULONG received_message;
    rtos::TXError queue_status;
    rtos::TXError semaphore_status;
    rtos::TXError flag_status;

    args->uarttx->printf("Thread 1 Created\n\r");

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

            global_count++;
            global_sum += received_message;
            thread1_count++;
            thread1_sum += received_message;
        }

        args->uarttx->printf("Thread 1 received message: %lu\r\n"
                    "Thread 1 count: %lu\r\n"
                    "Thread 1 sum: %lu\r\n"
                    "\r\n",
                    received_message, thread1_count, thread1_sum);

        semaphore_status = args->semaphore->put();
        //tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
        rtos::sleep(S_TO_TICKS(1));
    }
}

void thread_2_entry(other_thread_args_t* args) {
    ULONG received_message;
    rtos::TXError queue_status;
    rtos::TXError semaphore_status;
    rtos::TXError flag_status;

    args->uarttx->printf("Thread 2 Created\n\r");

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

            global_count++;
            global_sum += received_message;
            thread2_count++;
            thread2_sum += received_message;
        }

        args->uarttx->printf("Thread 2 received message: %lu\r\n"
                    "Thread 2 count: %lu\r\n"
                    "Thread 2 sum: %lu\r\n"
                    "\r\n",
                    received_message, thread2_count, thread2_sum);

        semaphore_status = args->semaphore->put();
        rtos::sleep(S_TO_TICKS(1));
    }
}

void thread_3_entry(other_thread_args_t* args) {

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