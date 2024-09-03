/**
 * @file    stm32f1xx_gpio.c
 * @author  kaljan
 * @version 1.0.0
 * @date    27-01-20 00:00:47
 *
 * @brief
 */

#include "hal_gpio.h"
#include "stm32f1xx_gpio.h"
#include <string.h>

int hal_gpio_pin_set_config(GPIO_TypeDef* port, uint32_t pin, const hal_gpio_pin_config_t* config) {
    if ((NULL == port) || (NULL == config)) {
        return HAL_FAILED;
    }

    uint32_t temp = 0;
    if (config->mode == GPIO_PIN_MODE_ANALOG) {
        temp = GPIO_CNF_MODE(GPIO_CNF_ANALOG, GPIO_MODE_INPUT);
    } else if (config->mode == GPIO_PIN_MODE_INPUT) {
        if (config->pull == GPIO_PIN_PULL_NO) {
            temp = GPIO_CNF_MODE(GPIO_CNF_FLOATING, GPIO_MODE_INPUT);
        } else {
            temp = GPIO_CNF_MODE(GPIO_CNF_PUPD, GPIO_MODE_INPUT);
            if (config->pull == GPIO_PIN_PULL_UP) {
                GPIO_PIN_SET(port, pin);
            } else {
                GPIO_PIN_RESET(port, pin);
            }
        }
    } else {
        if (config->mode == GPIO_PIN_MODE_ALTERNATE) {
            temp = GPIO_CNF_MODE(GPIO_CNF_ALTERNATE, config->speed);
        } else {
            temp = GPIO_CNF_MODE(GPIO_CNF_OUTPUT, config->speed);
        }

        if (config->outType == GPIO_PIN_OTYPE_OPEN_DRAIN) {
            temp |= GPIO_CNF_OPENDRAIN << GPIO_CNF_OFFSET;
        } else {
            temp |= GPIO_CNF_PUSHPULL << GPIO_CNF_OFFSET;
        }
    }

    if (pin < 8U) {
        HAL_MODIFY_FIELD(port->CRL, temp, 0x0F, pin * 4U);
    } else if (pin < 16U) {
        HAL_MODIFY_FIELD(port->CRH, temp, 0x0F, (pin - 8U) * 4U);
    } else {
        return HAL_FAILED;
    }
    return HAL_SUCCESS;
}

int hal_gpio_pin_get_config(GPIO_TypeDef* port, uint32_t pin, hal_gpio_pin_config_t* config) {
    if ((NULL == port) || (NULL == config)) {
        return HAL_FAILED;
    }
    uint32_t cnf = 0, mode = 0, temp = 0;

    if (pin < 8U) {
        temp = HAL_READ_FIELD(port->CRL, 0x0F, pin * 4U);
        HAL_MODIFY_FIELD(port->CRL, temp, 0x0F, pin * 4U);
    } else if (pin < 16U) {
        temp = HAL_READ_FIELD(port->CRH, 0x0F, (pin - 8U) * 4U);
    } else {
        return HAL_FAILED;
    }

    config->mode = 0;
    config->outType = 0;
    config->speed = 0;
    config->pull = 0;

    cnf = (temp & (GPIO_CNF_MASK << GPIO_CNF_OFFSET)) >> GPIO_CNF_OFFSET;
    mode = temp & GPIO_MODE_MASK;

    if (mode == GPIO_MODE_INPUT) {
        if (cnf == GPIO_CNF_ANALOG) {
            config->mode = GPIO_PIN_MODE_ANALOG;
        } else {
            config->mode = GPIO_PIN_MODE_INPUT;
            if (cnf == GPIO_CNF_FLOATING) {
                config->pull = GPIO_PIN_PULL_NO;
            } else {
                if (GPIO_IS_OUTPUT_PIN_SET(port, pin)) {
                    config->pull = GPIO_PIN_PULL_UP;
                } else {
                    config->pull = GPIO_PIN_PULL_DOWN;
                }
            }
        }
    } else {
        config->speed = mode;
        if (HAL_IS_BIT_SET(cnf, GPIO_CNF_ALTERNATE)) {
            config->mode = GPIO_PIN_MODE_ALTERNATE;
        } else {
            config->mode = GPIO_PIN_MODE_OUTPUT;
        }

        if (HAL_IS_BIT_SET(cnf, GPIO_CNF_OPENDRAIN)) {
            config->outType = GPIO_PIN_OTYPE_OPEN_DRAIN;
        } else {
            config->outType = GPIO_PIN_OTYPE_PUSH_PULL;
        }
    }

    return HAL_SUCCESS;
}

