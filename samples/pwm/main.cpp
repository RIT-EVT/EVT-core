/**
 * This example shows off a basic PWM signal. You will need to use a logic
 * analyzer to see the generated square wave.
 */
#include <EVT/io/PWM.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {

    IO::PWM& pwm = IO::getPWM<IO::Pin::PA_6>();
    pwm.setPeriod(65535);
    while(1) {
        for(uint16_t i = 0; i < 65535; i++) {
            time::wait(10);
        }
    }
}
