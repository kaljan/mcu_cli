/**
 * @file    stm32f1xx_flash.c
 * @author  kaljan
 * @version 1.0.0
 * @date    28-01-24
 *
 * @brief
 */
#include "stm32f1xx_flash.h"

#define FLASH_TIMEOUT   1000000

int FLASH_SetLatency(uint32_t latency) {
    int timeout = FLASH_TIMEOUT;
    HAL_MODIFY_FIELD(FLASH->ACR, latency , FLASH_LATENCY_MASK, FLASH_LATENCY_OFFSET);

    while (HAL_FIELD_NE(FLASH->ACR, latency , FLASH_LATENCY_MASK, FLASH_LATENCY_OFFSET)) {
        if (timeout <= 0) {
            return HAL_FAILED;
        }
        -- timeout;
    }
    return HAL_SUCCESS;
}
