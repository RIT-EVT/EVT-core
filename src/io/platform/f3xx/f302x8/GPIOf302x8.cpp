#include <cstdint>

#include <EVT/io/GPIO.hpp>
#include <EVT/io/platform/f3xx/f302x8/GPIOf302x8.hpp>
#include <EVT/io/pin.hpp>

#include <HALf3/stm32f3xx_hal_rcc.h>
#include <HALf3/stm32f3xx_hal_gpio.h>
#include <HALf3/stm32f3xx_hal_flash.h>

namespace
{
    void (*INTERRUPT_HANDLERS[16])() = {0};
}

extern "C" void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
extern "C" void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
extern "C" void EXTI2_TSC_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
extern "C" void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}
extern "C" void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
extern "C" void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

extern "C" void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint16_t tmpPin = GPIO_Pin;
    uint8_t count = 0;

    while((tmpPin & 0x0001) == 0)
    {
        tmpPin = tmpPin >> 1;
        count++;
    }
    if (nullptr != INTERRUPT_HANDLERS[count])
    {
        INTERRUPT_HANDLERS[count]();
    }
}

namespace EVT::core::IO
{

GPIOf302x8::GPIOf302x8(Pin pin, GPIO::Direction direction) : GPIO(pin, direction)
{

    GPIO_InitTypeDef gpioInit;

    this->halPin = 1 << (static_cast<uint16_t>(this->pin) & 0x0F);
    switch((static_cast<uint8_t>(pin) & 0xF0) >> 4)
    {
        case 0x0:
            this->port = GPIOA;
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case 0x1:
            this->port = GPIOB;
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case 0x2:
            this->port = GPIOC;
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case 0x3:
            this->port = GPIOD;
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        case 0x5:
            this->port = GPIOF;
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        default:
            break; // Should never get here
    }

    gpioInit.Pin = this->halPin;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(this->port, &gpioInit);
    this->writePin(GPIO::State::LOW); // Output set low by default
}

void GPIOf302x8::setDirection(GPIO::Direction direction)
{
    // TODO: Add implementation of resetting the direction
}

void GPIOf302x8::writePin(GPIO::State state)
{
    HAL_GPIO_WritePin(this->port, this->halPin, static_cast<GPIO_PinState>(state));
}

GPIO::State GPIOf302x8::readPin()
{
    return static_cast<GPIO::State>(HAL_GPIO_ReadPin(this->port, this->halPin));
}

}
