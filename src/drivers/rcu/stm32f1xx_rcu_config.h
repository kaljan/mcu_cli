/**
 * @file    stm32f1xx_rcu_config.h
 * @author  kaljan
 * @version 1.0.0
 * @date    01-12-23
 *
 * @brief
 */
#ifndef STM32F1XX_RCU_CONFIG_H
#define STM32F1XX_RCU_CONFIG_H

/* RCU data pulses config */
/* RCU start bit pulse config */
#define RCU_START_PERIOD_MIN               11000 //13000
#define RCU_START_PERIOD_MAX               15000 //14200

#define RCU_START_PULSE_MIN                8000//8800
#define RCU_START_PULSE_MAX                10000//9700

/* RCU data pulse config */
#define RCU_BIT_PULSE_MIN                  500//530
#define RCU_BIT_PULSE_MAX                  700//670

#define RCU_VALUE0_MIN                     1000//1100
#define RCU_VALUE0_MAX                     1500//1200

#define RCU_VALUE1_MIN                     1600 //1800
#define RCU_VALUE1_MAX                     2500//2300

/* RCU data bytes per comand */
#define RCU_DATA_WIDTH                     4


#define RCU_GPIO_PORT                  GPIOB
#define RCU_GPIO_PIN                   7U

#define RCU_GPIO_AF_REMAP()

#define RCU_TIMER                      TIM4
#define RCU_CAPTURE_CHANNEL1           1 // CHANNEL2
#define RCU_CAPTURE_CHANNEL2           0 // CHANNEL1
#define RCU_TIM_CC_ENABLE_IT(TIMx)     HAL_SET_BITS(TIMx->DIER, TIM_DIER_CC2IE) /*LL_TIM_EnableIT_CC2(TIMx)*/
#define RCU_TIM_IRQ_HANDLER            TIM4_IRQHandler
#define RCU_TIM_IRQ_N                  TIM4_IRQn
#define RCU_TRIG_SEL                   (TIM_SMCR_TS_2 | TIM_SMCR_TS_1)

#define RCU_TIM_ENABLE_CLOCK()          RCC->APB1ENR |= RCC_APB1ENR_TIM4EN /* LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4) */

#define RCU_TIM_PSC_VALUE               71

#define RCU_PERIOD_CC_IF_GET(TIMx)      HAL_IS_BIT_SET(TIMx->SR, TIM_SR_CC2IF) /* LL_TIM_IsActiveFlag_CC2(TIMx) */
#define RCU_PERIOD_CC_IF_CLEAR(TIMx)    HAL_WRITE_REG(TIMx->SR, ~(TIM_SR_CC2IF)) /* LL_TIM_ClearFlag_CC2(TIMx) */
#define RCU_PERIOD_CC_GET_VALUE(TIMx)   TIMx->CCR2 /* LL_TIM_IC_GetCaptureCH2(TIMx) */

#define RCU_PW_CC_IF_GET(TIMx)          HAL_IS_BIT_SET(TIMx->SR, TIM_SR_CC1IF) /* LL_TIM_IsActiveFlag_CC1(TIMx) */
#define RCU_PW_CC_IF_CLEAR(TIMx)        HAL_WRITE_REG(TIMx->SR, ~(TIM_SR_CC1IF)) /* LL_TIM_ClearFlag_CC1(TIMx) */
#define RCU_PW_CC_GET_VALUE(TIMx)       TIMx->CCR1 /* LL_TIM_IC_GetCaptureCH1(TIMx) */

#endif /* STM32F1XX_RCU_CONFIG_H */
/* EOF */
