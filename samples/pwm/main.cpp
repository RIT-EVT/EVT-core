/**
 * This example shows off a basic PWM signal. You will need to use a logic
 * analyzer to see the generated square wave.
 */
#include <core/io/PWM.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    io::PWM& pwm = io::getPWM<io::Pin::PA_8>();
    // 1000000 microseconds (1 second) period
    pwm.setPeriod(1000000);
    // 50 % duty cycle
    pwm.setDutyCycle(70);

    while (1) {
        time::wait(5000);
        pwm.setDutyCycle(70);
        time::wait(5000);
        pwm.setDutyCycle(30);
    }
}