/**
 * @file    stm32f1xx_gpio.h
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-24
 *
 * @brief
 */
#ifndef STM32F1XX_GPIO_H
#define STM32F1XX_GPIO_H

#include "stm32f1xx_config.h"


#define GPIO_MODE_INPUT                  (uint32_t)0x00U
#define GPIO_MODE_OUTPUT_MEDIUM_SPEED    (uint32_t)0x01U
#define GPIO_MODE_OUTPUT_LOW_SPEED       (uint32_t)0x02U
#define GPIO_MODE_OUTPUT_HIGH_SPEED      (uint32_t)0x03U
#define GPIO_MODE_MASK                   (uint32_t)0x03U

#define GPIO_CNF_ANALOG                  (uint32_t)0x00U
#define GPIO_CNF_FLOATING                (uint32_t)0x01U
#define GPIO_CNF_PUPD                    (uint32_t)0x02U

#define GPIO_CNF_OUTPUT                  (uint32_t)0x00U
#define GPIO_CNF_ALTERNATE               (uint32_t)0x02U
#define GPIO_CNF_PUSHPULL                (uint32_t)0x00U
#define GPIO_CNF_OPENDRAIN               (uint32_t)0x01U

#define GPIO_CNF_MASK                    (uint32_t)0x03U
#define GPIO_CNF_OFFSET                  (uint32_t)2U


#define GPIO_PIN_ASSERT(ptr) ((NULL != ptr) && (NULL != ptr->port) && (ptr->pin < 16))

#define GPIO_CNF_MODE(cnf, mode) \
    (((cnf) & GPIO_CNF_MASK) << GPIO_CNF_OFFSET) | \
    ((mode) & GPIO_MODE_MASK)


#define GPIO_SET(port, value)                   HAL_WRITE_REG((port)->BSRR, (value) & 0xFFFF)
#define GPIO_RESET(port, value)                 HAL_WRITE_REG((port)->BRR, (value) & 0xFFFF)
#define GPIO_PIN_SET(port, pin)                 GPIO_SET(port, 1U << (pin))
#define GPIO_PIN_RESET(port, pin)               GPIO_RESET(port, 1U << (pin))
#define GPIO_IS_OUTPUT_PIN_SET(port, pin)       HAL_IS_BIT_SET((port)->ODR, (1U << (pin)))
#define GPIO_IS_OUTPUT_PIN_RESET(port, pin)     HAL_IS_BIT_CLEAR((port)->ODR, (1U << (pin)))
#define GPIO_IS_INPUT_PIN_SET(port, pin)        HAL_IS_BIT_SET((port)->IDR, (1U << (pin)))
#define GPIO_IS_INPUT_PIN_RESET(port, pin)      HAL_IS_BIT_CLEAR((port)->IDR, (1U << (pin)))

#define GPIO_PIN_MODE_ANALOG                (uint32_t)0U
#define GPIO_PIN_MODE_INPUT                 (uint32_t)1U
#define GPIO_PIN_MODE_OUTPUT                (uint32_t)2U
#define GPIO_PIN_MODE_ALTERNATE             (uint32_t)3U
#define GPIO_PIN_MODE_MASK                  (uint32_t)0x03U

#define GPIO_PIN_SPEED_MEDIUM               (uint32_t)1U
#define GPIO_PIN_SPEED_LOW                  (uint32_t)2U
#define GPIO_PIN_SPEED_HIGH                 (uint32_t)3U
#define GPIO_PIN_SPEED_MASK                 (uint32_t)0x03U

#define GPIO_PIN_OTYPE_PUSH_PULL            (uint32_t)0U
#define GPIO_PIN_OTYPE_OPEN_DRAIN           (uint32_t)1U

#define GPIO_PIN_PULL_NO                    (uint32_t)0U
#define GPIO_PIN_PULL_UP                    (uint32_t)1U
#define GPIO_PIN_PULL_DOWN                  (uint32_t)2U
#define GPIO_PIN_PULL_MASK                  (uint32_t)0x03U

typedef struct hal_gpio_pin_config {
    uint32_t mode;
    uint32_t speed;
    uint32_t outType;
    uint32_t pull;
} hal_gpio_pin_config_t;

typedef struct hal_gpio_pin {
    GPIO_TypeDef* port;
    uint32_t pin;
} hal_gpio_pin_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int hal_gpio_pin_set_config(GPIO_TypeDef* port, uint32_t pin, const hal_gpio_pin_config_t* config);
int hal_gpio_pin_get_config(GPIO_TypeDef* port, uint32_t pin, hal_gpio_pin_config_t* config);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_GPIO_H */
/* EOF */
