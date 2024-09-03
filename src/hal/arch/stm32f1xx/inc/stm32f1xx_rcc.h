/**
 * @file    stm32f1xx_rcc.h
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-20 00:01:05
 *
 * @brief
 */
#ifndef STM32F1XX_RCC_H
#define STM32F1XX_RCC_H

#include "stm32f1xx_config.h"

#ifdef BKP
#define RCC_BKP_CLOCK_ENABLE()             HAL_SET_BITS(RCC->APB1ENR, RCC_APB1ENR_BKPEN)
#define RCC_BKP_CLOCK_DISABLE()            HAL_CLEAR_BITS(RCC->APB1ENR, RCC_APB1ENR_BKPEN)
#define RCC_BKP_IS_CLOCK_ENABLED()         HAL_IS_BIT_SET(RCC->APB1ENR, RCC_APB1ENR_BKPEN)
#endif

#ifdef PWR
#define RCC_PWR_CLOCK_ENABLE()             HAL_SET_BITS(RCC->APB1ENR, RCC_APB1ENR_PWREN)
#define RCC_PWR_CLOCK_DISABLE()            HAL_CLEAR_BITS(RCC->APB1ENR, RCC_APB1ENR_PWREN)
#define RCC_PWR_IS_CLOCK_ENABLED()         HAL_IS_BIT_SET(RCC->APB1ENR, RCC_APB1ENR_PWREN)
#endif

#ifdef AFIO
#define RCC_AFIO_CLOCK_ENABLE()             HAL_SET_BITS(RCC->APB2ENR, RCC_APB2ENR_AFIOEN)
#define RCC_AFIO_CLOCK_DISABLE()            HAL_CLEAR_BITS(RCC->APB2ENR, RCC_APB2ENR_AFIOEN)
#define RCC_AFIO_IS_CLOCK_ENABLED()         HAL_IS_BIT_SET(RCC->APB2ENR, RCC_APB2ENR_AFIOEN)
#endif

#ifdef GPIOA
#define RCC_GPIOA_CLOCK_ENABLE()            HAL_SET_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPAEN)
#define RCC_GPIOA_CLOCK_DISABLE()           HAL_CLEAR_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPAEN)
#define RCC_GPIOA_IS_CLOCK_ENABLED()        HAL_IS_BIT_SET(RCC->APB2ENR, RCC_APB2ENR_IOPAEN)
#endif

#ifdef GPIOB
#define RCC_GPIOB_CLOCK_ENABLE()            HAL_SET_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPBEN)
#define RCC_GPIOB_CLOCK_DISABLE()           HAL_CLEAR_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPBEN)
#define RCC_GPIOB_IS_CLOCK_ENABLED()        HAL_IS_BIT_SET(RCC->APB2ENR, RCC_APB2ENR_IOPBEN)
#endif

#ifdef GPIOC
#define RCC_GPIOC_CLOCK_ENABLE()            HAL_SET_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPCEN)
#define RCC_GPIOC_CLOCK_DISABLE()           HAL_CLEAR_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPCEN)
#define RCC_GPIOC_IS_CLOCK_ENABLED()        HAL_IS_BIT_SET(RCC->APB2ENR, RCC_APB2ENR_IOPCEN)
#endif

#ifdef GPIOD
#define RCC_GPIOD_CLOCK_ENABLE()            HAL_SET_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPDEN)
#define RCC_GPIOD_CLOCK_DISABLE()           HAL_CLEAR_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPDEN)
#define RCC_GPIOD_IS_CLOCK_ENABLED()        HAL_IS_BIT_SET(RCC->APB2ENR, RCC_APB2ENR_IOPDEN)
#endif

#ifdef GPIOE
#define RCC_GPIOE_CLOCK_ENABLE()            HAL_SET_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPEEN)
#define RCC_GPIOE_CLOCK_DISABLE()           HAL_CLEAR_BITS(RCC->APB2ENR, RCC_APB2ENR_IOPEEN)
#define RCC_GPIOE_IS_CLOCKE_NABLED()        HAL_IS_BIT_SET(RCC->APB2ENR, RCC_APB2ENR_IOPEEN)
#endif

#define RCC_RTC_ENABLE()                    HAL_SET_BITS(RCC->BDCR, RCC_BDCR_RTCEN)

/*  */
#define RCC_SYSCLK_SRC_HSI                  (uint32_t)0U
#define RCC_SYSCLK_SRC_HSE                  (uint32_t)1U
#define RCC_SYSCLK_SRC_PLL                  (uint32_t)2U
#define RCC_SYSCLK_SRC_MASK                 (uint32_t)0x03U
#define RCC_SYSCLK_SRC_SW_OFFSET            (uint32_t)0U
#define RCC_SYSCLK_SRC_SWS_OFFSET           (uint32_t)2U

/*  */
#define RCC_PLL_SRC_HSI_2                   (uint32_t)0U
#define RCC_PLL_SRC_HSE                     (uint32_t)1U

/*  */
#define RCC_MCO_DISABLED                    (uint32_t)0U
#define RCC_MCO_CLKSRC_PLL                  (uint32_t)4U
#define RCC_MCO_CLKSRC_HSI                  (uint32_t)5U
#define RCC_MCO_CLKSRC_HSE                  (uint32_t)6U
#define RCC_MCO_CLKSRC_SYS                  (uint32_t)7U
#define RCC_MCO_CLKSRC_MASK                 (uint32_t)0x07U
#define RCC_MCO_CLKSRC_OFFSET               (uint32_t)24U

