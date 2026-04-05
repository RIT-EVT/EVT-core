#include <core/io/platform/f4xx/GPIOf4xx.hpp>

#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>

#include <HALf4/stm32f4xx_hal_gpio.h>
#include <HALf4/stm32f4xx_hal_rcc.h>
#include <core/platform/f4xx/stm32f4xx.hpp>
#include <sys/types.h>

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

GPIOf4xx::GPIOf4xx(Pin pin, GPIO::Direction direction, Pull pull) : GPIO(pin, direction, pull) {

    gpioSingleInit(pin, static_cast<uint32_t>(direction), static_cast<uint32_t>(pull), GPIO_SPEED_FREQ_VERY_HIGH);

    switch (portFromPin(pin)) {
        // STM32F446 and STM32F469
    case Port::A:
        this->port = GPIOA;
        break;
    case Port::B:
        this->port = GPIOB;
        break;
    case Port::C:
        this->port = GPIOC;
        break;
    case Port::D:
        this->port = GPIOD;
        break;
    case Port::F:
        this->port = GPIOF;
        break;
    #ifdef HAS_PORT_E
    case Port::E:
        this->port = GPIOE;
        break;
    #endif
    #ifdef HAS_PORT_G
    case Port::G:
        this->port = GPIOG;
        break;
    #endif
    #ifdef HAS_PORT_H
    case Port::H:
        this->port = GPIOH;
        break;
    #endif
    #ifdef HAS_PORT_I
    case Port::I:
        this->port = GPIOI;
        break;
    #endif
    default:
        break; // Should never get here
    }

    this->halPin = 1 << (static_cast<uint16_t>(this->pin) & 0x0F);
    this->writePin(GPIO::State::LOW); // Output set low by default
}

void GPIOf4xx::setDirection(GPIO::Direction direction) {
    const uint16_t position = 2 * pinNumberFromPin(this->pin);
    this->port->MODER &= ~(0b11 << position);
    if (direction == Direction::INPUT) {
        this->port->MODER |= (MODE_INPUT << position);
    } else { // Direction::OUTPUT
        this->port->MODER |= (MODE_OUTPUT << position);
    }
}

void GPIOf4xx::writePin(GPIO::State state) {
    HAL_GPIO_WritePin(this->port, this->halPin, static_cast<GPIO_PinState>(state));
}

GPIO::State GPIOf4xx::readPin() {
    return static_cast<GPIO::State>(HAL_GPIO_ReadPin(this->port, this->halPin));
}

