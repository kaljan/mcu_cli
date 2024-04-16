/**
 * @file    hal_gpio.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-06
 *
 * @brief
 */
#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "hal_config.h"

#if (HAL_GPIO_ENABLE == HAL_ENABLED)

#define HAL_GPIO_DIR_OUT    0
#define HAL_GPIO_DIR_IN     1

#define HAL_GPIO_LOW        0
#define HAL_GPIO_HIGH       1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

hal_object_t hal_gpio_pin_get(int id);
void hal_gpio_pin_set_dir(hal_object_t ctx, int dir);
int hal_gpio_pin_get_dir(hal_object_t ctx);
void hal_gpio_pin_set(hal_object_t ctx);
void hal_gpio_pin_reset(hal_object_t ctx);
void hal_gpio_pin_toggle(hal_object_t ctx);
void hal_gpio_pin_set_value(hal_object_t ctx, int value);
int hal_gpio_pin_get_out_value(hal_object_t ctx);
int hal_gpio_pin_get_in_value(hal_object_t ctx);
int hal_gpio_pin_debounce_in(hal_object_t ctx);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* (HAL_GPIO_ENABLE == HAL_ENABLED) */
#endif /* HAL_GPIO_H */
