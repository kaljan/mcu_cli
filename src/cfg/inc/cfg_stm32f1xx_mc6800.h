/**
 * @file    cfg_stm32f1xx_mc6800.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-07
 *
 * @brief   Confuiguration for MC6800 port
 */
#ifndef CFG_STM32F1XX_MC6800_H
#define CFG_STM32F1XX_MC6800_H

#include "hal_config.h"

#if (HAL_CLCD_PWREN == HAL_ENABLED)
#define CLCD_PWREN_PORT                     GPIOA
#define CLCD_PWREN_PIN                      15U
#endif

#define MC6800_E_PORT                       GPIOA
#define MC6800_E_PIN                        9U

#define MC6800_RS_PORT                      GPIOA
#define MC6800_RS_PIN                       8U

#if (HAL_MC6800_READ == HAL_ENABLED)
#define MC6800_RW_PORT                      GPIOA
#define MC6800_RW_PIN                       10U
#endif

#if (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA8)
#define MC6800_D0_PORT                      GPIOB
#define MC6800_D0_PIN                       9U

#define MC6800_D1_PORT                      GPIOB
#define MC6800_D1_PIN                       12U

#define MC6800_D2_PORT                      GPIOB
#define MC6800_D2_PIN                       8U

#define MC6800_D3_PORT                      GPIOB
#define MC6800_D3_PIN                       13U
#endif

#define MC6800_D4_PORT                      GPIOB
#define MC6800_D4_PIN                       6U

#define MC6800_D5_PORT                      GPIOB
#define MC6800_D5_PIN                       3U

#define MC6800_D6_PORT                      GPIOB
#define MC6800_D6_PIN                       5U

#define MC6800_D7_PORT                      GPIOB
#define MC6800_D7_PIN                       4U


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CFG_STM32F1XX_MC6800_H */
