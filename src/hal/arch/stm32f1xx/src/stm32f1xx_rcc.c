/**
 * @file    stm32f1xx_rcc.c
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-20 00:01:05
 *
 * @brief
 */
#include "stm32f1xx_rcc.h"


#define RCC_TIMEOUT 1000000


static inline int RCC_EnableHSE(void) {
    int timeout = RCC_TIMEOUT;
    HAL_SET_BITS(RCC->CR, RCC_CR_HSEON);
    while (HAL_IS_BIT_CLEAR(RCC->CR, RCC_CR_HSERDY)) {
        if (timeout <= 0) {
            return HAL_FAILED;
        }
        timeout--;
    }
    return HAL_SUCCESS;
}


int RCC_ClockSetConfig(RCC_ClockConfig_t* config) {
    int timeout = RCC_TIMEOUT;
    if (NULL == config) {
        return HAL_FAILED;
    }

    if (RCC_SYSCLK_SRC_HSE == config->SYSCLK_Src) {
        if (RCC_EnableHSE() != HAL_SUCCESS) {
            return HAL_FAILED;
        }
    } else if (RCC_SYSCLK_SRC_PLL == config->SYSCLK_Src) {

        if (RCC_PLL_SRC_HSE == config->PLL_Src) {
            if (RCC_EnableHSE() != HAL_SUCCESS) {
                return HAL_FAILED;
            }

            HAL_SET_BITS(RCC->CFGR, RCC_CFGR_PLLXTPRE_HSE_DIV2);

            if (config->HSE_Div == RCC_HSE_PLL_DIV_1) {
                HAL_CLEAR_BITS(RCC->CFGR, RCC_CFGR_PLLXTPRE_HSE_DIV2);
            } else if (config->HSE_Div == RCC_HSE_PLL_DIV_2) {
                HAL_SET_BITS(RCC->CFGR, RCC_CFGR_PLLXTPRE_HSE_DIV2);
            }
            HAL_SET_BITS(RCC->CFGR, RCC_CFGR_PLLSRC);
        }

        HAL_MODIFY_FIELD(RCC->CFGR, config->PLL_Mul,
            RCC_PLL_MUL_MASK, RCC_PLL_MUL_OFFSET);

        HAL_SET_BITS(RCC->CR, RCC_CR_PLLON);

        timeout = RCC_TIMEOUT;
        while (HAL_IS_BIT_CLEAR(RCC->CR, RCC_CR_PLLRDY)) {
            if (timeout <= 0) {
                return HAL_FAILED;
            }
            timeout--;
        }
    }

    HAL_MODIFY_FIELD(RCC->CFGR, config->AHB_Pre,
        RCC_AHB_PRE_MASK, RCC_AHB_PRE_OFFSET);

    HAL_MODIFY_FIELD(RCC->CFGR, config->APB1_Pre,
        RCC_APB_PRE_MASK, RCC_APB1_PRE_OFFSET);

    HAL_MODIFY_FIELD(RCC->CFGR, config->APB2_Pre,
        RCC_APB_PRE_MASK, RCC_APB2_PRE_OFFSET);

    HAL_MODIFY_FIELD(RCC->CFGR, config->SYSCLK_Src, RCC_SYSCLK_SRC_MASK, RCC_SYSCLK_SRC_SW_OFFSET);

    timeout = RCC_TIMEOUT;
    while (HAL_FIELD_NE(RCC->CFGR, config->SYSCLK_Src, RCC_SYSCLK_SRC_MASK, RCC_SYSCLK_SRC_SWS_OFFSET)) {
        if (timeout <= 0) {
            return HAL_FAILED;
        }
        timeout--;
    }

    return HAL_SUCCESS;
}

uint32_t RCC_APB_ClockGetFreq(uint32_t sysclk, uint32_t pre) {
    if (pre < RCC_APB_PRE_2) {
        return sysclk;
    } else {
        return sysclk / (2 << (pre & 0x03U));
    }
}

