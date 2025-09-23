/**
 * This example demonstrates comprehensive DAC functionality testing.
 * The DAC outputs known voltages which are read back by ADC to validate
 * both DAC and ADC functionality. This creates a self-test system.
 *
 * Hardware Connection: DAC_OUT (PA4) -> ADC_IN (PA0)
 *
 * For STM32F3xx: DAC1_OUT1 (PA4) and ADC1_IN1 (PA0)
 * For STM32F4xx: DAC1_OUT1 (PA4) and ADC1_IN1 (PA0)
 */

#include <core/io/ADC.hpp>
#include <core/io/DAC.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

#ifdef STM32F3xx
    #include <core/io/platform/f3xx/ADCf3xx.hpp>
    #include <core/io/platform/f3xx/DACf3xx.hpp>
#elif defined(STM32F4xx)
    #include <core/io/platform/f4xx/ADCf4xx.hpp>
    #include <core/io/platform/f4xx/DACf4xx.hpp>
#endif

namespace io   = core::io;
namespace time = core::time;
namespace log  = core::log;

constexpr float TOLERANCE = 0.1f; // 100mV tolerance for ADC readings

bool testDACInitialization(io::DACBase& dac) {
    // Test 1: Verify DAC starts at zero
    float initialVoltage = dac.getVoltage();
    bool passed          = (initialVoltage < 0.1f); // Should be near 0V
    log::LOGGER.log(log::Logger::LogLevel::INFO, "DAC Init Test: %s (%.2fV)", passed ? "PASS" : "FAIL", initialVoltage);
    return passed;
}

bool testDACSetValue(io::DACBase& dac, uint32_t value, float expectedVoltage) {
    dac.setValue(value);
    float actualVoltage = dac.getVoltage();
    float difference =
        (actualVoltage > expectedVoltage) ? (actualVoltage - expectedVoltage) : (expectedVoltage - actualVoltage);
    bool passed = (difference < TOLERANCE);
    log::LOGGER.log(log::Logger::LogLevel::INFO,
                    "DAC setValue(%d): %s (Expected: %.2fV, Actual: %.2fV, Diff: %.2fV)",
                    value,
                    passed ? "PASS" : "FAIL",
                    expectedVoltage,
                    actualVoltage,
                    difference);
    return passed;
}

bool testDACSetVoltage(io::DACBase& dac, float voltage) {
    dac.setVoltage(voltage);
    float actualVoltage = dac.getVoltage();
    float difference    = (actualVoltage > voltage) ? (actualVoltage - voltage) : (voltage - actualVoltage);
    bool passed         = (difference < TOLERANCE);
    log::LOGGER.log(log::Logger::LogLevel::INFO,
                    "DAC setVoltage(%.2fV): %s (Actual: %.2fV, Diff: %.2fV)",
                    voltage,
                    passed ? "PASS" : "FAIL",
                    actualVoltage,
                    difference);
    return passed;
}

bool testADCReading(io::ADC& adc, float expectedVoltage) {
    float adcVoltage = adc.read();
    float difference = (adcVoltage > expectedVoltage) ? (adcVoltage - expectedVoltage) : (expectedVoltage - adcVoltage);
    bool passed      = (difference < TOLERANCE);
    log::LOGGER.log(log::Logger::LogLevel::INFO,
                    "ADC Reading: %s (Expected: %.2fV, Actual: %.2fV, Diff: %.2fV)",
                    passed ? "PASS" : "FAIL",
                    expectedVoltage,
                    adcVoltage,
                    difference);
    return passed;
}

bool testDALoopback(io::DACBase& dac, io::ADC& adc, float testVoltage) {
    log::LOGGER.log(log::Logger::LogLevel::INFO, "DAC-ADC Loopback Test: %.2fV", testVoltage);
    dac.setVoltage(testVoltage);
    time::wait(10); // Allow settling time
    return testADCReading(adc, testVoltage);
}

