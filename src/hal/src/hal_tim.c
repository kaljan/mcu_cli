/**
 * @file    hal_tim.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-06
 *
 * @brief
 */
#include "hal_tim.h"


uint64_t hal_sys_time_now_ms(void) {
    return hal_sys_time_now_us() / 1000;
}

void hal_sys_time_delay_us(uint64_t delay) {
    uint64_t timestamp = hal_sys_time_now_us();
    while (hal_sys_time_now_us() - timestamp < delay);
}

void hal_sys_time_delay_ms(uint64_t delay) {
    uint64_t timestamp = hal_sys_time_now_ms();
    while (hal_sys_time_now_ms() - timestamp < delay);
}