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
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

constexpr float TOLERANCE = 0.1f; // 100mV tolerance for ADC readings

/**
 * Test DAC initialization by checking if it starts at zero voltage
 * @param dac Reference to the DAC instance to test
 * @param uart Reference to UART for logging output
 * @return true if DAC initializes correctly (near 0V), false otherwise
 */
bool testDACInitialization(io::DACBase& dac, io::UART& uart) {
    // Test 1: Verify DAC starts at zero
    float initialVoltage = dac.getVoltage();
    bool passed          = (initialVoltage < 0.1f); // Should be near 0V
    uart.printf("DAC Init Test: %s (%dmV)\r\n", passed ? "PASS" : "FAIL", (int) (initialVoltage * 1000));
    return passed;
}

/**
 * Test DAC setValue functionality by setting a raw value and verifying the output voltage
 * @param dac Reference to the DAC instance to test
 * @param value Raw DAC value to set (0-4095)
 * @param expectedVoltage Expected output voltage in volts
 * @param uart Reference to UART for logging output
 * @return true if actual voltage matches expected within tolerance, false otherwise
 */
bool testDACSetValue(io::DACBase& dac, uint32_t value, float expectedVoltage, io::UART& uart) {
    dac.setValue(value);
    float actualVoltage = dac.getVoltage();
    float difference =
        (actualVoltage > expectedVoltage) ? (actualVoltage - expectedVoltage) : (expectedVoltage - actualVoltage);
    bool passed = (difference < TOLERANCE);
    uart.printf("DAC setValue(%d): %s (Expected: %dmV, Actual: %dmV, Diff: %dmV)\r\n",
                value,
                passed ? "PASS" : "FAIL",
                (int) (expectedVoltage * 1000),
                (int) (actualVoltage * 1000),
                (int) (difference * 1000));
    return passed;
}

/**
 * Test DAC setVoltage functionality by setting a voltage and verifying the output
 * @param dac Reference to the DAC instance to test
 * @param voltage Desired output voltage in volts
 * @param uart Reference to UART for logging output
 * @return true if actual voltage matches desired within tolerance, false otherwise
 */
bool testDACSetVoltage(io::DACBase& dac, float voltage, io::UART& uart) {
    dac.setVoltage(voltage);
    float actualVoltage = dac.getVoltage();
    float difference    = (actualVoltage > voltage) ? (actualVoltage - voltage) : (voltage - actualVoltage);
    bool passed         = (difference < TOLERANCE);
    uart.printf("DAC setVoltage(%dmV): %s (Actual: %dmV, Diff: %dmV)\r\n",
                (int) (voltage * 1000),
                passed ? "PASS" : "FAIL",
                (int) (actualVoltage * 1000),
                (int) (difference * 1000));
    return passed;
}

/**
 * Test ADC reading functionality by comparing with expected voltage
 * @param adc Reference to the ADC instance to test
 * @param expectedVoltage Expected voltage reading in volts
 * @param uart Reference to UART for logging output
 * @return true if ADC reading matches expected within tolerance, false otherwise
 */
bool testADCReading(io::ADC& adc, float expectedVoltage, io::UART& uart) {
    float adcVoltage = adc.read();
    float difference = (adcVoltage > expectedVoltage) ? (adcVoltage - expectedVoltage) : (expectedVoltage - adcVoltage);
    bool passed      = (difference < TOLERANCE);
    uart.printf("ADC Reading: %s (Expected: %dmV, Actual: %dmV, Diff: %dmV)\r\n",
                passed ? "PASS" : "FAIL",
                (int) (expectedVoltage * 1000),
                (int) (adcVoltage * 1000),
                (int) (difference * 1000));
    return passed;
}

/**
 * Test DAC-ADC loopback by setting DAC voltage and reading it back with ADC
 * @param dac Reference to the DAC instance to test
 * @param adc Reference to the ADC instance to test
 * @param testVoltage Test voltage to output in volts
 * @param uart Reference to UART for logging output
 * @return true if ADC reading matches DAC output within tolerance, false otherwise
 */