int RCC_ClockGetFreq(RCC_ClockFreq_t* dst) {
    if (NULL == dst) {
        return HAL_FAILED;
    }

    uint32_t sysclkSource = HAL_READ_FIELD(RCC->CFGR,
        RCC_SYSCLK_SRC_MASK, RCC_SYSCLK_SRC_SWS_OFFSET);

    switch (sysclkSource) {
    case RCC_SYSCLK_SRC_HSI:
        dst->SYSCLK = HSI_VALUE;
        break;

    case RCC_SYSCLK_SRC_HSE:
        dst->SYSCLK = HSE_VALUE;
        break;

    case RCC_SYSCLK_SRC_PLL:
        uint32_t pllIn = 0;
        uint32_t pllMul = 0;
        if (HAL_IS_BIT_SET(RCC->CFGR, RCC_CFGR_PLLSRC)) {
            if (HAL_IS_BIT_SET(RCC->CFGR, RCC_CFGR_PLLXTPRE)) {
                pllIn = HSE_VALUE / 2;
            } else {
                pllIn = HSE_VALUE;
            }
        } else {
            pllIn = HSI_VALUE / 2;
        }

        pllMul = HAL_READ_FIELD(RCC->CFGR, RCC_PLL_MUL_MASK, RCC_PLL_MUL_OFFSET);
        if (pllMul < 14) {
            pllMul += 2;
        } else {
            pllMul = 16;
        }

        dst->SYSCLK = pllIn * pllMul;

        break;

    default: break;
    }

    if (dst->SYSCLK != 0) {
        uint32_t pre = 0;
        pre = HAL_READ_FIELD(RCC->CFGR, RCC_AHB_PRE_MASK, RCC_AHB_PRE_OFFSET);
        if (pre < RCC_AHB_PRE_2) {
            dst->AHB = dst->SYSCLK;
        } else if (pre < RCC_AHB_PRE_64) {
            dst->AHB = dst->SYSCLK / (2 << (pre & 0x07));
        } else {
            dst->AHB = dst->SYSCLK / (2 << ((pre & 0x07) + 1));
        }

        pre = HAL_READ_FIELD(RCC->CFGR, RCC_APB_PRE_MASK, RCC_APB1_PRE_OFFSET);
        dst->APB1 = RCC_APB_ClockGetFreq(dst->SYSCLK, pre);

        pre = HAL_READ_FIELD(RCC->CFGR, RCC_APB_PRE_MASK, RCC_APB2_PRE_OFFSET);
        dst->APB2 = RCC_APB_ClockGetFreq(dst->SYSCLK, pre);
    }

    return HAL_SUCCESS;
}

uint32_t RCC_GetPeriphClock(uint32_t pAddr) {
    RCC_ClockFreq_t ckf;
    uint32_t ret = 0;
    if (RCC_ClockGetFreq(&ckf) == HAL_SUCCESS) {
        if (((0xA0000000 <= pAddr) && (0xA0000FFF >= pAddr)) ||
            ((0x40018000 <= pAddr) && (0x5003FFFF >= pAddr))) {
            ret = ckf.AHB;
        } else if (((0x40010000 <= pAddr) && (0x40017FFF >= pAddr))) {
            ret = ckf.APB2;
        } else if (((0x40000000 <= pAddr) && (0x4000FFFF >= pAddr))) {
            ret = ckf.APB1;
        }
    }
    return ret;
}

/*

1000 | 000 | 0 | | 0000 0000 0000 0010 | 2
1001 | 001 | 1 | | 0000 0000 0000 0100 | 4
1010 | 010 | 2 | | 0000 0000 0000 1000 | 8
1011 | 011 | 3 | | 0000 0000 0001 0000 | 16
1100 | 100 | 5 | | 0000 0000 0100 0000 | 64
1101 | 101 | 6 | | 0000 0000 1000 0000 | 128
1110 | 110 | 7 | | 0000 0001 0000 0000 | 256
1111 | 111 | 8 | | 0000 0010 0000 0000 | 512
*/


int RCC_LSE_Enable(void) {
    int timeout = RCC_TIMEOUT;
    HAL_SET_BITS(RCC->BDCR, RCC_BDCR_LSEON);
    while (HAL_READ_BITS(RCC->BDCR, RCC_BDCR_LSEON)) {
        if (timeout <= 0) {
            return HAL_FAILED;
        }
        timeout--;
    }

    HAL_MODIFY_FIELD(RCC->BDCR, RCC_RTC_CLKSRC_LSE,
        RCC_RTC_CLKSRC_MASK, RCC_RTC_CLKSRC_OFFSET);

    return HAL_SUCCESS;
}




#ifdef USART1
void RCC_USART1_EnableClock(int enable) {
    HAL_ENABLE_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN, enable)
}
#endif /* USART1 */

#ifdef USART2
void RCC_USART2_EnableClock(int enable) {
    HAL_ENABLE_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN, enable)
}
#endif /* USART2 */

#ifdef USART3
void RCC_USART3_EnableClock(int enable) {
    HAL_ENABLE_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN, enable)
}
#endif /* USART3 */

#ifdef TIM2
void RCC_TIM2_EnableClock(int enable) {
    HAL_ENABLE_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN, enable)
}
#endif


#if 0

#ifdef AFIO
void RCC_AFIO_ClockSetEnable(bool enabled);
#endif

#ifdef GPIOA
void RCC_GPIOA_ClockSetEnable(bool enabled);
#endif

#ifdef GPIOB
void RCC_GPIOB_ClockSetEnable(bool enabled);
#endif

#ifdef GPIOC
void RCC_GPIOC_ClockSetEnable(bool enabled);
#endif

#ifdef GPIOD
void RCC_GPIOD_ClockSetEnable(bool enabled);
#endif

#ifdef GPIOE
void RCC_GPIOE_ClockSetEnable(bool enabled);
#endif


#ifdef AFIO
void RCC_AFIO_ClockSetEnable(bool enabled) {}
#endif

#ifdef GPIOA
void RCC_GPIOA_ClockSetEnable(bool enabled) {}
#endif

#ifdef GPIOB
void RCC_GPIOB_ClockSetEnable(bool enabled) {}
#endif

#ifdef GPIOC
void RCC_GPIOC_ClockSetEnable(bool enabled) {}
#endif

#ifdef GPIOD
void RCC_GPIOD_ClockSetEnable(bool enabled) {}
#endif

#ifdef GPIOE
void RCC_GPIOE_ClockSetEnable(bool enabled) {}
#endif

#endif
