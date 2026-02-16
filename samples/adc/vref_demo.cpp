/**
 * This example demonstrates the VREF functionality of the ADC.
 * It shows how to set a custom reference voltage and how it affects
 * the voltage readings. It also demonstrates ADC calibration for
 * improved accuracy.
 */
#include <core/io/ADC.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

// Function to perform ADC test at a given reference voltage
void testADCAtVref(io::ADC& adc, float vref, const char* vref_name, io::UART& uart) {
    uart.printf("\r\n=== Testing with %s ===\r\n", vref_name);
    adc.setVref(vref);
    uint32_t vref_mv = static_cast<uint32_t>(adc.getVref() * 1000);
    uart.printf("VREF: %d mV\r\n", vref_mv);

    for (int i = 0; i < 5; i++) {
        float voltage    = adc.read();
        float percentage = adc.readPercentage();
        uint32_t raw     = adc.readRaw();

        uint32_t voltage_mv     = static_cast<uint32_t>(voltage * 1000);
        uint32_t percentage_int = static_cast<uint32_t>(percentage * 100);

        uart.printf("Reading %d: %d mV (%d%%, raw: %d)\r\n", i + 1, voltage_mv, percentage_int, raw);
        time::wait(200);
    }
}

int main() {
    // Initialize system
    core::platform::init();

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    uart.printf("Starting ADC VREF Demo\r\n");

    time::wait(500);

    io::ADC& adc0 = io::getADC<io::Pin::PA_0, io::ADCPeriph::ONE>();

    // Display initial VREF setting
    uint32_t vref_mv = static_cast<uint32_t>(adc0.getVref() * 1000);
    uart.printf("Initial VREF: %d mV\r\n", vref_mv);

    time::wait(1000);

    // Test with default VREF (3.3V)
    testADCAtVref(adc0, 3.3f, "default VREF (3.3V)", uart);

    // Test with maximum safe VREF (3.6V)
    testADCAtVref(adc0, 3.6f, "maximum safe VREF (3.6V)", uart);

    // Test with low voltage VREF (1.8V)
    testADCAtVref(adc0, 1.8f, "low voltage VREF (1.8V)", uart);

    // Demonstrate that raw values don't change, only voltage calculations do
    uart.printf("\r\n=== Demonstrating that raw values are independent of VREF ===\r\n");
    uart.printf("Note: Raw ADC values should remain the same regardless of VREF setting\r\n");

    float test_vrefs[] = {1.0f, 2.5f, 3.3f, 3.6f};
    for (float vref : test_vrefs) {
        adc0.setVref(vref);
        uint32_t raw        = adc0.readRaw();
        float voltage       = adc0.read();
        uint32_t vref_mv    = static_cast<uint32_t>(vref * 1000);
        uint32_t voltage_mv = static_cast<uint32_t>(voltage * 1000);
        uart.printf("VREF: %d mV -> Raw: %d, Voltage: %d mV\r\n", vref_mv, raw, voltage_mv);
        time::wait(100);
    }

    uart.printf("\r\n=== VREF Demo Complete ===\r\n");
    uart.printf("The same raw ADC value produces different voltage readings\r\n");
    uart.printf("based on the VREF setting, allowing for accurate measurements\r\n");
    uart.printf("with different reference voltages.\r\n");

    while (1) {
        time::wait(1000);
    }
}
