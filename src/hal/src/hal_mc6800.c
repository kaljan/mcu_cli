/**
 * @file    hal_mc6800.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-07
 *
 * @brief
 */
#include "hal_mc6800.h"
#include "hal_gpio.h"
#include "hal_tim.h"

extern hal_mc6800_t* hal_mc6800_get_port(void);


static hal_mc6800_t* hal_mc6800_ptr = NULL;


static void hal_mc6800_latch(void) {
    hal_gpio_pin_set(hal_mc6800_ptr->e);
    hal_sys_time_delay_us(HAL_MC6800_LATCH_DELAY_US);
    hal_gpio_pin_reset(hal_mc6800_ptr->e);
}

static void hal_mc6800_rs(int rs) {
    hal_gpio_pin_set_value(hal_mc6800_ptr->rs, rs);
}

#if (HAL_MC6800_READ == HAL_ENABLED)
static void hal_mc6800_rw(int rw) {
    hal_gpio_pin_set_value(hal_mc6800_ptr->rw, rw);
}

static void hal_mc6800_set_data_dir(int dir) {
    for (uint8_t i = 0; i < HAL_MC6800_DATA_WIDTH; i++) {
        hal_gpio_pin_set_dir(hal_mc6800_ptr->d[i], dir);
    }
    hal_mc6800_rw(dir);
}

static uint8_t hal_mc6800_get_data(void) {
    uint8_t data = 0;
    for (uint8_t i = 0; i < HAL_MC6800_DATA_WIDTH; i++) {
        if (hal_gpio_pin_get_in_value(hal_mc6800_ptr->d[i]) == 0) {
            data &= ~(0x01 << i);
        } else {
            data |= 0x01 << i;
        }
    }
    return data;
}
#endif /* (HAL_MC6800_READ == HAL_ENABLED) */

static void hal_mc6800_set_data(uint8_t data) {
    for (uint8_t i = 0; i < HAL_MC6800_DATA_WIDTH; i++) {
        hal_gpio_pin_set_value(hal_mc6800_ptr->d[i],
            ((data & (0x01 << i)) == (0x01 << i)) ? 1 : 0);
    }
}


void hal_mc6800_init(void) {
    hal_mc6800_ptr = hal_mc6800_get_port();
    if (NULL != hal_mc6800_ptr) {

        hal_gpio_pin_reset(hal_mc6800_ptr->e);
        hal_gpio_pin_set_dir(hal_mc6800_ptr->e, HAL_GPIO_DIR_OUT);
        hal_gpio_pin_reset(hal_mc6800_ptr->rs);
        hal_gpio_pin_set_dir(hal_mc6800_ptr->rs, HAL_GPIO_DIR_OUT);

#if (HAL_MC6800_READ == HAL_ENABLED)
        hal_gpio_pin_reset(hal_mc6800_ptr->rw);
        hal_gpio_pin_set_dir(hal_mc6800_ptr->rw, HAL_GPIO_DIR_OUT);
#endif /* (HAL_MC6800_READ == HAL_ENABLED) */

        for (uint8_t i = 0; i < HAL_MC6800_DATA_WIDTH; i++) {
            hal_gpio_pin_reset(hal_mc6800_ptr->d[i]);
            hal_gpio_pin_set_dir(hal_mc6800_ptr->d[i], HAL_GPIO_DIR_OUT);
        }
    }
}

void hal_mc6800_reset(void) {
    hal_gpio_pin_reset(hal_mc6800_ptr->e);
    hal_gpio_pin_reset(hal_mc6800_ptr->rs);
#if (HAL_MC6800_READ == HAL_ENABLED)
        hal_gpio_pin_reset(hal_mc6800_ptr->rw);
#endif

    for (uint8_t i = 0; i < HAL_MC6800_DATA_WIDTH; i++) {
        hal_gpio_pin_reset(hal_mc6800_ptr->d[i]);
    }
}

#if (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4)
void hal_mc6800_4bit_write(uint8_t di, uint8_t d) {
    if (NULL != hal_mc6800_ptr) {
        hal_mc6800_rs(di);
        hal_mc6800_set_data(d);
        hal_mc6800_latch();
    }
}
#endif /* (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4) */

void hal_mc6800_write(uint8_t di, uint8_t data){
    if (NULL != hal_mc6800_ptr) {
        hal_mc6800_rs(di);
#if (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4)
        hal_mc6800_set_data(data >> 4);
        hal_mc6800_latch();
#endif /* (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4) */
        hal_mc6800_set_data(data);
        hal_mc6800_latch();
    }
}

#if (HAL_MC6800_READ == HAL_ENABLED)
uint8_t hal_mc6800_read(int di) {
    uint8_t ret = 0;
    if (NULL != hal_mc6800_ptr) {
        hal_mc6800_set_data_dir(HAL_DIR_READ);
        hal_mc6800_rs(di);

        hal_gpio_pin_set(hal_mc6800_ptr->e);
        ret = hal_mc6800_get_data();
        hal_gpio_pin_reset(hal_mc6800_ptr->e);

#if (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4)
        ret <<= 4;
        hal_sys_time_delay_us(HAL_MC6800_LATCH_DELAY_US);
        hal_gpio_pin_set(hal_mc6800_ptr->e);
        ret |= (hal_mc6800_get_data() & 0x0F);
        hal_gpio_pin_reset(hal_mc6800_ptr->e);
#endif /* (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4) */
        hal_mc6800_set_data_dir(HAL_DIR_WRITE);
    }
    return ret;
}
#endif /* (HAL_MC6800_READ == HAL_ENABLED) */
