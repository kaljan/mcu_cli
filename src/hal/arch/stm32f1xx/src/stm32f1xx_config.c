/**
 * @file    stm32f1xx_config.c
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-24
 *
 * @brief
 */

#include "stm32f1xx_config.h"
#include "stm32f1xx_rcc.h"
#include "stm32f1xx_flash.h"

#include "hal_tim.h"

int hal_init(void) {
    RCC_ClockConfig_t RCC_Config = {
        .SYSCLK_Src = RCC_SYSCLK_SRC_PLL,
        .PLL_Src = RCC_PLL_SRC_HSE,
        .MCO_Src = RCC_MCO_DISABLED,
        .HSE_Div = RCC_HSE_PLL_DIV_1,
        .PLL_Mul = RCC_PLL_MUL_9,
        .AHB_Pre = RCC_AHB_PRE_NONE,
        .APB1_Pre = RCC_APB_PRE_2,
        .APB2_Pre = RCC_APB_PRE_NONE,
    };

    if (FLASH_SetLatency(FLASH_ACR_LATENCY_2) != HAL_SUCCESS) {
        return HAL_FAILED;
    }

    if (RCC_ClockSetConfig(&RCC_Config) != HAL_SUCCESS) {
        return HAL_FAILED;
    }

    RCC_ClockFreq_t clockFreq = {0, 0, 0, 0};
    RCC_ClockGetFreq(&clockFreq);

    RCC_GPIOA_CLOCK_ENABLE();
    RCC_GPIOB_CLOCK_ENABLE();
    RCC_GPIOC_CLOCK_ENABLE();
    RCC_GPIOD_CLOCK_ENABLE();
    RCC_AFIO_CLOCK_ENABLE();

    CLEAR_BIT(AFIO->MAPR,AFIO_MAPR_SWJ_CFG);
    SET_BIT(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);

    hal_sys_time_init();

    return HAL_SUCCESS;
}
