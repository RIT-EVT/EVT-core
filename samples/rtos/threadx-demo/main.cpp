/**
 * This sample shows off Threadx support from Evt-core.
 * This will make 4 threads and send data between them.
 */

#include <EVT/dev/LED.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>

#include <EVT/rtos/Semaphore.hpp>
#include <EVT/rtos/Enums.hpp>
#include <EVT/rtos/BytePool.hpp>
#include <EVT/rtos/Queue.hpp>
#include <EVT/rtos/Thread.hpp>
#include <EVT/rtos/UARTTX.hpp>

///Namespaces
namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;
namespace rtos = core::rtos;

///Defines
#define DEMO_STACK_SIZE 1024
#define DEMO_QUEUE_SIZE 100
#define TX_APP_MEM_POOL_SIZE 65536

///Variables
static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;

///TODO: UART thread test
TX_THREAD thread_uart;
TX_THREAD thread_0;
TX_THREAD thread_1;
TX_THREAD thread_2;
TX_THREAD thread_3;
TX_QUEUE queue_0;
TX_QUEUE queue_uart;
TX_SEMAPHORE semaphore_0;
TX_EVENT_FLAGS_GROUP event_flags_0;

typedef struct thread_0_args {
    rtos::Queue* queue;
    rtos::Semaphore* semaphore;
} thread_0_args_t;

///Function Prototypes
///TODO: UART thread test
void thread_uart_entry(ULONG arg);
void thread_0_entry(thread_0_args_t* args);
void thread_1_entry(ULONG thread_input);
void thread_2_entry(ULONG thread_input);
void thread_3_entry(ULONG thread_input);


int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    rtos::wrapper::UARTTX uarttx(uart);
    rtos::Queue q1("queue", 4, 8);
    rtos::BytePool<TX_APP_MEM_POOL_SIZE> txPool("txBytePool");
    rtos::Semaphore semaphore("Semaphore 1", 0);

    thread_0_args_t thread_0_args = {&q1, &semaphore};

    rtos::Thread<thread_0_args_t*> thread0("Worker Thread 0", thread_0_entry, &thread_0_args, 64, 0, 2, 32, true);


    rtos::Initializable *arr[3] = {
        &q1, &uarttx
    };

    rtos::startKernel(*arr, 3, txPool);

    return 0;
}

ULONG global_count = 0;//Times a random number has been sent and received
ULONG global_sum = 0;  //Sum of random numbers
ULONG thread0_count = 0;
ULONG thread0_sum = 0;
ULONG thread1_count = 0;
ULONG thread1_sum = 0;
ULONG thread2_count = 0;
ULONG thread2_sum = 0;

///Function definitions
void thread_0_entry(thread_0_args_t* args) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    rtos::wrapper::UARTTX uarttx(uart);

    srand(77);
    rtos::TXError queue_status;
    ULONG num;

    uarttx.printf("\n\rThread 0 Created\n\r");

    /* Delay ensures that thread 1 and thread 2 are created before thread 0 adds to the queue. */
    //tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    //could be replaced by:
    core::rtos::sleep(S_TO_TICKS(1));

    while (1) {
        num = rand() % 25 + 1;

        /* Send message to queue 0. */
        queue_status = args->queue->send(&num, rtos::TXWait::WaitForever);
        if (queue_status != rtos::TXError::Success) {
            //we could throw an error here
        }

        /* Take semaphore when it is released. */
        args->semaphore->get(rtos::WaitForever);

        uarttx.printf("\n\rThread 0: %lu\n\r", num);

        if (queue_status == rtos::TXError::Success) {
            thread0_count++;
            thread0_sum += num;
        }

        if (thread0_count % 10 == 0) {
            uarttx.printf("Global count: %lu\r\n"
                        "Global sum: %lu\r\n"
                        "Global average: %lu\r\n",
                        global_count, global_sum, global_sum / global_count);

            uarttx.printf("Thread 0 count: %lu\r\n"
                        "Thread 0 sum: %lu\r\n"
                        "Thread 0 average: %lu\r\n",
                        thread0_count, thread0_sum, thread0_sum / thread0_count);

            uarttx.printf("Thread 1 count: %lu\r\n"
                        "Thread 1 sum: %lu\r\n"
                        "Thread 1 average: %lu\r\n",
                        thread1_count, thread1_sum, thread1_sum / thread1_count);

            uarttx.printf("Thread 2 count: %lu\r\n"
                        "Thread 2 sum: %lu\r\n"
                        "Thread 2 average: %lu\r\n"
                        "\r\n",
                        thread2_count, thread2_sum, thread2_sum / thread2_count);
        }

        args->semaphore->put();
        rtos::sleep(S_TO_TICKS(1));
    }
}

void thread_1_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    rtos::wrapper::UARTTX uarttx(uart);

    ULONG received_message;
    ULONG queue_status;
    ULONG semaphore_status;
    UINT flag_status;

    uarttx.printf("Thread 1 Created\n\r");

    while (1) {
        /* Retrieve a message from the queue. */
        queue_status = tx_queue_receive(&queue_0, &received_message,
                                        TX_WAIT_FOREVER);

        /* Get the semaphore with suspension. */
        semaphore_status = tx_semaphore_get(&semaphore_0, TX_WAIT_FOREVER);

        if (queue_status == TX_SUCCESS) {
            if (received_message >= 20) {
                flag_status = tx_event_flags_set(&event_flags_0, 0x1, TX_OR);
            }

            global_count++;
            global_sum += received_message;
            thread1_count++;
            thread1_sum += received_message;
        }

        uarttx.printf("Thread 1 received message: %lu\r\n"
                    "Thread 1 count: %lu\r\n"
                    "Thread 1 sum: %lu\r\n"
                    "\r\n",
                    received_message, thread1_count, thread1_sum);

        semaphore_status = tx_semaphore_put(&semaphore_0);
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}

void thread_2_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    rtos::wrapper::UARTTX uarttx(uart);

    ULONG received_message;
    ULONG queue_status;
    ULONG semaphore_status;
    UINT flag_status;

    uarttx.printf("Thread 2 Created\n\r");

    while (1) {
        /* Retrieve a message from the queue. */
        queue_status = tx_queue_receive(&queue_0, &received_message,
                                        TX_WAIT_FOREVER);

        /* Get the semaphore with suspension. */
        semaphore_status = tx_semaphore_get(&semaphore_0, TX_WAIT_FOREVER);

        if (queue_status == TX_SUCCESS) {
            if (received_message >= 20) {
                flag_status = tx_event_flags_set(&event_flags_0, 0x1, TX_OR);
            }

            global_count++;
            global_sum += received_message;
            thread2_count++;
            thread2_sum += received_message;
        }

        uarttx.printf("Thread 2 received message: %lu\r\n"
                    "Thread 2 count: %lu\r\n"
                    "Thread 2 sum: %lu\r\n"
                    "\r\n",
                    received_message, thread2_count, thread2_sum);

        semaphore_status = tx_semaphore_put(&semaphore_0);
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}

void thread_3_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    rtos::wrapper::UARTTX uarttx(uart);

    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    ULONG flag_status;
    ULONG actual_flag;

    uarttx.printf("Thread 3 Created\n\r\n\r");

    while (1) {
        flag_status = tx_event_flags_get(&event_flags_0, 0x1, TX_OR_CLEAR, &actual_flag, TX_WAIT_FOREVER);

        if (flag_status == TX_SUCCESS) {
            uarttx.printf("Thread 3 flag set\n\r\n\r");
            led.toggle();
            tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
            led.toggle();
        }

        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}