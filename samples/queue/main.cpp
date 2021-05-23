/**
 * This is an example program which will go through using the FixedQueue
 * type. The program will print out the results of the operations over UART
 * for the user to watch the result.
 *
 * @author Collin Bolles
 */
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

namespace IO = EVT::core::IO;
namespace types = EVT::core::types;

int main() {
    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    ///////////////////////////////////////////////////////////////////////////
    // Test a queue of numbers, no overwritting
    ///////////////////////////////////////////////////////////////////////////
    uart.printf("Queue of Numbers\n\r");
    // First a queue of numbers, max capacity of 10 and no over writting
    types::FixedQueue<int> numberQueue(10);
    // Check if it is empty, should be empty
    uart.printf("numberQueue.isEmpty() -> %d\r\n", numberQueue.isEmpty());

    // Add ten numbers, the max allowed
    for(int i = 0; i < 10; i++) {
        uart.printf("numberQueue.append(%d)\r\n", i);
        numberQueue.append(i);
    }

    // Check if it is full, should be full
    uart.printf("numberQueue.isFull() -> %d\r\n", numberQueue.isFull());

    // Pull out all the numbers
    for(int i = 0; i < 10; i++) {
        int valuePopped;
        numberQueue.pop(&valuePopped);
        uart.printf("numberQueue.pop() -> %d\r\n", valuePopped);
    }

    // Check if it is empty again, should be
    uart.printf("numberQueue.isFull() -> %d\r\n", numberQueue.isFull());
    uart.printf("numberQueue.isEmpty() -> %d\r\n", numberQueue.isEmpty());

    ///////////////////////////////////////////////////////////////////////////
    // Test a queue of numbers, with overwritting
    ///////////////////////////////////////////////////////////////////////////

    return 0;
}
