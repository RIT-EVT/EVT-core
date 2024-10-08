#include <core/io/platform/f3xx/GPIOf3xx.hpp>

#include <stdint.h>

#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>

#include <HALf3/stm32f3xx_hal_gpio.h>
#include <HALf3/stm32f3xx_hal_rcc.h>
#include <core/platform/f3xx/stm32f3xx.hpp>

void (*INTERRUPT_HANDLERS[16])(core::io::GPIO* pin, void* priv) = {nullptr};
core::io::GPIO* INTERRUPT_GPIOS[16]                             = {nullptr};
void* INTERRUPT_ARGS[16]                                        = {nullptr};

extern "C" void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
extern "C" void EXTI1_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
extern "C" void EXTI2_TSC_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
extern "C" void EXTI3_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}
extern "C" void EXTI4_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
extern "C" void EXTI9_5_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

extern "C" void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    uint16_t tmpPin = GPIO_Pin;
    uint8_t count   = 0;

    while ((tmpPin & 0x0001) == 0) {
        tmpPin = tmpPin >> 1;
        count++;
    }
    if (nullptr != INTERRUPT_HANDLERS[count]) {
        INTERRUPT_HANDLERS[count](INTERRUPT_GPIOS[count], INTERRUPT_ARGS[count]);
    }
}

namespace core::io {

GPIOf3xx::GPIOf3xx(Pin pin, GPIO::Direction direction, Pull pull) : GPIO(pin, direction, pull) {

    GPIO_InitTypeDef gpioInit;
    Pin myPins[]      = {pin};
    uint8_t numOfPins = 1;

    gpioStateInit(&gpioInit,
                  myPins,
                  numOfPins,
                  static_cast<uint32_t>(direction),
                  static_cast<uint32_t>(pull),
                  GPIO_SPEED_FREQ_HIGH);

    switch ((static_cast<uint8_t>(pin) & 0xF0) >> 4) {
    case 0x0:
        this->port = GPIOA;
        break;
    case 0x1:
        this->port = GPIOB;
        break;
    case 0x2:
        this->port = GPIOC;
        break;
    case 0x3:
        this->port = GPIOD;
        break;
    case 0x5:
        this->port = GPIOF;
        break;
    default:
        break; // Should never get here
    }

    this->halPin = 1 << (static_cast<uint16_t>(this->pin) & 0x0F);
    this->writePin(GPIO::State::LOW); // Output set low by default
}

void GPIOf3xx::setDirection(GPIO::Direction direction) {
    // TODO: Add implementation of resetting the direction
}

void GPIOf3xx::writePin(GPIO::State state) {
    HAL_GPIO_WritePin(this->port, this->halPin, static_cast<GPIO_PinState>(state));
}

GPIO::State GPIOf3xx::readPin() {
    return static_cast<GPIO::State>(HAL_GPIO_ReadPin(this->port, this->halPin));
}

void GPIOf3xx::registerIRQ(TriggerEdge edge, void (*irqHandler)(GPIO* pin, void* priv), void* priv) {
    GPIO_InitTypeDef gpioInit;
    Pin myPins[]      = {pin};
    uint8_t numOfPins = 1;

    gpioStateInit(&gpioInit,
                  myPins,
                  numOfPins,
                  GPIOf3xx::GPIO_TRIGGER_INTERRUPT_BASE | (static_cast<uint32_t>(edge) << GPIO_MODE_IT_SHIFT),
                  GPIO_PULLDOWN,
                  GPIO_SPEED_FREQ_HIGH);

    auto pin_index                = static_cast<uint8_t>(this->pin) & 0x0F;
    INTERRUPT_HANDLERS[pin_index] = irqHandler;
    INTERRUPT_GPIOS[pin_index]    = this;
    INTERRUPT_ARGS[pin_index]     = priv;
    IRQn_Type irqNum;

    switch (this->halPin) {
    case GPIO_PIN_0:
        irqNum = EXTI0_IRQn;
        break;
    case GPIO_PIN_1:
        irqNum = EXTI1_IRQn;
        break;
    case GPIO_PIN_2:
        irqNum = EXTI2_TSC_IRQn;
        break;
    case GPIO_PIN_3:
        irqNum = EXTI3_IRQn;
        break;
    case GPIO_PIN_4:
        irqNum = EXTI4_IRQn;
        break;
    case GPIO_PIN_5:
    case GPIO_PIN_6:
    case GPIO_PIN_7:
    case GPIO_PIN_8:
    case GPIO_PIN_9:
        irqNum = EXTI9_5_IRQn;
        break;
    case GPIO_PIN_10:
    case GPIO_PIN_11:
    case GPIO_PIN_12:
    case GPIO_PIN_13:
    case GPIO_PIN_14:
    case GPIO_PIN_15:
        irqNum = EXTI15_10_IRQn;
        break;

    default:
        return; // Shouldn't get here
    }

    HAL_NVIC_SetPriority(irqNum, core::platform::GPIO_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(irqNum);
}

void GPIOf3xx::gpioStateInit(GPIO_InitTypeDef* targetGpio, Pin* pins, uint8_t numOfPins, uint32_t mode, uint32_t pull,
                             uint32_t speed, uint8_t alternate) {
    if (numOfPins == 2) {
        targetGpio->Pin = static_cast<uint32_t>(1 << (static_cast<uint32_t>(pins[0]) & 0x0F))
            | static_cast<uint32_t>(1 << (static_cast<uint32_t>(pins[1]) & 0x0F));
    } else {
        targetGpio->Pin = static_cast<uint32_t>(1 << (static_cast<uint32_t>(pins[0]) & 0x0F));
    }

    targetGpio->Mode = mode;

    targetGpio->Pull = pull;

    targetGpio->Speed = speed;

    // 0x0DU is not used by the F302x8 as an alternate value
    if (alternate != 0x0DU) {
        targetGpio->Alternate = alternate;
    }

    for (uint8_t i = 0; i < numOfPins; i++) {
        switch ((static_cast<uint8_t>(pins[i]) & 0xF0) >> 4) {
        case 0x0:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            HAL_GPIO_Init(GPIOA, targetGpio);
            break;
        case 0x1:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            HAL_GPIO_Init(GPIOB, targetGpio);
            break;
        case 0x2:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            HAL_GPIO_Init(GPIOC, targetGpio);
            break;
        case 0x3:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            HAL_GPIO_Init(GPIOD, targetGpio);
            break;
        case 0x5:
            __HAL_RCC_GPIOF_CLK_ENABLE();
            HAL_GPIO_Init(GPIOF, targetGpio);
            break;
        default:
            break; // Should never get here
        }
    }
}
} // namespace core::io
