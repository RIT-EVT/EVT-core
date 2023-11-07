/**
 * This contains the different pin options that are available. The pin mappings
 * are hardware specific and determined by the platform that is being compiled
 * for
 */
#ifndef _EVT_PIN_
#define _EVT_PIN_

namespace EVT::core::IO {
// STM32F302R8 pin names

/**
 * Pin mapping information. These values are generated via a combination of the GPIO bank that
 * the pin is on and the number of the pin. We referenced MBed's documentation for generating
 * these values.
 */
enum class Pin {
    PA_0 = 0x00,
    PA_1 = 0x01,
    PA_2 = 0x02,
    PA_3 = 0x03,
    PA_4 = 0x04,
    PA_5 = 0x05,
    PA_6 = 0x06,
    PA_7 = 0x07,
    PA_8 = 0x08,
    PA_9 = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,

    PB_0 = 0x10,
    PB_1 = 0x11,
    PB_2 = 0x12,
    PB_3 = 0x13,
    PB_4 = 0x14,
    PB_5 = 0x15,
    PB_6 = 0x16,
    PB_7 = 0x17,
    PB_8 = 0x18,
    PB_9 = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,

    PC_0 = 0x20,
    PC_1 = 0x21,
    PC_2 = 0x22,
    PC_3 = 0x23,
    PC_4 = 0x24,
    PC_5 = 0x25,
    PC_6 = 0x26,
    PC_7 = 0x27,
    PC_8 = 0x28,
    PC_9 = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

    PD_2 = 0x32,

    PF_0 = 0x50,
    PF_1 = 0x51,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Arduino connector namings
    A0 = PA_0,
    A1 = PA_1,
    A2 = PA_4,
    A3 = PB_0,
    A4 = PC_1,
    A5 = PC_0,
    D0 = PA_3,
    D1 = PA_2,
    D2 = PA_10,
    D3 = PB_3,
    D4 = PB_5,
    D5 = PB_4,
    D6 = PB_10,
    D7 = PA_8,
    D8 = PA_9,
    D9 = PC_7,
    D10 = PB_6,
    D11 = PB_15,
    D12 = PB_14,
    D13 = PB_13,
    D14 = PB_9,
    D15 = PB_8,

// Common Configured Pins
#ifdef STM32F302x8
    LED = PB_13,
#endif
#ifdef STM32F334x8
    LED = PA_5,
#endif

#ifdef STM32F302x8
    SPI_SCK = PB_13, //D13
    SPI_MISO = PB_14,//D12
    SPI_MOSI = PB_15,//D11
    SPI_CS = PB_6,   //D10
    I2C_SDA = PB_8,
    I2C_SCL = PB_9,
#endif
#ifdef STM32F334x8
    SPI_SCK = PA_5, //D13
    SPI_MISO = PA_6,//D12
    SPI_MOSI = PA_7,//D11
    SPI_CS = PB_6,  //D10
    I2C_SDA = PB_8,
    I2C_SCL = PB_9,
#endif

#ifdef STM32f4xx
    LED = PA_5,
#endif

    UART_TX = PA_2,
    UART_RX = PA_3,
    I2C_SDA = PB_6,
    I2C_SCL = PB_7,
};
};// namespace EVT::core::IO
#endif