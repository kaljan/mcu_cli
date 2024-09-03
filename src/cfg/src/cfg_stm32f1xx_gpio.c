/**
 * @file    cfg_stm32f1xx_gpio.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-07
 *
 * @brief   GPIO configs
 */
#include "stm32f1xx_gpio.h"

#include "cfg_stm32f1xx_mc6800.h"
#include "hal_mc6800.h"

// static hal_gpio_pin_t gpio_array[] = {
//     {GPIOA, 0},
//     {GPIOA, 1},
//     {GPIOA, 2},
//     {GPIOA, 3},
//     {GPIOA, 4},
//     {GPIOA, 5},
//     {GPIOA, 6},
//     {GPIOA, 7},
//     {GPIOA, 8},
//     {GPIOA, 9},
//     {GPIOA, 10},
//     {GPIOA, 11},
//     {GPIOA, 12},
//     {GPIOA, 13},
//     {GPIOA, 14},
//     {GPIOA, 15},
// };

#if (HAL_CLCD_PWREN == HAL_ENABLED)
static hal_gpio_pin_t CLCD_PWR_EN_Pin = {.port = CLCD_PWREN_PORT, .pin = CLCD_PWREN_PIN,};
#endif

static hal_gpio_pin_t MC6800_E_Pin = {.port = MC6800_E_PORT, .pin = MC6800_E_PIN,};
static hal_gpio_pin_t MC6800_RS_Pin = {.port = MC6800_RS_PORT, .pin = MC6800_RS_PIN,};

#if (HAL_MC6800_READ == HAL_ENABLED)
static hal_gpio_pin_t MC6800_RW_Pin = {.port = MC6800_RW_PORT, .pin = MC6800_RW_PIN,};
#endif


#if (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA8)
static hal_gpio_pin_t MC6800_Data0 = {.port = MC6800_D0_PORT, .pin = MC6800_D0_PIN,};
static hal_gpio_pin_t MC6800_Data1 = {.port = MC6800_D1_PORT, .pin = MC6800_D1_PIN,};
static hal_gpio_pin_t MC6800_Data2 = {.port = MC6800_D2_PORT, .pin = MC6800_D2_PIN,};
static hal_gpio_pin_t MC6800_Data3 = {.port = MC6800_D3_PORT, .pin = MC6800_D3_PIN,};
#endif
static hal_gpio_pin_t MC6800_Data4 = {.port = MC6800_D4_PORT, .pin = MC6800_D4_PIN,};
static hal_gpio_pin_t MC6800_Data5 = {.port = MC6800_D5_PORT, .pin = MC6800_D5_PIN,};
static hal_gpio_pin_t MC6800_Data6 = {.port = MC6800_D6_PORT, .pin = MC6800_D6_PIN,};
static hal_gpio_pin_t MC6800_Data7 = {.port = MC6800_D7_PORT, .pin = MC6800_D7_PIN,};


static hal_mc6800_t hal_mc6800_instance = {
    .e = &MC6800_E_Pin,
    .rs = &MC6800_RS_Pin,
#if (HAL_MC6800_READ == HAL_ENABLED)
    .rw = &MC6800_RW_Pin,
#endif
    .d = {
#if (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA8)
        &MC6800_Data0,
        &MC6800_Data1,
        &MC6800_Data2,
        &MC6800_Data3,
#endif
        &MC6800_Data4,
        &MC6800_Data5,
        &MC6800_Data6,
        &MC6800_Data7,
    }
};

hal_mc6800_t* hal_mc6800_get_port(void) {
    return &hal_mc6800_instance;
}

#if (HAL_CLCD_PWREN == HAL_ENABLED)

hal_object_t hal_clcd_pwren_get_pin(void) {
    return &CLCD_PWR_EN_Pin;
}
#endif

