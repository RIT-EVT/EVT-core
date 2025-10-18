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
constexpr int VOLTAGE_TO_MILLIVOLTS = 1000; // Conversion factor from volts to millivolts
constexpr int SETTLING_TIME_MS = 10; // Time to wait for DAC/ADC settling in milliseconds
constexpr int TRIANGLE_STEP_SIZE = 50; // Step size for triangle wave generation
constexpr int TRIANGLE_LOG_INTERVAL = 100; // Log every N cycles for triangle wave
constexpr int TRIANGLE_MAX_VALUE = 4000; // Maximum value for triangle wave (below DAC max)
constexpr float VOLTAGE_CLAMP_MIN = 3.29f; // Minimum expected voltage after clamping
constexpr float VOLTAGE_CLAMP_MAX = 3.31f; // Maximum expected voltage after clamping
constexpr uint32_t DAC_MAX_VALUE = 4095; // Maximum 12-bit DAC value
constexpr uint32_t TEST_OVERFLOW_VALUE = 5000; // Test value that exceeds DAC maximum
constexpr float TEST_OVERFLOW_VOLTAGE = 5.0f; // Test voltage that exceeds DAC maximum
constexpr float TEST_VOLTAGE_1 = 0.5f; // Test voltage 1 for loopback testing
constexpr float TEST_VOLTAGE_2 = 1.5f; // Test voltage 2 for loopback testing
constexpr float TEST_VOLTAGE_3 = 2.5f; // Test voltage 3 for loopback testing
constexpr float TEST_VOLTAGE_4 = 1.0f; // Test voltage 4 for setVoltage testing
constexpr float TEST_VOLTAGE_5 = 2.0f; // Test voltage 5 for setVoltage testing
constexpr float TEST_VOLTAGE_6 = 3.0f; // Test voltage 6 for setVoltage testing
constexpr uint32_t TEST_VALUE_1 = 2048; // Test value 1 for setValue testing (~1.65V)
constexpr uint32_t TEST_VALUE_2 = 3072; // Test value 2 for setValue testing (~2.47V)
constexpr float EXPECTED_VOLTAGE_1 = 1.65f; // Expected voltage for TEST_VALUE_1
constexpr float EXPECTED_VOLTAGE_2 = 2.47f; // Expected voltage for TEST_VALUE_2
constexpr float EXPECTED_VOLTAGE_3 = 3.3f; // Expected voltage for DAC_MAX_VALUE
constexpr uint32_t TEST_VALUE_0 = 0; // Test value 0 for setValue testing (0V)
constexpr uint32_t TEST_VALUE_3 = 1024; // Test value 3 for setValue testing (~0.82V)
constexpr float EXPECTED_VOLTAGE_0 = 0.0f; // Expected voltage for TEST_VALUE_0
constexpr float EXPECTED_VOLTAGE_4 = 0.82f; // Expected voltage for TEST_VALUE_3
constexpr uint32_t UART_BAUD_RATE = 9600; // UART baud rate for logging

/**
 * Test DAC initialization by checking if it starts at zero voltage
 * @param dac Reference to the DAC instance to test
 * @param uart Reference to UART for logging output
 * @return true if DAC initializes correctly (near 0V), false otherwise
 */
