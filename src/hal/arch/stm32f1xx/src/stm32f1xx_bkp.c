/**
 * @file    stm32f1xx_bkp.c
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-24
 *
 * @brief
 */
#include "stm32f1xx_bkp.h"

void BKP_WriteDataRegister(uint32_t index, uint32_t data) {
    if (RTC_BKP_NUMBER > index) {
        uint32_t volatile * drptr = (uint32_t volatile*)((uint32_t)BKP_BASE + (index * 4U) + 4U);
        *drptr = data;
    }
}

uint32_t BKP_ReadDataRegistre(uint32_t index) {
    uint32_t volatile * drptr = (uint32_t volatile*)((uint32_t)BKP_BASE + (index * 4U) + 4U);
    return ((*drptr) & 0x0000FFFFU);
}

