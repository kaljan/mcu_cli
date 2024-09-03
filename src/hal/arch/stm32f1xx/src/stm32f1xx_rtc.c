/**
 * @file    stm32f1xx_rtc.c
 * @author  kaljan
 * @version 1.0.0
 * @date    28-01-24
 *
 * @brief
 */
#include "stm32f1xx_rtc.h"
#include "stm32f1xx_rcc.h"
#include "stm32f1xx_bkp.h"
#include "stm32f1xx_pwr.h"

#define RTC_BKP_INIT_WORD           0x55AA
#define RTC_BKP_INIT_WORD_INDEX     0

int RTC_Initialize(void) {
    RCC_BKP_CLOCK_ENABLE();
    RCC_PWR_CLOCK_ENABLE();

    if (BKP_ReadDataRegistre(RTC_BKP_INIT_WORD_INDEX) == RTC_BKP_INIT_WORD) {
        return HAL_SUCCESS;
    }

    PWR_BKP_ENABLE();

    BKP_WriteDataRegister(RTC_BKP_INIT_WORD_INDEX, RTC_BKP_INIT_WORD);

    if (hal_rcc_lse_enable() != HAL_SUCCESS) {
        return HAL_FAILED;
    }

    RCC_RTC_ENABLE();

    return HAL_SUCCESS;
}