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

    IO::PWM& pwm = IO::getPWM<IO::Pin::PA_3>();
    pwm.setPeriod(100);

    while(1) {
        time::wait(100);
    }
}
