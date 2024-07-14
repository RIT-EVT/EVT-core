//
// Created by diego on 6/15/2024.
//
#include "tx_api.h"
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/dev/LED.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

///Namespaces
namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

//Needs custom data type
//Look into entry argument when creating a thread when determining what thread it is

///Defines
#define DEMO_STACK_SIZE 1024
#define DEMO_QUEUE_SIZE 100
#define TX_APP_MEM_POOL_SIZE 6536

///Variables
static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;

TX_THREAD thread_0;
TX_THREAD thread_1;
TX_THREAD thread_2;
TX_THREAD thread_3;
TX_QUEUE queue_0;
TX_SEMAPHORE semaphore_0;
TX_EVENT_FLAGS_GROUP event_flags_0;

///Function Prototypes
void thread_0_entry(ULONG thread_input);
void thread_1_entry(ULONG thread_input);
void thread_2_entry(ULONG thread_input);
void thread_3_entry(ULONG thread_input);

//Merged tx_application_define and App_ThreadX_Init functions
VOID tx_application_define(VOID *first_unused_memory) {
    VOID* memory_ptr;
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    if (tx_byte_pool_create(&tx_app_byte_pool, "Tx App memory pool", tx_byte_pool_buffer, TX_APP_MEM_POOL_SIZE) != TX_SUCCESS) {
        uart.printf("\n\rByte pool could not be created\n\r");
    }
    else {

        memory_ptr = (VOID*) &tx_app_byte_pool;

        TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*) memory_ptr;

        /* USER CODE BEGIN App_ThreadX_Init */
        char *pointer = static_cast<char *>(TX_NULL);

        /* Allocate the stack for thread 0.  */
        tx_byte_allocate(byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

        /* Create the main thread 0.  */
        tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0, pointer,
                         DEMO_STACK_SIZE, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

        /* Allocate the stack for thread 1.  */
        tx_byte_allocate(byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

        /* Create the main thread 1.  */
        tx_thread_create(&thread_1, "thread 1", thread_1_entry, 0, pointer,
                         DEMO_STACK_SIZE, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

        /* Allocate the stack for thread 2.  */
        tx_byte_allocate(byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

        /* Create the main thread 2.  */
        tx_thread_create(&thread_2, "thread 2", thread_2_entry, 0, pointer,
                         DEMO_STACK_SIZE, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

        /* Allocate the stack for thread 3.  */
        tx_byte_allocate(byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

        /* Create the main thread 3.  */
        tx_thread_create(&thread_3, "thread 3", thread_3_entry, 0, pointer,
                         DEMO_STACK_SIZE, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

        /* Create the message queue shared by all threads. */
        tx_queue_create(&queue_0, "queue 0", TX_1_ULONG, pointer,
                        DEMO_QUEUE_SIZE*sizeof(ULONG));

        /* Create the semaphore used by all threads. */
        tx_semaphore_create(&semaphore_0, "counting semaphore 0", 1);

        tx_event_flags_create(&event_flags_0, "event flags 0");
    }
}

int main()
{
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    SystemCoreClockUpdate();
    uart.printf("\n\rSystem Clock: %lu\n\r", SystemCoreClock);

    tx_kernel_enter();

    return 0;
}

ULONG global_count = 0; //Times a random number has been sent and received
ULONG global_sum = 0; //Sum of random numbers
ULONG thread0_count = 0;
ULONG thread0_sum = 0;
ULONG thread1_count = 0;
ULONG thread1_sum = 0;
ULONG thread2_count = 0;
ULONG thread2_sum = 0;

///Function declarations
void thread_0_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    srand(77);
    ULONG queue_status;
    ULONG semaphore_status;
    ULONG num;

    uart.printf("\n\rThread 0 Created\n\r");

    /* Delay ensures that thread 1 and thread 2 are created before thread 0 adds to the queue. */
    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);

    while(1){
        num = rand() % 25 + 1;

        /* Send message to queue 0. */
        queue_status = tx_queue_send(&queue_0, &num, TX_WAIT_FOREVER);

        /* Take semaphore when it is released. */
        semaphore_status = tx_semaphore_get(&semaphore_0, TX_WAIT_FOREVER);

        if (queue_status == TX_SUCCESS) {
            thread0_count++;
            thread0_sum += num;
        }

        if(thread0_count % 10 == 0) {
            uart.printf("Global count: %lu\r\n"
                        "Global sum: %lu\r\n"
                        "Global average: %lu\r\n",
                        global_count, global_sum, global_sum / global_count);

            uart.printf("Thread 0 count: %lu\r\n"
                        "Thread 0 sum: %lu\r\n"
                        "Thread 0 average: %lu\r\n",
                        thread0_count, thread0_sum, thread0_sum / thread0_count);

            uart.printf("Thread 1 count: %lu\r\n"
                        "Thread 1 sum: %lu\r\n"
                        "Thread 1 average: %lu\r\n",
                        thread1_count, thread1_sum, thread1_sum / thread1_count);

            uart.printf("Thread 2 count: %lu\r\n"
                        "Thread 2 sum: %lu\r\n"
                        "Thread 2 average: %lu\r\n"
                        "\r\n",
                        thread2_count, thread2_sum, thread2_sum / thread2_count);
        }

        semaphore_status = tx_semaphore_put(&semaphore_0);
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}

void thread_1_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    ULONG received_message;
    ULONG queue_status;
    ULONG semaphore_status;
    UINT flag_status;

    uart.printf("Thread 1 Created\n\r");

    while(1){
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

        uart.printf("Thread 1 received message: %lu\r\n"
                 "Thread 1 count: %lu\r\n"
                 "Thread 1 sum: %lu\r\n"
                 "\r\n", received_message, thread1_count, thread1_sum);

        semaphore_status = tx_semaphore_put(&semaphore_0);
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}

void thread_2_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    ULONG received_message;
    ULONG queue_status;
    ULONG semaphore_status;
    UINT flag_status;

    uart.printf("Thread 2 Created\n\r");

    while(1){
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

        uart.printf("Thread 2 received message: %lu\r\n"
                    "Thread 2 count: %lu\r\n"
                    "Thread 2 sum: %lu\r\n"
                    "\r\n", received_message, thread2_count, thread2_sum);

        semaphore_status = tx_semaphore_put(&semaphore_0);
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}

void thread_3_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    ULONG flag_status;
    ULONG actual_flag;

    uart.printf("Thread 3 Created\n\r\n\r");

    while(1){
        flag_status = tx_event_flags_get(&event_flags_0, 0x1, TX_OR_CLEAR, &actual_flag,TX_WAIT_FOREVER);

        if(flag_status == TX_SUCCESS)
        {
            uart.printf("Thread 3 flag set\n\r\n\r");
            led.toggle();
            tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
            led.toggle();
        }

        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}
