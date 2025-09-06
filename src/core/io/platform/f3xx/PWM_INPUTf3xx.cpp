
#include <core/io/platform/f3xx/GPIOf3xx.hpp>
#include <core/io/platform/f3xx/PWM_INPUTf3xx.hpp>

namespace core::io {


// Global pointer to the active instance (one at a time)
static PWM_INPUTf3xx* activePwmInput = nullptr;

/**
 * Get timer instance, direct channel, indirect channel, and alternate function
 * associated with a pin. This information is pulled from the STM32F302x8 documentation.
 * F302 pinout and alternate functions specifically for PWM_INPUT can be found in the drive.
 *
  @param pin The pin to check the instance of
* @param instance The timer instance to assign to
* @param directChannel The direct channel to assign to
* @param indirectChannel The indirect channel to assign to
* @param alternateFunction The GPIO identifier for the function of the pin
* @param irqNumber IRQ number for NVIC
* @param activeChannel HAL enum for which channel will fire the interrupt
 */

static void getInputInstance(Pin pin,
                             TIM_TypeDef** instance,
                             uint32_t* directChannel,
                             uint32_t* indirectChannel,
                             uint32_t* alternateFunction,
                             uint32_t* triggerSource,
                             IRQn_Type* irqNumber,
                             uint32_t* activeChannel) {
    switch (pin) {
#if defined(STM32F302x8)
    case Pin::PA_2:
        *instance = TIM1;
        *directChannel = TIM_CHANNEL_1;
        *indirectChannel = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF2_TIM1;
        *triggerSource = TIM_TS_TI1FP1;
        *irqNumber = TIM1_CC_IRQn;
        *activeChannel = HAL_TIM_ACTIVE_CHANNEL_1;
            break;
        // TODO: Add other pins like in PWMf3xx::getInstance
#endif
    default:
        *instance = NULL;
        *directChannel = -1;
        *indirectChannel = -1;
        *alternateFunction = -1;
        *triggerSource = -1;
        *irqNumber = (IRQn_Type)-1; //TODO fix this, find actual default
        *activeChannel = -1;

    }
}



PWM_INPUTf3xx::PWM_INPUTf3xx(Pin pin) : PWM_INPUT(pin) {
    TIM_TypeDef* instance;
    uint32_t alternateFunction;
    uint32_t triggerSource;
    IRQn_Type irqNumber;

    getInputInstance(pin, &instance, &directChannel, &indirectChannel, &alternateFunction, &triggerSource,
                     &irqNumber, &activeChannel);

    if (instance == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    } else if (instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    } else if (instance == TIM15) {
        __HAL_RCC_TIM15_CLK_ENABLE();
    }

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig       = {0};
    TIM_IC_InitTypeDef sConfigIC              = {0};
    TIM_MasterConfigTypeDef sMasterConfig     = {0};

    //initilize timer base
    halTIM.Instance               = instance;
    halTIM.Init.Prescaler         = 0;
    halTIM.Init.CounterMode       = TIM_COUNTERMODE_UP;
    halTIM.Init.Period            = 4294967295;
    halTIM.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    halTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&halTIM);

    //configure clock src
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_PWM_Init(&halTIM);
    HAL_TIM_ConfigClockSource(&halTIM, &sClockSourceConfig);
    HAL_TIM_IC_Init(&halTIM);

    //slave reset mode, used to measure period
    sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger     = triggerSource;
    sSlaveConfig.TriggerPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
    sSlaveConfig.TriggerFilter    = 0;
    HAL_TIM_SlaveConfigSynchro(&halTIM, &sSlaveConfig);

    //configure direct, rising channel
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    HAL_TIM_IC_ConfigChannel(&halTIM, &sConfigIC, directChannel);

    //configure indirect, falling channel
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    HAL_TIM_IC_ConfigChannel(&halTIM, &sConfigIC, indirectChannel);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&halTIM, &sMasterConfig);

    //setup GPIO
    GPIO_InitTypeDef gpioInit = {0};
    Pin myPins[] = { pin };
    uint8_t numOfPins = 1;

    GPIOf3xx::gpioStateInit(&gpioInit,myPins,numOfPins,GPIO_MODE_AF_PP,GPIO_NOPULL,
                            GPIO_SPEED_FREQ_LOW,alternateFunction);


    //configure NVIC
    HAL_NVIC_SetPriority(irqNumber, 0, 0);
    HAL_NVIC_EnableIRQ(irqNumber);
    // enable interrupt src
    HAL_TIM_IC_Start_IT(&halTIM, directChannel); // main channel
    HAL_TIM_IC_Start(&halTIM, indirectChannel);  // indirect channel

    activePwmInput = this; // Register this instance as active
}


void PWM_INPUTf3xx::handleCapture(TIM_HandleTypeDef* htim) {
    if (htim->Channel == activeChannel) // If the interrupt is triggered by channel 1
    {
        // Read the IC value
        uint32_t inputCaptureValue = HAL_TIM_ReadCapturedValue(htim, directChannel);

        if (inputCaptureValue != 0) // first value is zero, non zero is period
        {
            // calculate the Duty Cycle
            // signal high time/ period = duty cycle
            dutyCycle = (HAL_TIM_ReadCapturedValue(htim, indirectChannel) * 100) / inputCaptureValue;

            frequency = SystemCoreClock / inputCaptureValue; // APB1 Timer clocks/period = F
            period = inputCaptureValue;
        }
    }
}

// Global HAL callback -> forwards to the single active instance
extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    if (activePwmInput) {
        activePwmInput->handleCapture(htim);
    }
}


uint32_t PWM_INPUTf3xx::getDutyCycle() {
    return dutyCycle;
}

uint32_t PWM_INPUTf3xx::getPeriod() {
    return period;
}

uint32_t PWM_INPUTf3xx::getFrequency() {
    return frequency;
}







}
