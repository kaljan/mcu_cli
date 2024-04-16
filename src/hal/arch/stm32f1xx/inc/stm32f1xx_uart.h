/**
 * @file    stm32f1xx_uart.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-03-23
 *
 * @brief
 */
#ifndef STM32F1XX_UART_H
#define STM32F1XX_UART_H

#include "stm32f1xx_config.h"
#include "stm32f1xx_rcc.h"
#include "stm32f1xx_gpio.h"
#include "tools_fifo.h"
#include "hal_opt.h"

typedef struct UART {
    volatile uint8_t flags;
    USART_TypeDef * handle;
    IRQn_Type irqn;
    tools_fifo_t rxFIFO;
    tools_fifo_t txFIFO;
    RCC_EnableClockFunc_t enableClock;
    void (*GPIO_Config)(int);
} UART_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_UART_H */
