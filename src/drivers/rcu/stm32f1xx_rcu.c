/**
 * @file     stm32f1xx_rcu.c
 * @author   kaljan
 * @version  0.0.3
 * @date     Aug 16, 2020
 *
 * @brief
 */

#include "drivers_rcu.h"
#include "stm32f1xx_rcu_config.h"

#include "hal_gpio.h"
#include "stm32f1xx.h"
#include "stm32f1xx_gpio.h"

static uint32_t m_RCU_Period;
static uint32_t m_RCU_PulseWidth;

static uint32_t m_RCU_CurrentBitMask = 0x00000001UL;
static uint32_t m_RCU_Event = 0;

static bool m_RCU_StartReceived = false;
static bool m_RCU_KeyPressed = false;

#define PWM_CHECK_PULSE(value, min, max) \
    (((value) >= (min)) && ((value) <= (max)))

void RCU_TIM_IRQ_HANDLER(void) {
    /* Get period */
    if (RCU_PERIOD_CC_IF_GET(RCU_TIMER)) {
        RCU_PERIOD_CC_IF_CLEAR(RCU_TIMER);
        m_RCU_Period = RCU_PERIOD_CC_GET_VALUE(RCU_TIMER);
    }

    /* Get pulse width */
    if (RCU_PW_CC_IF_GET(RCU_TIMER)) {
        RCU_PW_CC_IF_CLEAR(RCU_TIMER);
        m_RCU_PulseWidth = RCU_PW_CC_GET_VALUE(RCU_TIMER);
    }

    /* Check if start bit is received */
    if (PWM_CHECK_PULSE(m_RCU_Period, RCU_START_PERIOD_MIN, RCU_START_PERIOD_MAX) &&
        PWM_CHECK_PULSE(m_RCU_PulseWidth, RCU_START_PULSE_MIN, RCU_START_PULSE_MAX)) {
        m_RCU_StartReceived = true;
        m_RCU_CurrentBitMask = 0x00000001UL;
        m_RCU_Event = 0;
        return;
    }

    if (m_RCU_StartReceived) {
        /* Check main pulse */
        if (PWM_CHECK_PULSE(m_RCU_PulseWidth, RCU_BIT_PULSE_MIN, RCU_BIT_PULSE_MAX)) {
            if (PWM_CHECK_PULSE(m_RCU_Period, RCU_VALUE1_MIN, RCU_VALUE1_MAX)) {
                m_RCU_Event |= m_RCU_CurrentBitMask;
            } else if (!PWM_CHECK_PULSE(m_RCU_Period, RCU_VALUE0_MIN, RCU_VALUE0_MAX)) {
                m_RCU_StartReceived = false;
                return;
            }

            if (m_RCU_CurrentBitMask & 0x80000000) {
                m_RCU_CurrentBitMask = 0x00000001UL;
                m_RCU_StartReceived = false;
                m_RCU_KeyPressed = true;
                return;
            }
            m_RCU_CurrentBitMask <<= 1;
        } else {
            m_RCU_StartReceived = false;
        }
    }
}

static RCU_EventCallback_t m_RCU_Callback = NULL;
static void* m_RCU_Context = NULL;

void RCU_SetCallback(RCU_EventCallback_t callback, void* context) {
    m_RCU_Callback = callback;
    m_RCU_Context = context;
}

static void TIM_IC_SetActiveInput(TIM_TypeDef* TIMx, uint32_t channel, uint32_t value) {
    if (HAL_IS_BIT_SET(channel, 0x02U)) {
        HAL_MODIFY_FIELD(TIMx->CCMR2, value, 0x03U, (channel & 0x01U) * 8U);
    } else {
        HAL_MODIFY_FIELD(TIMx->CCMR1, value, 0x03U, (channel & 0x01U) * 8U);
    }
}

static void TIM_IC_SetPolarity(TIM_TypeDef* TIMx, uint32_t channel, uint32_t value) {
    HAL_MODIFY_FIELD(TIMx->CCER, value, 0x01U, (4U * channel) + 1);
}

static void TIM_IC_EnableChannel(TIM_TypeDef* TIMx, uint32_t channel) {
    HAL_SET_BITS(TIMx->CCER, 1 << (4U * channel));
}


