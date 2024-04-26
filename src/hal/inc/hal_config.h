/**
 * @file    hal_config.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-07
 *
 * @brief
 */
#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H


#ifdef HAL_CONFIG
#include HAL_CONFIG
#else
#include "hal_opt.h"

#define HAL_GPIO_ENABLE             HAL_ENABLED
#define HAL_TIM_ENABLE              HAL_ENABLED
#define HAL_UART_ENABLE             HAL_ENABLED

#define HAL_MC6800_ENABLE           HAL_ENABLED

#if (HAL_MC6800_ENABLE == HAL_ENABLED)
#define HAL_CLCD_PWREN              HAL_ENABLED
#define HAL_MC6800_READ             HAL_DISABLED /* HAL_ENABLED */
#define HAL_MC6800_DATA_WIDTH       HAL_MC6800_DATA4
#define HAL_MC6800_LATCH_DELAY_US   1
// #define HAL_MC6800_READ             HAL_DISABLED
#endif
#endif

#endif /* HAL_CONFIG_H */
