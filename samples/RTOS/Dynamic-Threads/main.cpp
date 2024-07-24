#include "tx_api.h"
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/dev/LED.hpp>
#include <EVT/io/GPIO.hpp>

///Namespaces
namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

///Defines
#define DEMO_STACK_SIZE 1024
#define DEMO_QUEUE_SIZE 100
#define TX_APP_MEM_POOL_SIZE 6536

///Variables
static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;
TX_QUEUE queue_0;
TX_SEMAPHORE semaphore_0;
TX_EVENT_FLAGS_GROUP event_flags_0;
const int ThreadNum = 2;
TX_THREAD generator;
TX_THREAD event;

///Function Prototypes
void generator_entry(ULONG thread_input);
void worker_entry(ULONG thread_input);
void event_entry(ULONG thread_input);

typedef struct {
    TX_THREAD worker;
    int count;
    ULONG sum;
} ThreadData;

typedef void (*EntryFunction)(ULONG thread_input);

ThreadData dataArray[ThreadNum+1];

void ThreadCreator(TX_BYTE_POOL* byte_pool, TX_THREAD* thread, EntryFunction thread_entry, ULONG thread_input) {
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    char* pointer = static_cast<char*>(TX_NULL);

        tx_byte_allocate(byte_pool, (void**) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
        ULONG status = tx_thread_create(thread, "dynamic thread", thread_entry, thread_input, pointer,
                         DEMO_STACK_SIZE, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

        if (status != TX_SUCCESS){
            uart.printf("\n\rThread not be created %lu\n\r", status);
            uart.printf("\n\rThread ptr %lu\n\r", byte_pool);
        }
        else {
            uart.printf("\n\rThread created\n\r");
        }
}

// Merged tx_application_define and App_ThreadX_Init functions
void tx_application_define(void* first_unused_memory) {
    void* memory_ptr;
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    if (tx_byte_pool_create(&tx_app_byte_pool, "Tx App memory pool", tx_byte_pool_buffer, TX_APP_MEM_POOL_SIZE) != TX_SUCCESS) {
        uart.printf("\n\rByte pool could not be created\n\r");
    }
    else {
        memory_ptr = (void*)&tx_app_byte_pool;
        TX_BYTE_POOL* byte_pool = (TX_BYTE_POOL*)memory_ptr;

        tx_queue_create(&queue_0, "queue 0", TX_1_ULONG, tx_byte_pool_buffer,
                        DEMO_QUEUE_SIZE * sizeof(ULONG));

        tx_semaphore_create(&semaphore_0, "counting semaphore 0", 1);

        tx_event_flags_create(&event_flags_0, "event flags 0");

        ThreadCreator(byte_pool,&generator,generator_entry,0 );
        ThreadCreator(byte_pool,&event,event_entry,0 );

        // Create threads
        for (int i = 0; i < ThreadNum; ++i) {
            ThreadCreator(byte_pool, &dataArray[i].worker, worker_entry, i);
        }
    }
}

int main() {
    EVT::core::platform::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    SystemCoreClockUpdate();
    uart.printf("\n\rSystem Clock: %lu\n\r", SystemCoreClock);

    tx_kernel_enter();

    return 0;
}

ULONG global_count = 0; //Times a random number has been sent and received
ULONG global_sum = 0; //Sum of random numbers
ULONG generator_count = 0;
ULONG generator_sum = 0;

///Function declarations
void generator_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    srand(77);
    ULONG queue_status;
    ULONG semaphore_status;
    ULONG num;
    ThreadData *data1 = &dataArray[0];
    ThreadData *data2 = &dataArray[1];

    uart.printf("\n\rGenerator Created\n\r");

    /* Delay ensures that thread 1 and thread 2 are created before thread 0 adds to the queue. */
    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);

    while (1) {
        num = rand() % 25 + 1;

        /* Send message to queue 0. */
        queue_status = tx_queue_send(&queue_0, &num, TX_WAIT_FOREVER);

        /* Take semaphore when it is released. */
        semaphore_status = tx_semaphore_get(&semaphore_0, TX_WAIT_FOREVER);

        if (queue_status == TX_SUCCESS) {
            generator_count++;
            generator_sum += num;
        }

        if (generator_count % 10 == 0) {
            uart.printf("Summary\r\n");
            uart.printf("Global:"
                        "\tcount: %lu\r\n"
                        "\tsum: %lu\r\n"
                        "\taverage: %lu\r\n",
                        global_count, global_sum, global_sum / global_count);

            uart.printf("Generator:\r\n"
                        "\tcount: %lu\r\n"
                        "\tsum: %lu\r\n"
                        "\taverage: %lu\r\n",
                        generator_count, generator_sum, generator_sum / generator_count);

            uart.printf("Worker 1:\r\n"
                        "\tsum: %lu\r\n"
                        "\tcount: %lu\r\n"
                        "\taverage: %lu\r\n",
                        data1->sum, data1->count, data1->sum / data1->count);

            uart.printf("Worker 2:\r\n"
                        "\tsum: %lu\r\n"
                        "\tcount: %lu\r\n"
                        "\taverage: %lu\r\n\r\n",
                        data2->sum, data2->count, data2->sum / data2->count);

        }
            semaphore_status = tx_semaphore_put(&semaphore_0);
            tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}

void worker_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    ULONG received_message;
    ULONG queue_status;
    ULONG semaphore_status;
    UINT flag_status;

    ThreadData *data = &dataArray[thread_input];

    uart.printf("Worker %d Created\n\r", thread_input+1);

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
            data->count++;
            data->sum += received_message;
        }

        uart.printf("Worker %lu:\r\n"
                    "\treceived message: %lu\r\n"
                    "\tcount: %lu\r\n"
                    "\tsum: %lu\r\n"
                    "\r\n", thread_input+1,received_message, data->count, data->sum);

        semaphore_status = tx_semaphore_put(&semaphore_0);
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}

void event_entry(ULONG thread_input) {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    ULONG flag_status;
    ULONG actual_flag;

    uart.printf("Event Created\n\r\n\r");

    while(1){
        flag_status = tx_event_flags_get(&event_flags_0, 0x1, TX_OR_CLEAR, &actual_flag,TX_WAIT_FOREVER);

        if(flag_status == TX_SUCCESS)
        {
            uart.printf("Event flag set\n\r\n\r");
            led.toggle();
            tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
            led.toggle();
        }

        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 1);
    }
}
