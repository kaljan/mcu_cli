/**
 * @file    hal_mc6800.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-07
 *
 * @brief   MC6800 port driver
 */
#ifndef HAL_MC6800_H
#define HAL_MC6800_H

#include "hal_config.h"

#if (HAL_MC6800_ENABLE == HAL_ENABLED)

typedef struct hal_mc6800 {
    hal_object_t e;
    hal_object_t rs;
    hal_object_t rw;
    hal_object_t d[HAL_MC6800_DATA_WIDTH];
} hal_mc6800_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void hal_mc6800_init(void);
void hal_mc6800_reset(void);

#if (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4)
void hal_mc6800_4bit_write(uint8_t di, uint8_t d);
#endif /* (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4) */

void hal_mc6800_write(uint8_t di, uint8_t data);

#if (HAL_MC6800_READ == HAL_ENABLED)
uint8_t hal_mc6800_read(int di);
#endif /* (HAL_MC6800_READ == HAL_ENABLED) */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

#endif /* HAL_MC6800_H */
