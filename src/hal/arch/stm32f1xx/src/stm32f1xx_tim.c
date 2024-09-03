/**
 * @file    stm32f1xx_tim.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-03-02
 *
 * @brief
 */
#include "stm32f1xx_tim.h"
#include "stm32f1xx_rcc.h"
#include "hal_tim.h"

#define TIM_CKD_TCKINT      0U
#define TIM_CKD_2TCKINT     1U
#define TIM_CKD_4TCKINT     2U

#define TIM_CMS_EDGE        0U
#define TIM_CMS_CENTER1     1U
#define TIM_CMS_CENTER2     2U
#define TIM_CMS_CENTER3     3U

#define TIM_MMS_RESET       0U
#define TIM_MMS_ENABLE      1U
#define TIM_MMS_UPDATE      2U
#define TIM_MMS_CMP_PUILSE  3U
#define TIM_MMS_CMP_OC1REF  4U
#define TIM_MMS_CMP_OC2REF  5U
#define TIM_MMS_CMP_OC3REF  6U
#define TIM_MMS_CMP_OC4REF  7U


/* System timer */

#define SYS_TIMER                   TIM2
#define SYS_TIMER_IRQ               TIM2_IRQn
#define SYS_TIMER_IRQ_HANDLER       TIM2_IRQHandler
#define SYS_TIMER_ENABLE_CLOCK(e)   hal_rcc_tim2_enable_clock(e)

static uint64_t m_time = 0;

void hal_sys_time_init(void) {
    if (HAL_IS_BIT_SET(SYS_TIMER->CR1, TIM_CR1_CEN)) {
        return;
    }
    SYS_TIMER_ENABLE_CLOCK(1);

    SYS_TIMER->PSC = 71;
    SYS_TIMER->ARR = 0xFFFF;
    SYS_TIMER->CNT = 0;
    SYS_TIMER->EGR |= TIM_EGR_UG;

    while ((SYS_TIMER->SR & TIM_SR_UIF) != TIM_SR_UIF);

    SYS_TIMER->SR &= ~(TIM_SR_UIF);
    SYS_TIMER->DIER |= TIM_DIER_UIE;

    NVIC_EnableIRQ(SYS_TIMER_IRQ);

    SYS_TIMER->CR1 |= TIM_CR1_CEN;
}

uint64_t hal_sys_time_now_us(void) {
    return m_time + SYS_TIMER->CNT;
}

void SYS_TIMER_IRQ_HANDLER(void) {
    if ((SYS_TIMER->SR & TIM_SR_UIF) == TIM_SR_UIF) {
        SYS_TIMER->SR &= ~(TIM_SR_UIF);
        m_time += 0xFFFF;
    }
}

