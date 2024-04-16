/**
 * @file    hal_uart.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-06
 *
 * @brief
 */
#ifndef HAL_UART_H
#define HAL_UART_H

#include "hal_config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int hal_uart_open(hal_object_t ctx, hal_ptr_t cfg);
void hal_uart_close(hal_object_t ctx);
int hal_uart_put(hal_object_t ctx, uint8_t ch);
int hal_uart_get(hal_object_t ctx, uint8_t* c);
hal_object_t hal_uart_get_handle(int id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_UART_H */
