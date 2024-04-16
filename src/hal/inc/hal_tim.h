/**
 * @file    hal_tim.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-06
 *
 * @brief
 */
#ifndef HAL_TIM_H
#define HAL_TIM_H

#include "hal_config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void hal_sys_time_init(void);
uint64_t hal_sys_time_now_us(void);
uint64_t hal_sys_time_now_ms(void);
void hal_sys_time_delay_us(uint64_t delay);
void hal_sys_time_delay_ms(uint64_t delay);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_TIM_H */
