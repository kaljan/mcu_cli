/**
 * @file    hal_pwm.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-15
 *
 * @brief
 */
#ifndef HAL_PWM_H
#define HAL_PWM_H

#include "hal_config.h"

#define HAL_PWM_CH1_EN      0x0010U
#define HAL_PWM_CH2_EN      0x0020U
#define HAL_PWM_CH3_EN      0x0040U
#define HAL_PWM_CH4_EN      0x0080U
#define HAL_PWM_DMA_EN      0x0200U
#define HAL_PWM_HPOL_FLAG   0x0400U

typedef struct hal_pwm_config {
    uint32_t freq_hz;
    uint32_t period_ns;
    uint32_t flags;
} hal_pwm_config_t;

typedef struct hal_pwm_irq {
    void* object;
    void (*handler)(void*);
} hal_pwm_irq_t;

struct hal_pwm;
typedef struct hal_pwm hal_pwm_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

hal_pwm_t* hal_pwm_get_instance(int id);
int hal_pwm_init(hal_pwm_t* context, hal_pwm_config_t* cfg);
void hal_pwm_deinit(hal_pwm_t* context);
void hal_pwm_set_callback(hal_pwm_t* context, hal_callback_t callback);
int hal_pwm_pulse(hal_pwm_t* context, uint32_t width, uint32_t count);
int hal_pwm_set_pulse_buffer(hal_pwm_t* context, const uint16_t* width, uint32_t count);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_PWM_H */
