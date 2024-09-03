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
    hal_rcc_clock_config_t RCC_Config = {
        .sysclk_src = RCC_SYSCLK_SRC_PLL,
        .pll_src = RCC_PLL_SRC_HSE,
        .mco_src = RCC_MCO_DISABLED,
        .hse_div = RCC_HSE_PLL_DIV_1,
        .pll_mul = RCC_PLL_MUL_9,
        .ahb_pre = RCC_AHB_PRE_NONE,
        .apb1_pre = RCC_APB_PRE_2,
        .apb2_pre = RCC_APB_PRE_NONE,
    };

    if (FLASH_SetLatency(FLASH_ACR_LATENCY_2) != HAL_SUCCESS) {
        return HAL_FAILED;
    }

    if (hal_rcc_set_clock_config(&RCC_Config) != HAL_SUCCESS) {
        return HAL_FAILED;
    }

    hal_rcc_clock_freq_t clockFreq = {0, 0, 0, 0};
    hal_rcc_clock_get_freq(&clockFreq);

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
