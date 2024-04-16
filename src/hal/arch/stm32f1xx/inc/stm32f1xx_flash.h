/**
 * @file    stm32f1xx_flash.h
 * @author  kaljan
 * @version 1.0.0
 * @date    28-01-24
 *
 * @brief
 */
#ifndef STM32F1XX_FLASH_H
#define STM32F1XX_FLASH_H

#include "stm32f1xx_config.h"

#define FLASH_LATENCY_0         (uint32_t)0U
#define FLASH_LATENCY_1         (uint32_t)1U
#define FLASH_LATENCY_2         (uint32_t)2U
#define FLASH_LATENCY_MASK      (uint32_t)0x07U
#define FLASH_LATENCY_OFFSET    (uint32_t)0U

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int FLASH_SetLatency(uint32_t latency);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_FLASH_H */
/* EOF */
