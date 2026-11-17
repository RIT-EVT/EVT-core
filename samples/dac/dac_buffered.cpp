/**
 * Plays a 1 kHz triangle wave on PA4 from a buffer, using DMA.
 * A callback counts completed wave cycles, which are printed once a second.
 *
 * To see it working, connect an oscilloscope to PA4.
 */

#include <core/io/DAC.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

constexpr uint16_t WAVE_SAMPLES   = 100;
constexpr uint32_t WAVE_FREQUENCY = 1000;
constexpr uint32_t SAMPLE_RATE    = WAVE_SAMPLES * WAVE_FREQUENCY;

/// One period of a triangle wave spanning the full DAC range
uint16_t wave[WAVE_SAMPLES];

/// Incremented once per wave period, from interrupt context
volatile uint32_t cycles = 0;

void onDACEvent(io::DACBase::DACEvent event, void* _priv) {
    if (event == io::DACBase::DACEvent::COMPLETE) {
        cycles = cycles + 1;
    }
}

int main() {
    // Initialize system
    core::platform::init();

    io::UART& uart   = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    io::DACBase& dac = io::getDAC<io::Pin::PA_4>();

    // Attach our custom callback for buffered mode
    dac.setCallback(onDACEvent);

    // Populate the sample output buffer
    for (uint16_t i = 0; i < WAVE_SAMPLES / 2; i++) {
        wave[i]                    = i * io::DACBase::MAX_RAW / (WAVE_SAMPLES / 2);
        wave[WAVE_SAMPLES - 1 - i] = wave[i];
    }

    uart.printf("\r\nDAC buffered output: %lu Hz triangle wave on PA4\r\n", WAVE_FREQUENCY);
    if (dac.startBuffer(wave, WAVE_SAMPLES, SAMPLE_RATE) != io::DACBase::DACStatus::OK) {
        uart.printf("startBuffer failed\r\n");
        while (true) {}
    }

    while (true) {
        time::wait(1000);
        uart.printf("wave cycles: %lu\r\n", cycles);
    }
}
