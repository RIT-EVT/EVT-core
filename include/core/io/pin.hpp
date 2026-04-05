/**
 * This contains the different pin options that are available. The pin mappings
 * are hardware specific and determined by the platform that is being compiled
 * for
 */
#ifndef _EVT_PIN_
#define _EVT_PIN_

#include <cstdint>

// TODO: Fix this so that it changes pin out on a per Package Basis instead of a per MCU basis
#ifdef STM32F302x8
    #define NUCLEO_SUPPORT
#endif

#ifdef STM32F334x8
    #define NUCLEO_SUPPORT
#endif

#ifdef STM32F446xx
    #define NUCLEO_SUPPORT
#endif

#ifdef STM32F469xx
    #define HAS_PORT_D
    #define HAS_PORT_E
    #define HAS_PORT_F
    #define HAS_PORT_G
    #define HAS_PORT_H
    #define HAS_PORT_I
#endif

namespace core::io {

/**
 * Pin mapping information. These values are generated via a combination of the GPIO bank that
 * the pin is on and the number of the pin. We referenced MBed's documentation for generating
 * these values.
 */
enum class Pin {
    INVALID = -1, // THIS INTENTIONALLY DOES NOT POINT TO A PIN. Used as a default value, so the default value is
                  // no longer PA_O (a real pin)
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_2  = 0x02,
    PA_3  = 0x03,
    PA_4  = 0x04,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_7  = 0x07,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,

    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,

    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_2  = 0x22,
    PC_3  = 0x23,
    PC_4  = 0x24,
    PC_5  = 0x25,
    PC_6  = 0x26,
    PC_7  = 0x27,
    PC_8  = 0x28,
    PC_9  = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

#ifdef HAS_PORT_D
    PD_0  = 0x30,
    PD_1  = 0x31,
    PD_2  = 0x32,
    PD_3  = 0x33,
    PD_4  = 0x34,
    PD_5  = 0x35,
    PD_6  = 0x36,
    PD_7  = 0x37,
    PD_8  = 0x38,
    PD_9  = 0x39,
    PD_10 = 0x3A,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,
    PD_14 = 0x3E,
    PD_15 = 0x3F,
#endif

#ifdef HAS_PORT_E
    PE_0  = 0x40,
    PE_1  = 0x41,
    PE_2  = 0x42,
    PE_3  = 0x43,
    PE_4  = 0x44,
    PE_5  = 0x45,
    PE_6  = 0x46,
    PE_7  = 0x47,
    PE_8  = 0x48,
    PE_9  = 0x49,
    PE_10 = 0x4A,
    PE_11 = 0x4B,
    PE_12 = 0x4C,
    PE_13 = 0x4D,
    PE_14 = 0x4E,
    PE_15 = 0x4F,
#endif

#ifdef HAS_PORT_F
    PF_0  = 0x50,
    PF_1  = 0x51,
    PF_2  = 0x52,
    PF_3  = 0x53,
    PF_4  = 0x54,
    PF_5  = 0x55,
    PF_6  = 0x56,
    PF_7  = 0x57,
    PF_8  = 0x58,
    PF_9  = 0x59,
    PF_10 = 0x5A,
    PF_11 = 0x5B,
    PF_12 = 0x5C,
    PF_13 = 0x5D,
    PF_14 = 0x5E,
    PF_15 = 0x5F,
#endif

#ifdef HAS_PORT_G
    PG_0  = 0x60,
    PG_1  = 0x61,
    PG_2  = 0x62,
    PG_3  = 0x63,
    PG_4  = 0x64,
    PG_5  = 0x65,
    PG_6  = 0x66,
    PG_7  = 0x67,
    PG_8  = 0x68,
    PG_9  = 0x69,
    PG_10 = 0x6A,
    PG_11 = 0x6B,
    PG_12 = 0x6C,
    PG_13 = 0x6D,
    PG_14 = 0x6E,
    PG_15 = 0x6F,
#endif

#ifdef HAS_PORT_H
    PH_0  = 0x70,
    PH_1  = 0x71,
    PH_2  = 0x72,
    PH_3  = 0x73,
    PH_4  = 0x74,
    PH_5  = 0x75,
    PH_6  = 0x76,
    PH_7  = 0x77,
    PH_8  = 0x78,
    PH_9  = 0x79,
    PH_10 = 0x7A,
    PH_11 = 0x7B,
    PH_12 = 0x7C,
    PH_13 = 0x7D,
    PH_14 = 0x7E,
    PH_15 = 0x7F,
#endif

#ifdef HAS_PORT_I
    PI_0  = 0x80,
    PI_1  = 0x81,
    PI_2  = 0x82,
    PI_3  = 0x83,
    PI_4  = 0x84,
    PI_5  = 0x85,
    PI_6  = 0x86,
    PI_7  = 0x87,
    PI_8  = 0x88,
    PI_9  = 0x89,
    PI_10 = 0x8A,
    PI_11 = 0x8B,
    PI_12 = 0x8C,
    PI_13 = 0x8D,
    PI_14 = 0x8E,
    PI_15 = 0x8F,
#endif

#ifdef NUCLEO_SUPPORT
    PD_2 = 0x32,

