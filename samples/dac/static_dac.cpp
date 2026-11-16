/**
 * Steps the DAC output on PA4 through a few voltages, holding each for a second.
 *
 * To see it working, measure PA4 with a multimeter or oscilloscope.
 */

#include <core/io/DAC.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

int main() {
    core::platform::init();

    io::UART& uart   = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    io::DACBase& dac = io::getDAC<io::Pin::PA_4>();

    const uint32_t testMillivolts[] = {0, 500, 1000, 1650, 2500, 3300};

    uart.printf("\r\nDAC static output on PA4\r\n");
    while (true) {
        for (uint32_t mv : testMillivolts) {
            dac.setVoltage(mv / 1000.0f);
            uart.printf("set %4lu mV  raw %4u\r\n", mv, dac.getValue());
            time::wait(1000);
        }
    }
}
