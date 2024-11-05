
#include <core/io/PWMINPUT.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;


TIM_HandleTypeDef htim2;


uint32_t ICValue = 0;
uint32_t Frequency = 0;
uint32_t Duty = 0;


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // If the interrupt is triggered by channel 1
    {
        // Read the IC value
        ICValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

        if (ICValue != 0)	//first value is zero, non zero is period
        {
            // calculate the Duty Cycle
            //signal high time/ period = duty cycle
            Duty = (HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) *100)/ICValue;

            Frequency = SystemCoreClock/ICValue; //APB1 Timer clocks/period = F
        }
    }
}

int main(void){
    // Initialize system
    core::platform::init();
    //HAL_Init();
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    //input capture
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //main channel
    HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_2); 	//indirect channel

    while (1)
    {
        HAL_Delay(1000);
        uart.printf("\n\rICValue: %s\n\r", ICValue);
        uart.printf("\n\rFrequency: %s\n\r", Frequency);
        uart.printf("\n\rDuty: %s\n\r", Duty);
        uart.printf("\n\r----------------------------------\n\r");
    }

}

