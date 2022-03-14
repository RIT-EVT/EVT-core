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
    // Initialize system
    IO::init();

    IO::PWM& pwm = IO::getPWM<IO::Pin::PC_0>();
    // 1 second in microseconds period
    pwm.setPeriod(1000000);
    // 50 % duty cycle
    pwm.setDutyCycle(0.5);
    while (1) {
        time::wait(10);
    }
}