void RCU_Initialize(void) {

    RCU_TIM_ENABLE_CLOCK();

    GPIO_PinConfig_t gpio_config = {
        .mode = GPIO_PIN_MODE_INPUT,
        .speed = GPIO_PIN_SPEED_HIGH,
        .outType = GPIO_PIN_OTYPE_PUSH_PULL,
        .pull = GPIO_PIN_PULL_UP
    };

    GPIO_PinSetConfig(RCU_GPIO_PORT, RCU_GPIO_PIN, &gpio_config);

    RCU_GPIO_AF_REMAP();

    RCU_TIMER->ARR = 0x0000FFFF;
    RCU_TIMER->PSC = RCU_TIM_PSC_VALUE;

    HAL_SET_BITS(RCU_TIMER->EGR, TIM_EGR_UG);

    HAL_MODIFY_REG(RCU_TIMER->SMCR, TIM_SMCR_SMS_2, TIM_SMCR_SMS);

    HAL_MODIFY_REG(RCU_TIMER->SMCR, (TIM_SMCR_TS_2 | TIM_SMCR_TS_1), TIM_SMCR_TS);

    TIM_IC_SetActiveInput(RCU_TIMER, RCU_CAPTURE_CHANNEL1, TIM_CCMR1_CC1S_0);

    TIM_IC_SetActiveInput(RCU_TIMER, RCU_CAPTURE_CHANNEL2, TIM_CCMR1_CC1S_1);

    TIM_IC_SetPolarity(RCU_TIMER, RCU_CAPTURE_CHANNEL1, 1);

    TIM_IC_SetPolarity(RCU_TIMER, RCU_CAPTURE_CHANNEL2, 0);

    TIM_IC_EnableChannel(RCU_TIMER, RCU_CAPTURE_CHANNEL1);
    TIM_IC_EnableChannel(RCU_TIMER, RCU_CAPTURE_CHANNEL2);

    RCU_TIM_CC_ENABLE_IT(RCU_TIMER);

    HAL_SET_BITS(RCU_TIMER->CR1, TIM_CR1_CEN);

    NVIC_EnableIRQ(RCU_TIM_IRQ_N);
}

static inline int32_t RCU_DecodeData(uint32_t data) {
    if (((data & 0xFF000000UL) >> 8) == ((~data) & 0x00FF0000UL)) {
        if (((data & 0x0000FF00UL) >> 8) == ((~data) & 0x000000FFUL)) {
            return ((data & 0x000000FFUL) << 8) | ((data & 0x00FF0000UL) >> 16);
        } else {
            return ((data & 0x0000FF00UL) | ((data & 0x000000FFUL) << 16)) |
                ((data & 0x00FF0000UL) >> 16);
        }
    }
    return -1;
}

void RCU_Update(void) {
    if (m_RCU_KeyPressed) {
        int32_t cmd = RCU_DecodeData(m_RCU_Event);
        m_RCU_KeyPressed = false;
        if ((0 <= cmd) && (NULL != m_RCU_Callback)) {
            m_RCU_Callback(m_RCU_Context,(uint32_t)cmd);
        }
    }
}

int RCU_KeyToDigit(uint32_t key) {
    switch (key) {
    case RM_L1162_KEY_CODE_0:
    case RCU_KEY_CODE_0: return 0;
    case RM_L1162_KEY_CODE_1:
    case RCU_KEY_CODE_1: return 1;
    case RM_L1162_KEY_CODE_2:
    case RCU_KEY_CODE_2: return 2;
    case RM_L1162_KEY_CODE_3:
    case RCU_KEY_CODE_3: return 3;
    case RM_L1162_KEY_CODE_4:
    case RCU_KEY_CODE_4: return 4;
    case RM_L1162_KEY_CODE_5:
    case RCU_KEY_CODE_5: return 5;
    case RM_L1162_KEY_CODE_6:
    case RCU_KEY_CODE_6: return 6;
    case RM_L1162_KEY_CODE_7:
    case RCU_KEY_CODE_7: return 7;
    case RM_L1162_KEY_CODE_8:
    case RCU_KEY_CODE_8: return 8;
    case RM_L1162_KEY_CODE_9:
    case RCU_KEY_CODE_9: return 9;
    default: return -1;
    }
}