    PF_0 = 0x50,
    PF_1 = 0x51,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Arduino connector namings
    A0  = PA_0,
    A1  = PA_1,
    A2  = PA_4,
    A3  = PB_0,
    A4  = PC_1,
    A5  = PC_0,
    D0  = PA_3,
    D1  = PA_2,
    D2  = PA_10,
    D3  = PB_3,
    D4  = PB_5,
    D5  = PB_4,
    D6  = PB_10,
    D7  = PA_8,
    D8  = PA_9,
    D9  = PC_7,
    D10 = PB_6,
    D11 = PB_15,
    D12 = PB_14,
    D13 = PB_13,
    D14 = PB_9,
    D15 = PB_8,

    // Common Configured Pins
    #if defined(STM32F302x8)
    LED = PB_13,

    SPI_CS   = D10, // PB_6
    SPI_MOSI = D11, // PB_15
    SPI_MISO = D12, // PB_14
    SPI_SCK  = D13, // PB_13
    I2C_SDA  = D14, // PB_8
    I2C_SCL  = D15, // PB_9
    #elif defined(STM32F334x8)
    LED = PA_5,

    SPI_CS   = D10, // PB_6
    SPI_MOSI = D11, // PA_5
    SPI_MISO = D12, // PA_6
    SPI_SCK  = D13, // PA_7
    I2C_SDA  = D14, // PB_8
    I2C_SCL  = D15, // PB_9
    #elif defined(STM32F446xx)
    LED = PA_5,

    SPI_CS   = D10, // PA_5
    SPI_MOSI = D11, // PA_6
    SPI_MISO = D12, // PA_7
    SPI_SCK  = D13, // PB_6
    I2C_SDA  = D14, // PB_9
    I2C_SCL  = D15, // PB_8
    #endif

    UART_TX = PA_2, // D1
    UART_RX = PA_3, // D0
#endif              // NUCLEO_SUPPORT

#ifndef NUCLEO_SUPPORT // STM32F469xx
    LED      = INVALID,
    SPI_CS   = INVALID,
    SPI_MOSI = INVALID,
    SPI_MISO = INVALID,
    SPI_SCK  = INVALID,
    I2C_SDA  = INVALID,
    I2C_SCL  = INVALID,
#endif

#ifdef STM32F469xx
    UART_TX = PA_9,
    UART_RX = PA_10,
#endif
};

enum class Port : uint8_t {
    Invalid = -1,
    A = 0x0,
    B = 0x1,
    C = 0x2,
    D = 0x3,
    #ifdef HAS_PORT_E
    E = 0x4,
    #endif
    F = 0x5,
    #ifdef HAS_PORT_G
    G = 0x6,
    #endif
    #ifdef HAS_PORT_H
    H = 0x7,
    #endif
    #ifdef HAS_PORT_I
    I = 0x8,
    #endif
};

/**
 * Pull the port from a Pin
 *
 * @param pin Pin
 * @return Port of the Pin
 */
constexpr Port portFromPin(Pin pin) {
    return static_cast<Port>((static_cast<uint32_t>(pin) & 0xF0) >> 4);
}

/**
 * Pull the pin number from a Pin
 *
 * @param pin Pin
 * @return Pin number of the Pin
 */
constexpr uint8_t pinNumberFromPin(Pin pin) {
    return static_cast<uint8_t>(pin) & 0x0F ;
}

}; // namespace core::io
#endif
