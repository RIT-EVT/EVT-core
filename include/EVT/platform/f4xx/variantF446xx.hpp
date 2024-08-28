/**
 * This contains the specific pin configurations for the F446 platform.
 */

#ifndef _EVT_VARIANT_
#define _EVT_VARIANT_

#include <stm32f446xx.h>

namespace core::io {

// Arduino connector namings
#define A0  PA_0
#define A1  PA_1
#define A2  PA_4
#define A3  PB_0
#define A4  PC_1
#define A5  PC_0
#define D0  PA_3
#define D1  PA_2
#define D2  PA_10
#define D3  PB_3
#define D4  PB_5
#define D5  PB_4
#define D6  PB_10
#define D7  PA_8
#define D8  PA_9
#define D9  PC_7
#define D10 PB_6
#define D11 PA_7
#define D12 PA_6
#define D13 PA_5
#define D14 PB_9
#define D15 PB_8

// Board Features
#define LED    PA_5
#define BUTTON PC_13

// standard communication pins
#define I2C_SDA  PB_9,
#define I2C_SCL  PB_8,
#define SPI_SCK  PA_5, // D13
#define SPI_MISO PA_6, // D12
#define SPI_MOSI PA_7, // D11
#define SPI_CS   PB_6, // D10
#define UART_TX  PA_2,
#define UART_RX  PA_3,

/*	Typedefs	*/
typedef ADC_TypeDef* ADC_t;
typedef TIM_TypeDef* TIM_t;
typedef USART_TypeDef* USART_t;
typedef I2C_TypeDef* I2C_t;
typedef CAN_TypeDef* CAN_t;
typedef SPI_TypeDef* SPI_t;

enum class PeriperalUsartVariants : uint8_t
{

}

typedef uint32_t PinMap_t;

}; // namespace core::io

#endif //_EVT_VARIANT_