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

int main() {
    // Initialize system
    core::platform::init();

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // Set up the logger to catch errors in ADC creation
    core::log::LOGGER.setUART(&uart);
    core::log::LOGGER.setLogLevel(core::log::Logger::LogLevel::INFO);

    uart.printf("Starting ADC VREF Demo\r\n");

    time::wait(500);

    io::ADC& adc0 = io::getADC<io::Pin::PA_0, io::ADCPeriph::ONE>();

    // Display initial VREF setting
    uint32_t vref_mv = static_cast<uint32_t>(adc0.getVref() * 1000);
    uart.printf("Initial VREF: %d mV\r\n", vref_mv);

    // Note: ADC calibration is available in platform-specific implementations
    // For STM32F3xx: ADCf3xx::calibrate()
    // For STM32F4xx: ADCf4xx::calibrate()
    uart.printf("Note: ADC calibration is available via platform-specific methods\r\n");

    time::wait(1000);

    // Test with default VREF (3.3V)
    uart.printf("\r\n=== Testing with default VREF (3.3V) ===\r\n");
    for (int i = 0; i < 5; i++) {
        float voltage = adc0.read();
        float percentage = adc0.readPercentage();
        uint32_t raw = adc0.readRaw();
        
        uart.printf("Reading %d: %.3f V (%.1f%%, raw: %d)\r\n", 
                   i+1, voltage, percentage * 100, raw);
        time::wait(200);
    }

    // Change VREF to 5.0V (common for many sensors)
    uart.printf("\r\n=== Changing VREF to 5.0V ===\r\n");
    adc0.setVref(5.0f);
    uart.printf("New VREF: %.2f V\r\n", adc0.getVref());

    // Test with new VREF
    uart.printf("\r\n=== Testing with new VREF (5.0V) ===\r\n");
    for (int i = 0; i < 5; i++) {
        float voltage = adc0.read();
        float percentage = adc0.readPercentage();
        uint32_t raw = adc0.readRaw();
        
        uart.printf("Reading %d: %.3f V (%.1f%%, raw: %d)\r\n", 
                   i+1, voltage, percentage * 100, raw);
        time::wait(200);
    }

    // Change VREF to 1.8V (common for low-voltage systems)
    uart.printf("\r\n=== Changing VREF to 1.8V ===\r\n");
    adc0.setVref(1.8f);
    uart.printf("New VREF: %.2f V\r\n", adc0.getVref());

    // Test with new VREF
    uart.printf("\r\n=== Testing with new VREF (1.8V) ===\r\n");
    for (int i = 0; i < 5; i++) {
        float voltage = adc0.read();
        float percentage = adc0.readPercentage();
        uint32_t raw = adc0.readRaw();
        
        uart.printf("Reading %d: %.3f V (%.1f%%, raw: %d)\r\n", 
                   i+1, voltage, percentage * 100, raw);
        time::wait(200);
    }

    // Demonstrate that raw values don't change, only voltage calculations do
    uart.printf("\r\n=== Demonstrating that raw values are independent of VREF ===\r\n");
    uart.printf("Note: Raw ADC values should remain the same regardless of VREF setting\r\n");
    
    float test_vrefs[] = {1.0f, 2.5f, 3.3f, 5.0f};
    for (float vref : test_vrefs) {
        adc0.setVref(vref);
        uint32_t raw = adc0.readRaw();
        float voltage = adc0.read();
        uart.printf("VREF: %.1fV -> Raw: %d, Voltage: %.3fV\r\n", vref, raw, voltage);
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