int main() {
    core::platform::init();

    // Initialize UART for logging
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // Set up the logger
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::INFO);

    uart.printf("Starting DAC Comprehensive Test\r\n");
    uart.printf("Hardware: DAC_OUT(PA4) -> ADC_IN(PA0)\r\n");
    uart.printf("Tolerance: %.1fV\r\n\r\n", TOLERANCE);

    time::wait(500);

    // Initialize DAC and ADC
#ifdef STM32F3xx
    io::DACf3xx dac(io::Pin::PA_4, io::DACPeriph::ONE);
    io::ADCf3xx adc(io::Pin::PA_0, io::ADCPeriph::ONE);
#elif defined(STM32F4xx)
    io::DACf4xx dac(io::Pin::PA_4, io::DACPeriph::ONE);
    io::ADCf4xx adc(io::Pin::PA_0, io::ADCPeriph::ONE);
#endif

    // Test 1: DAC Initialization
    log::LOGGER.log(log::Logger::LogLevel::INFO, "--- Test 1: DAC Initialization ---");
    testDACInitialization(dac);

    // Test 2: DAC setValue() functionality
    log::LOGGER.log(log::Logger::LogLevel::INFO, "--- Test 2: DAC setValue() Functionality ---");
    testDACSetValue(dac, 0, 0.0f);
    testDACSetValue(dac, 1024, 0.82f); // ~0.82V
    testDACSetValue(dac, 2048, 1.65f); // ~1.65V
    testDACSetValue(dac, 3072, 2.47f); // ~2.47V
    testDACSetValue(dac, 4095, 3.3f);  // ~3.3V

    // Test 3: DAC setVoltage() functionality
    log::LOGGER.log(log::Logger::LogLevel::INFO, "--- Test 3: DAC setVoltage() Functionality ---");
    testDACSetVoltage(dac, 0.5f);
    testDACSetVoltage(dac, 1.0f);
    testDACSetVoltage(dac, 2.0f);
    testDACSetVoltage(dac, 3.0f);

    // Test 4: Input validation (should clamp to max)
    log::LOGGER.log(log::Logger::LogLevel::INFO, "--- Test 4: Input Validation ---");
    dac.setValue(5000); // Should clamp to 4095
    log::LOGGER.log(log::Logger::LogLevel::INFO, "DAC setValue(5000): Clamped to %d", dac.getValue());
    dac.setVoltage(5.0f); // Should clamp to 3.3V
    log::LOGGER.log(log::Logger::LogLevel::INFO, "DAC setVoltage(5.0V): Clamped to %.2fV", dac.getVoltage());

    // Test 5: DAC-ADC Integration
    log::LOGGER.log(log::Logger::LogLevel::INFO, "--- Test 5: DAC-ADC Integration ---");
    testDALoopback(dac, adc, 0.5f);
    testDALoopback(dac, adc, 1.5f);
    testDALoopback(dac, adc, 2.5f);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Starting Triangle Wave Pattern (Visual Test)");
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Connect oscilloscope to PA4 for waveform observation");

    // Test 6: Triangle wave pattern (visual test)
    uint32_t value       = 0;
    bool increasing      = true;
    uint32_t cycle_count = 0;

    while (1) {
        // Set DAC value
        dac.setValue(value);

        // Read back with ADC (for validation)
        float adcVoltage = adc.read();

        // Log every 100 cycles (every ~1 second)
        if (cycle_count % 100 == 0) {
            log::LOGGER.log(log::Logger::LogLevel::INFO,
                            "Triangle Wave: DAC=%d (%.2fV), ADC=%.2fV",
                            value,
                            dac.getVoltage(),
                            adcVoltage);
        }

        // Update triangle wave
        if (increasing) {
            value += 50;
            if (value >= 4000) {
                increasing = false;
                log::LOGGER.log(log::Logger::LogLevel::INFO, "Triangle Wave: Peak reached, reversing");
            }
        } else {
            value -= 50;
            if (value <= 50) {
                increasing = true;
                log::LOGGER.log(log::Logger::LogLevel::INFO, "Triangle Wave: Valley reached, reversing");
            }
        }

        cycle_count++;
        time::wait(10);
    }

    return 0;
}
