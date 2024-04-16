/**
 * @file    stm32f1xx_pwr.h
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-24
 *
 * @brief
 */
#ifndef STM32F1XX_PWR_H
#define STM32F1XX_PWR_H

#include "stm32f1xx_config.h"

#define PWR_BKP_ENABLE()     HAL_SET_BITS(PWR->CR, PWR_CR_DBP);

#define PWR_BKP_DISABLE()    HAL_CLEAR_BITS(PWR->CR, PWR_CR_DBP);

#endif /* STM32F1XX_PWR_H */
/* EOF */