bool testDALoopback(io::DACBase& dac, io::ADC& adc, float testVoltage, io::UART& uart) {
    uart.printf("DAC-ADC Loopback Test: %dmV\r\n", (int) (testVoltage * 1000));
    dac.setVoltage(testVoltage);
    time::wait(10); // Allow settling time
    return testADCReading(adc, testVoltage, uart);
}

int main() {
    core::platform::init();

    // Initialize UART for logging
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    uart.printf("Starting DAC Comprehensive Test\r\n");
    uart.printf("Hardware: DAC_OUT(PA4) -> ADC_IN(PA0)\r\n");
    uart.printf("Tolerance: %dmV\r\n\r\n", (int) (TOLERANCE * 1000));

    // Initialize DAC and ADC using manager methods
    io::DAC& dac = io::getDAC<io::Pin::PA_4, io::DACPeriph::ONE>();
    io::ADC& adc = io::getADC<io::Pin::PA_0, io::ADCPeriph::ONE>();

    // Test 1: DAC Initialization
    uart.printf("--- Test 1: DAC Initialization ---\r\n");
    testDACInitialization(dac, uart);

    // Test 2: DAC setValue() functionality
    uart.printf("--- Test 2: DAC setValue() Functionality ---\r\n");
    testDACSetValue(dac, 0, 0.0f, uart);
    testDACSetValue(dac, 1024, 0.82f, uart); // ~0.82V
    testDACSetValue(dac, 2048, 1.65f, uart); // ~1.65V
    testDACSetValue(dac, 3072, 2.47f, uart); // ~2.47V
    testDACSetValue(dac, 4095, 3.3f, uart);  // ~3.3V

    // Test 3: DAC setVoltage() functionality
    uart.printf("--- Test 3: DAC setVoltage() Functionality ---\r\n");
    testDACSetVoltage(dac, 0.5f, uart);
    testDACSetVoltage(dac, 1.0f, uart);
    testDACSetVoltage(dac, 2.0f, uart);
    testDACSetVoltage(dac, 3.0f, uart);

    // Test 4: Input validation (should clamp to max)
    uart.printf("--- Test 4: Input Validation ---\r\n");
    dac.setValue(5000); // Should clamp to 4095
    bool valueClampTest = (dac.getValue() == 4095);
    uart.printf("DAC setValue(5000): %s (Clamped to %d)\r\n", 
                valueClampTest ? "PASS" : "FAIL", dac.getValue());
    
    dac.setVoltage(5.0f); // Should clamp to 3.3V
    bool voltageClampTest = (dac.getVoltage() >= 3.29f && dac.getVoltage() <= 3.31f);
    uart.printf("DAC setVoltage(5.0V): %s (Clamped to %dmV)\r\n", 
                voltageClampTest ? "PASS" : "FAIL", (int) (dac.getVoltage() * 1000));

    // Test 5: DAC-ADC Integration
    uart.printf("--- Test 5: DAC-ADC Integration ---\r\n");
    testDALoopback(dac, adc, 0.5f, uart);
    testDALoopback(dac, adc, 1.5f, uart);
    testDALoopback(dac, adc, 2.5f, uart);

    uart.printf("Starting Triangle Wave Pattern (Visual Test)\r\n");
    uart.printf("Connect oscilloscope to PA4 for waveform observation\r\n");

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
            uart.printf("Triangle Wave: DAC=%d (%dmV), ADC=%dmV\r\n",
                        value,
                        (int) (dac.getVoltage() * 1000),
                        (int) (adcVoltage * 1000));
        }

        // Update triangle wave
        if (increasing) {
            value += 50;
            if (value >= 4000) {
                increasing = false;
                uart.printf("Triangle Wave: Peak reached, reversing\r\n");
            }
        } else {
            value -= 50;
            if (value <= 50) {
                increasing = true;
                uart.printf("Triangle Wave: Valley reached, reversing\r\n");
            }
        }

        cycle_count++;
        time::wait(10);
    }

    return 0;
}
