/**
 * @file    clcd.h
 * @author  Mikalai Naurotski
 * @version 1.0.4
 * @date    29-01-20
 *
 * @brief   Character display driver
 */
#ifndef CLCD_H
#define CLCD_H

#include <stdint.h>
#include "clcd_config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (HAL_CLCD_PWREN == HAL_ENABLED)
void clcd_power_enable(void);
void clcd_power_disable(void);
#endif

void clcd_init(void);
void clcd_wait_busy(void);
void clcd_clear(void);

void clcd_set_cursor_visible(uint8_t isVisible);
void clcd_enable_cursor_blink(uint8_t isBlink);
void clcd_set_cursor_position(uint8_t position);
void clcd_set_char(uint8_t c);

void clcd_write_char(uint8_t addr, char c);
void clcd_write_text(uint8_t addr, const char* text, uint32_t size);
void clcd_print(uint8_t start, const char* fmt, ...);
void clcd_over_print(uint8_t start, uint32_t maxLen, const char* fmt, ...);
void clcd_write_custom_char(uint8_t index, const uint8_t* data, uint32_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLCD_H */
/* EOF */