/*  */
#define RCC_HSE_PLL_DIV_1                   (uint32_t)0U
#define RCC_HSE_PLL_DIV_2                   (uint32_t)1U

/*  */
#define RCC_PLL_MUL_2                       (uint32_t)0U
#define RCC_PLL_MUL_3                       (uint32_t)1U
#define RCC_PLL_MUL_4                       (uint32_t)2U
#define RCC_PLL_MUL_5                       (uint32_t)3U
#define RCC_PLL_MUL_6                       (uint32_t)4U
#define RCC_PLL_MUL_7                       (uint32_t)5U
#define RCC_PLL_MUL_8                       (uint32_t)6U
#define RCC_PLL_MUL_9                       (uint32_t)7U
#define RCC_PLL_MUL_10                      (uint32_t)8U
#define RCC_PLL_MUL_11                      (uint32_t)9U
#define RCC_PLL_MUL_12                      (uint32_t)10U
#define RCC_PLL_MUL_13                      (uint32_t)11U
#define RCC_PLL_MUL_14                      (uint32_t)12U
#define RCC_PLL_MUL_15                      (uint32_t)13U
#define RCC_PLL_MUL_16                      (uint32_t)14U
#define RCC_PLL_MUL_MASK                    (uint32_t)0x0FU
#define RCC_PLL_MUL_OFFSET                  (uint32_t)18

/*  */
#define RCC_AHB_PRE_NONE                    (uint32_t)0U
#define RCC_AHB_PRE_2                       (uint32_t)8U
#define RCC_AHB_PRE_4                       (uint32_t)9U
#define RCC_AHB_PRE_8                       (uint32_t)10U
#define RCC_AHB_PRE_16                      (uint32_t)11U
#define RCC_AHB_PRE_64                      (uint32_t)12U
#define RCC_AHB_PRE_128                     (uint32_t)13U
#define RCC_AHB_PRE_256                     (uint32_t)14U
#define RCC_AHB_PRE_512                     (uint32_t)15U
#define RCC_AHB_PRE_MASK                    (uint32_t)0x0FU
#define RCC_AHB_PRE_OFFSET                  (uint32_t)4U

/*  */
#define RCC_APB_PRE_NONE                    (uint32_t)0U
#define RCC_APB_PRE_2                       (uint32_t)4U
#define RCC_APB_PRE_4                       (uint32_t)5U
#define RCC_APB_PRE_8                       (uint32_t)6U
#define RCC_APB_PRE_16                      (uint32_t)7U
#define RCC_APB_PRE_MASK                    (uint32_t)0x07U
#define RCC_APB1_PRE_OFFSET                 (uint32_t)8U
#define RCC_APB2_PRE_OFFSET                 (uint32_t)11U

/*  */
#define RCC_USB_CLK_DIV_1                   (uint32_t)0U
#define RCC_USB_CLK_DIV_1_5                 (uint32_t)1U

/*  */
#define RCC_ADC_CLK_DIV_2                   (uint32_t)0U
#define RCC_ADC_CLK_DIV_4                   (uint32_t)1U
#define RCC_ADC_CLK_DIV_6                   (uint32_t)2U
#define RCC_ADC_CLK_DIV_8                   (uint32_t)3U
#define RCC_ADC_CLK_DIV_MASK                (uint32_t)0x03U
#define RCC_ADC_CLK_DIV_OFFSET              (uint32_t)14U

/*  */
#define RCC_RTC_CLKSRC_NONE                 (uint32_t)0U
#define RCC_RTC_CLKSRC_LSE                  (uint32_t)1U
#define RCC_RTC_CLKSRC_LSI                  (uint32_t)2U
#define RCC_RTC_CLKSRC_HSE_128              (uint32_t)3U
#define RCC_RTC_CLKSRC_MASK                 (uint32_t)0x03U
#define RCC_RTC_CLKSRC_OFFSET               (uint32_t)8U


typedef struct hal_rcc_clock_config {
    uint32_t sysclk_src;
    uint32_t pll_src;
    uint32_t mco_src;
    uint32_t hse_div;
    uint32_t pll_mul;
    uint32_t ahb_pre;
    uint32_t apb1_pre;
    uint32_t apb2_pre;
} hal_rcc_clock_config_t;

typedef struct hal_rcc_clock_freq {
    uint32_t SYSCLK;
    uint32_t AHB;
    uint32_t APB1;
    uint32_t APB2;
} hal_rcc_clock_freq_t;

typedef void (*hal_rcc_cken_func_t)(int);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int hal_rcc_set_clock_config(hal_rcc_clock_config_t* config);
int hal_rcc_clock_get_freq(hal_rcc_clock_freq_t* dst);
uint32_t hal_rcc_get_periph_clock(uint32_t pAddr);
int hal_rcc_lse_enable(void);

#ifdef USART1
void hal_rcc_usart1_enable_clock(int enable);
#endif /* USART1 */

#ifdef USART2
void hal_rcc_usart2_enable_clock(int enable);
#endif /* USART2 */

#ifdef USART3
void hal_rcc_usart3_enable_clock(int enable);
#endif /* USART3 */

#ifdef TIM2
void hal_rcc_tim2_enable_clock(int enable);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_RCC_H */
/* EOF */
