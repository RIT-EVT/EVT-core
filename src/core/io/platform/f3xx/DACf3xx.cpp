#include <core/io/platform/f3xx/DACf3xx.hpp>

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal.h>
#include <core/io/pin.hpp>

namespace core::io {

static DACf3xx* dacInstance = nullptr;

extern "C" void TIM6_DAC1_IRQHandler(void) {
    if (dacInstance) {
        HAL_DAC_IRQHandler(&dacInstance->halDac);
    }
}

extern "C" void TIM7_DAC2_IRQHandler(void) {
    if (dacInstance) {
        HAL_DAC_IRQHandler(&dacInstance->halDac);
    }
}

DACf3xx::DACf3xx(Pin pin, DACPeriph dacPeriph) : DACBase(pin, dacPeriph), halDac{} {
    dacInstance = this;

    // Validate that the pin supports the requested DAC peripheral
    Channel_Support channelSupport = getChannelSupport(pin);
    if (!checkSupport(dacPeriph, channelSupport)) {
        // Pin doesn't support the requested DAC peripheral, use default
        dacPeriph = DACPeriph::ONE;
    }

    // Set the appropriate DAC instance based on the peripheral
    switch (dacPeriph) {
    case DACPeriph::ONE:
        halDac.Instance = DAC1;
        break;
    case DACPeriph::TWO:
#ifdef DAC2
        halDac.Instance = DAC2;
#else
        halDac.Instance = DAC1;      // DAC2 not available on this variant
#endif
        break;
    default:
        halDac.Instance = DAC1; // Default fallback
        break;
    }

    // Determine and store the channel
    channel = getChannelFromPin();

    initGPIO();
    initDAC();
}

void DACf3xx::setValue(uint32_t value) {
    if (value > MAX_RAW) {
        value = MAX_RAW;
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
    case DACPeriph::TWO:
        return channelStruct.dac2 == 1;
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

    // Enable appropriate DAC clock based on peripheral
    switch (dacPeriph) {
    case DACPeriph::ONE:
        __HAL_RCC_DAC1_CLK_ENABLE();
        break;
    case DACPeriph::TWO:
#ifdef __HAL_RCC_DAC2_CLK_ENABLE
        __HAL_RCC_DAC2_CLK_ENABLE();
#else
        __HAL_RCC_DAC1_CLK_ENABLE(); // DAC2 not available on this variant
#endif
        break;
    default:
        __HAL_RCC_DAC1_CLK_ENABLE(); // Default fallback
        break;
    }

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

uint32_t DACf3xx::getChannelFromPin() {
    if (pin == Pin::PA_4) {
        return DAC_CHANNEL_1;
    } else if (pin == Pin::PA_5) {
#ifdef DAC_CHANNEL_2
        return DAC_CHANNEL_2; // STM32F334 supports PA_5 (DAC_CHANNEL_2)
#else
        return DAC_CHANNEL_1;        // STM32F302 doesn't support PA_5, use CHANNEL_1
#endif
    } else {
        return DAC_CHANNEL_1; // Default fallback
    }
}

DACf3xx::Channel_Support DACf3xx::getChannelSupport(Pin pin) {
    Channel_Support support = {0};

    if (pin == Pin::PA_4) {
        support.dac1    = 1; // PA_4 supports DAC1
        support.dac2    = 0; // PA_4 doesn't support DAC2
        support.channel = DAC_CHANNEL_1;
    } else if (pin == Pin::PA_5) {
        support.dac1 = 0; // PA_5 doesn't support DAC1
#ifdef DAC_CHANNEL_2
        support.dac2    = 1; // PA_5 supports DAC2 (on STM32F334)
        support.channel = DAC_CHANNEL_2;
#else
        support.dac2    = 0;         // PA_5 doesn't support DAC2 (on STM32F302)
        support.channel = DAC_CHANNEL_1;
#endif
    } else {
        // Default fallback
        support.dac1    = 1;
        support.dac2    = 0;
        support.channel = DAC_CHANNEL_1;
    }

    return support;
}

} // namespace core::io
