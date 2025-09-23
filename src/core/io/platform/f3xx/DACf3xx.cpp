#include <core/io/platform/f3xx/DACf3xx.hpp>

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal.h>
#include <core/io/pin.hpp>

namespace core::io {

static DACf3xx* dacInstance = nullptr;

extern "C" void TIM6_DAC1_IRQHandler(void) {
    if (dacInstance) {
        HAL_DAC_IRQHandler(dacInstance->getHalDac());
    }
}

extern "C" void TIM7_DAC2_IRQHandler(void) {
    if (dacInstance) {
        HAL_DAC_IRQHandler(dacInstance->getHalDac());
    }
}

DACf3xx::DACf3xx(Pin pin, DACPeriph dacPeriph) : DACBase(pin, dacPeriph), halDac{} {
    dacInstance     = this;
    halDac.Instance = DAC1;
    initGPIO();
    initDAC();
}

void DACf3xx::setValue(uint32_t value) {
    if (value > MAX_RAW) {
        value = MAX_RAW;
    }

    uint32_t channel;
    if (pin == Pin::PA_4) {
        channel = DAC_CHANNEL_1;
    } else if (pin == Pin::PA_5) {
#if defined(DAC_CHANNEL2_SUPPORT)
        channel = DAC_CHANNEL_2; // STM32F334 supports PA_5 (DAC_CHANNEL_2)
#else
        channel = DAC_CHANNEL_1; // STM32F302 doesn't support PA_5, use CHANNEL_1
#endif
    } else {
        channel = DAC_CHANNEL_1; // Default fallback
    }

    HAL_DAC_SetValue(&halDac, channel, DAC_ALIGN_12B_R, value);
    currentValue = value;
}

uint32_t DACf3xx::getValue() const {
    return currentValue;
}

void DACf3xx::setVoltage(float voltage) {
    uint32_t value = static_cast<uint32_t>((voltage * MAX_RAW) / VREF_POS);

    if (value > MAX_RAW) {
        value = MAX_RAW;
    }

    setValue(value);
}

float DACf3xx::getVoltage() const {
    return (static_cast<float>(currentValue) * VREF_POS) / MAX_RAW;
}

bool DACf3xx::checkSupport(DACPeriph periph, Channel_Support channelStruct) {
    switch (periph) {
    case DACPeriph::ONE:
        return channelStruct.dac1 == 1;
    default:
        return false;
    }
}

void DACf3xx::initDAC() {
    HAL_StatusTypeDef status;
    DAC_ChannelConfTypeDef sConfig = {0};

    status = HAL_DAC_Init(&halDac);
    if (status != HAL_OK) {
        return;
    }

    sConfig.DAC_Trigger      = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    uint32_t channel;
    if (pin == Pin::PA_4) {
        channel = DAC_CHANNEL_1;
    } else if (pin == Pin::PA_5) {
#if defined(DAC_CHANNEL2_SUPPORT)
        channel = DAC_CHANNEL_2; // STM32F334 supports PA_5 (DAC_CHANNEL_2)
#else
        channel = DAC_CHANNEL_1; // STM32F302 doesn't support PA_5, use CHANNEL_1
#endif
    } else {
        channel = DAC_CHANNEL_1; // Default fallback
    }
    status = HAL_DAC_ConfigChannel(&halDac, &sConfig, channel);
    if (status != HAL_OK) {
        return;
    }

    status = HAL_DAC_Start(&halDac, channel);
    if (status != HAL_OK) {
        return;
    }
}

void DACf3xx::initGPIO() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_DAC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    if (pin == Pin::PA_4) {
        GPIO_InitStruct.Pin = GPIO_PIN_4;
    } else if (pin == Pin::PA_5) {
        GPIO_InitStruct.Pin = GPIO_PIN_5;
    }

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

} // namespace core::io
