/**
 * This is an example program which will go through using the FixedQueue
 * type. The program will print out the results of the operations over UART
 * for the user to watch the result.
 *
 * @author Collin Bolles
 */
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/types/FixedQueue.hpp>

namespace IO = EVT::core::IO;
namespace types = EVT::core::types;

/**
 * Test class for showing off having a custom class in the FixedQueue.
 */
class TestClass {
private:
    int a;
    int b;
    int c;

public:
    TestClass(int a, int b, int c) : a(a), b(b), c(c) {}
    TestClass() : a(0), b(0), c(0) {}

    // Copy operator, important to have
    TestClass& operator=(const TestClass& other) {
        if (this == &other)
            return *this;
        this->a = other.a;
        this->b = other.b;
        this->c = other.c;
        return *this;
    }

    // For easier debugging
    void print(IO::UART* uart) {
        uart->printf("TestClass(a = %d, b = %d, c = %d)", a, b, c);
    }
};

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    ///////////////////////////////////////////////////////////////////////////
    // Test a queue of numbers, no overwritting
    ///////////////////////////////////////////////////////////////////////////
    uart.printf("Queue of Numbers, no overwritting\n\r");
    // First a queue of numbers, max capacity of 10 and no over writting
    types::FixedQueue<10, int> numberQueue;
    // Check if it is empty, should be empty
    uart.printf("numberQueue.isEmpty() -> %d\r\n", numberQueue.isEmpty());

    // Add 15 numbers when only 10 are allowed
    for (int i = 0; i < 15; i++) {
        uart.printf("numberQueue.append(%d)\r\n", i);
        numberQueue.append(i);
    }

    // Check if it is full, should be full
    uart.printf("numberQueue.isFull() -> %d\r\n", numberQueue.isFull());

    // Pull out all the numbers
    for (int i = 0; i < 10; i++) {
        int valuePopped = 0;
        numberQueue.pop(&valuePopped);
        uart.printf("numberQueue.pop() -> %d\r\n", valuePopped);
    }

    // Check if it is empty again, should be
    uart.printf("numberQueue.isFull() -> %d\r\n", numberQueue.isFull());
    uart.printf("numberQueue.isEmpty() -> %d\r\n", numberQueue.isEmpty());

    // Try to pop from empty queue
    int value;
    uart.printf("numberQueue.pop() success ? ->%d\r\n",
                numberQueue.pop(&value));

    ///////////////////////////////////////////////////////////////////////////
    // Test a queue of numbers, with overwritting
    ///////////////////////////////////////////////////////////////////////////
    uart.printf("\r\n\r\nQueue of Numbers, with overwritting\n\r");
    types::FixedQueue<10, int> numberQueueOverwrite(10);

    // Add ten numbers, should be full after
    for (int i = 0; i < 10; i++) {
        uart.printf("numberQueueOverwrite.append(%d)\n\r", i);
        numberQueueOverwrite.append(i);
    }
    uart.printf("numberQueueOverwrite.isFull() -> %d\r\n",
                numberQueueOverwrite.isFull());
    uart.printf("numberQueueOverwrite.canInsert() -> %d\r\n",
                numberQueueOverwrite.canInsert());

    // Add ten more numbers
    for (int i = 10; i < 20; i++) {
        uart.printf("numberQueueOverwrite.append(%d)\n\r", i);
        numberQueueOverwrite.append(i);
    }
    for (int i = 0; i < 10; i++) {
        int valuePopped = 0;
        numberQueueOverwrite.pop(&valuePopped);
        uart.printf("numberQueueOverwrite.pop() -> %d\r\n", valuePopped);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Test a queue of custom types
    ///////////////////////////////////////////////////////////////////////////
    uart.printf("\r\n\r\nQueue of Custom Objects, no overwritting\n\r");
    types::FixedQueue<15, TestClass> customQueue(10);

    // Add ten objects to the queue
    for (int i = 0; i < 15; i++) {
        TestClass element(i * 1, i * 2, i * 3);
        uart.printf("customQueue.append(");
        element.print(&uart);
        uart.printf(")\r\n");
        customQueue.append(element);
    }

    // Print out all of the elements
    for (int i = 0; i < 15; i++) {
        TestClass element;
        customQueue.pop(&element);
        uart.printf("numberQueueOverwrite.pop() -> ");
        element.print(&uart);
        uart.printf("\r\n");
    }

    return 0;
}
