/**
 * @file    stm32f1xx_config.h
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-24
 *
 * @brief
 */
#ifndef STM32F1XX_CONFIG_H
#define STM32F1XX_CONFIG_H

#include "stm32f1xx.h"
#include "hal_opt.h"

#ifdef STM32F1XX_CONFIG
#include STM32F1XX_CONFIG
#else
#endif /* STM32F1XX_CONFIG */

#ifndef HSE_VALUE
#define HSE_VALUE 8000000
#endif

#ifndef HSI_VALUE
#define HSI_VALUE 8000000
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_CONFIG_H */
/* EOF */