void hal_gpio_pin_set_dir(hal_object_t ctx, int dir) {
    hal_gpio_pin_t* context = (hal_gpio_pin_t*)ctx;
    hal_gpio_pin_config_t cfg;
    if ((NULL != context) && (hal_gpio_pin_get_config(context->port, context->pin, &cfg) == HAL_SUCCESS)) {
        if (dir == 0) {
            cfg.mode = GPIO_PIN_MODE_OUTPUT;
            cfg.speed = GPIO_PIN_SPEED_HIGH;
        } else {
            cfg.mode = GPIO_PIN_MODE_INPUT;
            cfg.outType = 0;
            cfg.speed = 0;
        }
    }

    hal_gpio_pin_set_config(context->port, context->pin, &cfg);
}

int hal_gpio_pin_get_dir(hal_object_t ctx) {
    hal_gpio_pin_t* context = (hal_gpio_pin_t*)ctx;
    hal_gpio_pin_config_t cfg;
    if ((NULL != context) && (hal_gpio_pin_get_config(context->port, context->pin, &cfg) == HAL_SUCCESS)) {
        if (cfg.mode == GPIO_PIN_MODE_OUTPUT) {
            return HAL_GPIO_DIR_OUT;
        } else if (cfg.mode == GPIO_PIN_MODE_INPUT) {
            return HAL_GPIO_DIR_IN;
        }
    }
    return HAL_FAILED;
}

void hal_gpio_pin_set(hal_object_t ctx) {
    hal_gpio_pin_t* context = (hal_gpio_pin_t*)ctx;
    if (GPIO_PIN_ASSERT(context)) {
        GPIO_PIN_SET((GPIO_TypeDef*)context->port, context->pin);
    }
}

void hal_gpio_pin_reset(hal_object_t ctx) {
    hal_gpio_pin_t* context = (hal_gpio_pin_t*)ctx;
    if (GPIO_PIN_ASSERT(context)) {
        GPIO_PIN_RESET((GPIO_TypeDef*)context->port, context->pin);
    }
}

void hal_gpio_pin_toggle(hal_object_t ctx) {
    hal_gpio_pin_t* context = (hal_gpio_pin_t*)ctx;
    if (GPIO_PIN_ASSERT(context)) {
        if (GPIO_IS_OUTPUT_PIN_SET((GPIO_TypeDef*)context->port, context->pin)) {
            GPIO_PIN_RESET((GPIO_TypeDef*)context->port, context->pin);
        } else {
            GPIO_PIN_SET((GPIO_TypeDef*)context->port, context->pin);
        }
    }
}

void hal_gpio_pin_set_value(hal_object_t ctx, int value) {
    hal_gpio_pin_t* context = (hal_gpio_pin_t*)ctx;
    if (GPIO_PIN_ASSERT(context)) {
        if (0 == value) {
            GPIO_PIN_RESET((GPIO_TypeDef*)context->port, context->pin);
        } else {
            GPIO_PIN_SET((GPIO_TypeDef*)context->port, context->pin);
        }
    }
}

int hal_gpio_pin_get_out_value(hal_object_t ctx) {
    hal_gpio_pin_t* context = (hal_gpio_pin_t*)ctx;
    if (GPIO_PIN_ASSERT(context)) {
        return (GPIO_IS_OUTPUT_PIN_SET((GPIO_TypeDef*)context->port, context->pin)) ? 1 : 0;
    }
    return -1;
}

int hal_gpio_pin_get_in_value(hal_object_t ctx) {
    hal_gpio_pin_t* context = (hal_gpio_pin_t*)ctx;
    if (GPIO_PIN_ASSERT(context)) {
        return (GPIO_IS_INPUT_PIN_SET((GPIO_TypeDef*)context->port, context->pin)) ? 1 : 0;
    }
    return -1;
}

#define DEB_MAX 1000
#define DEB_MIN -1000

int hal_gpio_pin_debounce_in(hal_object_t ctx) {
    int debCnt = 0;
    int ret = 0;

    while (1) {
        ret = hal_gpio_pin_get_in_value(ctx);
        if (!ret) {
            debCnt--;
        } else if (1 == ret) {
            debCnt++;
        } else {
            ret = 0xFF;
            break;
        }

        if (debCnt > DEB_MAX) {
            ret = 1;
            break;
        } else if (debCnt < DEB_MIN) {
            ret = 0;
            break;
        }
    }

    return ret;
}

