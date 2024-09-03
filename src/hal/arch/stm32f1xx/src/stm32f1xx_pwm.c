/**
 * @file    stm32f1xx_pwm.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-08-09
 *
 * @brief
 */
#include "stm32f1xx_pwm.h"
#include "stm32f1xx.h"
#include "stm32f1xx_rcc.h"
#include "stm32f1xx_dma.h"
#include "hal_pwm.h"

struct hal_pwm {
    uint32_t flags;
    uint32_t channel_no;
    TIM_TypeDef* handle;
    hal_pwm_irq_t h_pwm_irq;
    hal_pwm_irq_t h_tim_up_irq;
    hal_dma_channel_t* dma;
    const uint16_t* buf;
    uint32_t buf_size;
};

/*

T0H 220ns..380ns
T0L 580ns..1us
T1H 580ns..1us
T1L 220ns..420ns
RES 280us



10'000'000 Hz
10 MHz
0.1 us
100ns
F_{TIM} = 72'000'000
*/

/*
00
01
10
11
*/
static void hal_pwm_set_channel(hal_pwm_t* context) {
    // context->handle->CCMR1
    uint32_t offset = ((context->channel_no & 0x1U) * 8) + 4;
    if (context->channel_no & 0x02) {
        HAL_MODIFY_FIELD(context->handle->CCMR2, 0x06, 0x07, offset);
    } else {
        HAL_MODIFY_FIELD(context->handle->CCMR1, 0x06, 0x07, offset);
    }
}

int hal_pwm_init(hal_pwm_t* context, hal_pwm_config_t* config) {
    uint32_t tim_clock = 0;
    uint32_t prescaller = 0;
    uint32_t autoreload = 0;
    uint32_t fck_div = 0;

    if ((context == NULL) || (config == NULL) || (context->handle == NULL)) {
        return -1;
    }

    tim_clock = hal_rcc_get_periph_clock(context->handle);
    if (tim_clock == 0) {
        return -1;
    }

    // config->freq_hz
    prescaller = tim_clock / config->freq_hz - 1;
    while ((prescaller > 65535) && (fck_div < 3)) {
        fck_div++;
        HAL_MODIFY_FIELD(context->handle->CR1, fck_div, 0x03, TIM_CR1_CKD_Pos);
    }

    if (prescaller > 65535) {
        return -1;
    }

    if (config->freq_hz > 1000000) {
        autoreload = config->period_ns / (1000 / (config->freq_hz / 1000000));
    } else if (config->freq_hz > 1000) {
        autoreload = config->period_ns / ((1000 / (config->freq_hz / 1000)) * 1000);
    } else {
        autoreload = config->period_ns / ((1000 / config->freq_hz) * 1000000);
    }

    context->handle->PSC = prescaller;
    context->handle->ARR = autoreload;

    HAL_SET_BITS(context->handle->CR1, TIM_CR1_ARPE | TIM_CR1_URS);

#if 0
	RCC->APB1ENR |=
		RCC_APB1ENR_TIM3EN;

	TIM3->PSC = 0;
	TIM3->ARR = 89;
	TIM3->CCR3 = 50;

	TIM3->CR1 |=
		TIM_CR1_ARPE |
		TIM_CR1_URS;

	TIM3->CR2 |= TIM_CR2_CCDS;

	TIM3->CCMR2 = 0;
	TIM3->CCMR2 |=
		TIM_CCMR2_OC3M_2 |
		TIM_CCMR2_OC3M_1;

	TIM3->CCER = 0;

	TIM3->SMCR |= TIM_SMCR_ETP;

	// Настроим DMA для таймера
	// Запрос к DMA когда таймер обнавляется

	// Разрешим запрос к DMA от CC1
	TIM3->DIER |= TIM_DIER_CC3DE;//TIM_DIER_UDE;//

	// Разрешить выход
	TIM3->CCER |=
		TIM_CCER_CC3E;

	// Update event
	// DMA Request Enable
	/*
	 * Нужно перенастроить таймер на DMA
	 * Для таймера 15 используется DMA1 канал 5
	 * Разрешить DMA1
	 */
	 // Настройка DMA
	 // Адрес в памяти
	 DMA1_Channel2->CMAR = (unsigned int)(&led_strip_pwm[0][0]);
	 DMA1_Channel2->CPAR = TIM3_BASE + 0x3C;
	 DMA1_Channel2->CNDTR = 24;
http://localhost:1234
	 DMA1_Channel2->CCR |=
		DMA_CCR2_MSIZE_0 |
		DMA_CCR2_PSIZE_0 |
		DMA_CCR2_MINC |
		DMA_CCR2_CIRC |
		DMA_CCR2_DIR |
		DMA_CCR2_TCIE |
		DMA_CCR2_EN;

	// Сбросим флаги прерывание DMA
	DMA1->IFCR |= 0x000000F0;

	// Разрешим приерывание от DMA
	NVIC->ISER[0] |= 0x00001000;
#endif
    return 0;
}

void hal_pwm_deinit(hal_pwm_t* context);
void hal_pwm_set_callback(hal_pwm_t* context, hal_callback_t callback);
int hal_pwm_pulse(hal_pwm_t* context, uint32_t width, uint32_t count);
int hal_pwm_set_pulse_buffer(hal_pwm_t* context, const uint16_t* width, uint32_t count);

hal_pwm_t* hal_pwm_get_instance(int id) {
    return NULL;
}


void hal_pwm_irq_handler(hal_pwm_t* context, uint16_t index) {
    if ((context != NULL) && (context->handle != NULL) &&
        (index < context->pwm_irq_list_size)) {
        TIM_TypeDef* tim_n = (TIM_TypeDef*)context->handle;
        // tim_n->SR &


    }

}