bool testDACInitialization(io::DigitalToAnalogConverter& dac, io::UART& uart) {
    // Test 1: Verify DAC starts at zero
    float initialVoltage = dac.getVoltage();
    bool passed          = (initialVoltage < TOLERANCE); // Should be near 0V
    uart.printf("DAC Init Test: %s (%dmV)\r\n", passed ? "PASS" : "FAIL", (int) (initialVoltage * VOLTAGE_TO_MILLIVOLTS));
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
bool testDACSetValue(io::DigitalToAnalogConverter& dac, uint32_t value, float expectedVoltage, io::UART& uart) {
    dac.setValue(value);
    float actualVoltage = dac.getVoltage();
    float difference =
        (actualVoltage > expectedVoltage) ? (actualVoltage - expectedVoltage) : (expectedVoltage - actualVoltage);
    bool passed = (difference < TOLERANCE);
    uart.printf("DAC setValue(%d): %s (Expected: %dmV, Actual: %dmV, Diff: %dmV)\r\n",
                value,
                passed ? "PASS" : "FAIL",
                (int) (expectedVoltage * VOLTAGE_TO_MILLIVOLTS),
                (int) (actualVoltage * VOLTAGE_TO_MILLIVOLTS),
                (int) (difference * VOLTAGE_TO_MILLIVOLTS));
    return passed;
}

/**
 * Test DAC setVoltage functionality by setting a voltage and verifying the output
 * @param dac Reference to the DAC instance to test
 * @param voltage Desired output voltage in volts
 * @param uart Reference to UART for logging output
 * @return true if actual voltage matches desired within tolerance, false otherwise
 */
bool testDACSetVoltage(io::DigitalToAnalogConverter& dac, float voltage, io::UART& uart) {
    dac.setVoltage(voltage);
    float actualVoltage = dac.getVoltage();
    float difference    = (actualVoltage > voltage) ? (actualVoltage - voltage) : (voltage - actualVoltage);
    bool passed         = (difference < TOLERANCE);
    uart.printf("DAC setVoltage(%dmV): %s (Actual: %dmV, Diff: %dmV)\r\n",
                (int) (voltage * VOLTAGE_TO_MILLIVOLTS),
                passed ? "PASS" : "FAIL",
                (int) (actualVoltage * VOLTAGE_TO_MILLIVOLTS),
                (int) (difference * VOLTAGE_TO_MILLIVOLTS));
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
                (int) (expectedVoltage * VOLTAGE_TO_MILLIVOLTS),
                (int) (adcVoltage * VOLTAGE_TO_MILLIVOLTS),
                (int) (difference * VOLTAGE_TO_MILLIVOLTS));
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
bool testDALoopback(io::DigitalToAnalogConverter& dac, io::ADC& adc, float testVoltage, io::UART& uart) {
    uart.printf("DAC-ADC Loopback Test: %dmV\r\n", (int) (testVoltage * VOLTAGE_TO_MILLIVOLTS));
    dac.setVoltage(testVoltage);
    time::wait(SETTLING_TIME_MS); // Allow settling time
    return testADCReading(adc, testVoltage, uart);
}

int main() {
    core::platform::init();

    // Initialize UART for logging
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(UART_BAUD_RATE);

    uart.printf("Starting DAC Comprehensive Test\r\n");
    uart.printf("Hardware: DAC_OUT(PA4) -> ADC_IN(PA0)\r\n");
    uart.printf("Tolerance: %dmV\r\n\r\n", (int) (TOLERANCE * VOLTAGE_TO_MILLIVOLTS));

    // Initialize DAC and ADC using manager methods
    io::DigitalToAnalogConverter& dac = io::getDAC<io::Pin::PA_4, io::DACPeriph::ONE>();
    io::ADC& adc = io::getADC<io::Pin::PA_0, io::ADCPeriph::ONE>();

    // Test 1: DAC Initialization
    uart.printf("--- Test 1: DAC Initialization ---\r\n");
    testDACInitialization(dac, uart);

    // Test 2: DAC setValue() functionality
    uart.printf("--- Test 2: DAC setValue() Functionality ---\r\n");
    testDACSetValue(dac, TEST_VALUE_0, EXPECTED_VOLTAGE_0, uart);
    testDACSetValue(dac, TEST_VALUE_3, EXPECTED_VOLTAGE_4, uart); // ~0.82V
    testDACSetValue(dac, TEST_VALUE_1, EXPECTED_VOLTAGE_1, uart); // ~1.65V
    testDACSetValue(dac, TEST_VALUE_2, EXPECTED_VOLTAGE_2, uart); // ~2.47V
    testDACSetValue(dac, DAC_MAX_VALUE, EXPECTED_VOLTAGE_3, uart);  // ~3.3V

    // Test 3: DAC setVoltage() functionality
    uart.printf("--- Test 3: DAC setVoltage() Functionality ---\r\n");
    testDACSetVoltage(dac, TEST_VOLTAGE_1, uart);
    testDACSetVoltage(dac, TEST_VOLTAGE_4, uart);
    testDACSetVoltage(dac, TEST_VOLTAGE_5, uart);
    testDACSetVoltage(dac, TEST_VOLTAGE_6, uart);

    // Test 4: Input validation (should clamp to max)
    uart.printf("--- Test 4: Input Validation ---\r\n");
    dac.setValue(TEST_OVERFLOW_VALUE); // Should clamp to DAC_MAX_VALUE
    bool valueClampTest = (dac.getValue() == DAC_MAX_VALUE);
    uart.printf("DAC setValue(%d): %s (Clamped to %d)\r\n",
                TEST_OVERFLOW_VALUE,
                valueClampTest ? "PASS" : "FAIL", dac.getValue());
    
    dac.setVoltage(TEST_OVERFLOW_VOLTAGE); // Should clamp to 3.3V
    bool voltageClampTest = (dac.getVoltage() >= VOLTAGE_CLAMP_MIN && dac.getVoltage() <= VOLTAGE_CLAMP_MAX);
    uart.printf("DAC setVoltage(%.1fV): %s (Clamped to %dmV)\r\n",
                TEST_OVERFLOW_VOLTAGE,
                voltageClampTest ? "PASS" : "FAIL", (int) (dac.getVoltage() * VOLTAGE_TO_MILLIVOLTS));

    // Test 5: DAC-ADC Integration
    uart.printf("--- Test 5: DAC-ADC Integration ---\r\n");
    testDALoopback(dac, adc, TEST_VOLTAGE_1, uart);
    testDALoopback(dac, adc, TEST_VOLTAGE_2, uart);
    testDALoopback(dac, adc, TEST_VOLTAGE_3, uart);

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

        // Log every N cycles (every ~1 second)
        if (cycle_count % TRIANGLE_LOG_INTERVAL == 0) {
            uart.printf("Triangle Wave: DAC=%d (%dmV), ADC=%dmV\r\n",
                        value,
                        (int) (dac.getVoltage() * VOLTAGE_TO_MILLIVOLTS),
                        (int) (adcVoltage * VOLTAGE_TO_MILLIVOLTS));
        }

        // Update triangle wave
        if (increasing) {
            value += TRIANGLE_STEP_SIZE;
            if (value >= TRIANGLE_MAX_VALUE) {
                increasing = false;
                uart.printf("Triangle Wave: Peak reached, reversing\r\n");
            }
        } else {
            value -= TRIANGLE_STEP_SIZE;
            if (value <= TRIANGLE_STEP_SIZE) {
                increasing = true;
                uart.printf("Triangle Wave: Valley reached, reversing\r\n");
            }
        }

        cycle_count++;
        time::wait(SETTLING_TIME_MS);
    }

    return 0;
}
