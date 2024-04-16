/**
 * @file    stm32f1xx_bkp.h
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-24
 *
 * @brief
 */
#ifndef STM32F1XX_BKP_H
#define STM32F1XX_BKP_H

#include "stm32f1xx_config.h"


#define BKP_RTC_SET_CAL_VALUE(value)        HAL_MODIFY_REG(BKP->RTCCR, value, BKP_RTCCR_CAL_Msk);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void BKP_WriteDataRegister(uint32_t index, uint32_t data);
uint32_t BKP_ReadDataRegistre(uint32_t index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_BKP_H */
/* EOF */
