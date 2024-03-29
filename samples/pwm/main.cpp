/**
 * This example shows off a basic PWM signal. You will need to use a logic
 * analyzer to see the generated square wave.
 */
#include <EVT/io/PWM.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    EVT::core::platform::init();

    IO::PWM& pwm = IO::getPWM<IO::Pin::PA_8>();
    // 1000000 microseconds (1 second) period
    pwm.setPeriod(1000000);
    // 50 % duty cycle
    pwm.setDutyCycle(50);

    while (1) {
        time::wait(5000);
        pwm.setDutyCycle(70);
        time::wait(5000);
        pwm.setDutyCycle(30);
    }
}