void GPIOf4xx::registerIRQ(TriggerEdge edge, void (*irqHandler)(GPIO* pin, void* priv), void* priv) {
    GPIO_InitTypeDef gpioInit;
    Pin myPins[]      = {pin};
    uint8_t numOfPins = 1;

    gpioStateInit(&gpioInit,
                  myPins,
                  numOfPins,
                  GPIOf4xx::GPIO_TRIGGER_INTERRUPT_BASE | (static_cast<uint32_t>(edge) << GPIO_MODE_IT_SHIFT),
                  static_cast<uint32_t>(pull),
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
        irqNum = EXTI2_IRQn;
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

static void initHALGPIO(GPIO_InitTypeDef& targetGpio, Port port) {
    switch (port) {
    // STM32F446 and STM32F469
    case Port::A:
        __HAL_RCC_GPIOA_CLK_ENABLE();
        HAL_GPIO_Init(GPIOA, &targetGpio);
        break;
    case Port::B:
        __HAL_RCC_GPIOB_CLK_ENABLE();
        HAL_GPIO_Init(GPIOB, &targetGpio);
        break;
    case Port::C:
        __HAL_RCC_GPIOC_CLK_ENABLE();
        HAL_GPIO_Init(GPIOC, &targetGpio);
        break;
    case Port::D:
        __HAL_RCC_GPIOD_CLK_ENABLE();
        HAL_GPIO_Init(GPIOD, &targetGpio);
        break;
    case Port::F:
        __HAL_RCC_GPIOF_CLK_ENABLE();
        HAL_GPIO_Init(GPIOF, &targetGpio);
        break;
    #ifdef HAS_PORT_E
    case Port::E:
        __HAL_RCC_GPIOE_CLK_ENABLE();
        HAL_GPIO_Init(GPIOE, &targetGpio);
        break;
    #endif
    #ifdef HAS_PORT_G
    case Port::G:
        __HAL_RCC_GPIOG_CLK_ENABLE();
        HAL_GPIO_Init(GPIOG, &targetGpio);
        break;
    #endif
    #ifdef HAS_PORT_H
    case Port::H:
        __HAL_RCC_GPIOH_CLK_ENABLE();
        HAL_GPIO_Init(GPIOH, &targetGpio);
        break;
    #endif
    #ifdef HAS_PORT_I
    case Port::I:
        __HAL_RCC_GPIOI_CLK_ENABLE();
        HAL_GPIO_Init(GPIOI, &targetGpio);
        break;
    #endif
    default:
        break; // Bad Port input
    }
}

// do easy swaps without needing stack or calling additional function
#define SWAP(A, B) \
        A = A ^ B; \
        B = A ^ B; \
        A = A ^ B;

void GPIOf4xx::gpioInit(Pin* pins, uint8_t numOfPins, uint32_t mode, uint32_t pull, uint32_t speed, uint8_t alternate) {
    if (numOfPins == 0) {
        return;
    }
    if (numOfPins == 1) {
        gpioSingleInit(pins[0], mode, pull, speed, alternate);
        return;
    }

    // HeapSort on Pin* pins from https://en.wikipedia.org/wiki/Heapsort#Standard_implementation
    auto* sorted  = reinterpret_cast<uint8_t*>(pins);
    uint8_t start = numOfPins / 2, end = numOfPins;
    while (end > 1) {
        if (start > 0) { // Heap Construction
            start--;
        } else { // Heap Extraction
            end--;
            SWAP(sorted[0], sorted[end]);
        }
        // siftDown(a, start, end)
        uint8_t child;
        uint8_t root = start;
        while ((child = 2 * root + 1) < end) {
            if (child + 1 < end && sorted[child] < sorted[child + 1]) {
                child++;
            }
            if (sorted[root] < sorted[child]) {
                SWAP(sorted[root], sorted[child]);
                root = child; // repeat to continue sifting down the child now
            } else {
                break; // return to outer loop
            }
        }
    }

    // reuse start variable
    PinPack pin_pack{};
    start              = 0;
    Port port_of_start = portFromPin(pins[start]);
    for (uint8_t i = 1; i < numOfPins; i++) {
        if (portFromPin(pins[i]) > port_of_start) {
            if (start == i - 1) {
                gpioSingleInit(pins[start], mode, pull, speed, alternate);
            } else {
                fillPinPack(pin_pack, &pins[start], i - start);
                gpioPortInit(pin_pack, port_of_start, mode, pull, speed, alternate);
            }
            start         = i;
            port_of_start = portFromPin(pins[start]);
        }
    }
}

void GPIOf4xx::gpioSingleInit(Pin pin, uint32_t mode, uint32_t pull, uint32_t speed, uint8_t alternate) {
    GPIO_InitTypeDef targetGpio;
    targetGpio.Pin       = setPackBit(pin);
    targetGpio.Mode      = mode;
    targetGpio.Pull      = pull;
    targetGpio.Speed     = speed;
    targetGpio.Alternate = alternate;

    initHALGPIO(targetGpio, portFromPin(pin));
}

// If your passed in pins aren't all on the same port then it's your own fault
void GPIOf4xx::gpioPortInit(PinPack pack_pins, Port port, uint32_t mode, uint32_t pull, uint32_t speed,
                            uint8_t alternate) {
    GPIO_InitTypeDef targetGpio;
    targetGpio.Pin       = pack_pins.value;
    targetGpio.Mode      = mode;
    targetGpio.Pull      = pull;
    targetGpio.Speed     = speed;
    targetGpio.Alternate = alternate;

    initHALGPIO(targetGpio, port);
}

void GPIOf4xx::gpioStateInit(GPIO_InitTypeDef* targetGpio, Pin* pins, uint8_t numOfPins, uint32_t mode, uint32_t pull,
                             uint32_t speed, uint8_t alternate) {
#pragma deprecated("GPIOf4xx::gpioStateInit is deprecated, but available for backwards compatibility. Instead use" \
    "GPIOf4xx::gpioSingleInit() in most cases where you would want to use this function")

    if (numOfPins == 2) {
        targetGpio->Pin = setPackBit(pins[0]) | setPackBit(pins[1]);
    } else {
        targetGpio->Pin = setPackBit(pins[0]);
    }

    targetGpio->Mode = mode;

    targetGpio->Pull = pull;

    targetGpio->Speed = speed;

    // 0x00U is the default value used by the F4xxxx
    if (alternate != 0x00U) {
        targetGpio->Alternate = alternate;
    }

    for (uint8_t i = 0; i < numOfPins; i++) {
        initHALGPIO(*targetGpio, portFromPin(pins[i]));
    }
}

} // namespace core::io
