/**
 * @file    clcd_config.h
 * @author  Mikalai Naurotski
 * @version 1.0.4
 * @date    01-12-23
 *
 * @brief   Character display configurations
 */
#ifndef CLCD_CONFIG_H
#define CLCD_CONFIG_H

#include "hal_tim.h"

/** Characters per line */
#define CLCD_WIDTH                                  16

/** Number of display lines  */
#define CLCD_LINES                                  2

/** Display font  */
#define CLCD_FONT_5x8                               0
#define CLCD_FONT_5x10                              1
#define CLCD_FONT                                   CLCD_FONT_5x8

/** HD44780 display type */
#define CLCD_IC_HD44780                             0
#define CLCD_IC_KS0066                              1
#define CLCD_IC_RS0010                              2
#define CLCD_IC_TYPE                                CLCD_IC_KS0066

/** HD44780 Character table */
#if (CLCD_IC_TYPE == CLCD_IC_HD44780)
#define CLCD_CHAR_TABLE_A00                         0
#define CLCD_CHAR_TABLE_A02                         1
#define CLCD_FONT_TABLE                             CLCD_CHAR_TABLE_A02
#elif (CLCD_IC_TYPE == CLCD_IC_KS0066)
#define CLCD_IC_KS0066_CYRILLIC                     0
#define CLCD_FONT_TABLE                             CLCD_IC_KS0066_CYRILLIC
#elif (CLCD_IC_TYPE == CLCD_IC_RS0010)
/** RS0010 Raystar support */
#define RS0010_ENGLISH_JAPANESE_FONT_TABLE          0U
#define RS0010_WESTERN_EUROPEAN_FONT_TABLE_I        1U
#define RS0010_ENGLISH_RUSSIAN_FONT_TABLE           2U
#define RS0010_WESTERN_EUROPEAN_FONT_TABLE_II       3U
#define RS0010_FONT_TABLE_MASK                      3U
#define CLCD_FONT_TABLE                             RS0010_ENGLISH_RUSSIAN_FONT_TABLE
#endif

/** CLCD Interface type */
#define CLDC_IF_TYPE_6800                           0
#define CLDC_IF_TYPE                                CLDC_IF_TYPE_6800

/** Delay after command execution */
#define CLCD_CMD_DELAY_US                           100

/** Delay after */
#define CLCD_CMD_INIT_DELAY                         5000

/** Delay after clear instrucion */
#define CLCD_CLEAR_DELAY_US                         2000

/** Power on delay */
#define CLCD_POWER_ON_DELAY_US                      200000

#define clcd_delay_us(delay)                        hal_sys_time_delay_us(delay)


#if (CLDC_IF_TYPE == CLDC_IF_TYPE_6800)

#include "hal_mc6800.h"
#include "hal_gpio.h"

#define CLCD_INSTRUCTION                            HAL_INSTRUCTION
#define CLCD_DATA                                   HAL_DATA

#define clcd_port_init()                            hal_mc6800_init()

#define clcd_port_reset()                           hal_mc6800_reset()

#if (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4)
#define clcd_port_half_byte_write(DI, DATA)         hal_mc6800_4bit_write(DI, DATA)
#endif /* (HAL_MC6800_DATA_WIDTH == HAL_MC6800_DATA4) */

#define clcd_port_write(di, data)                   hal_mc6800_write(di, data)

#if (HAL_MC6800_READ == HAL_ENABLED)
#define clcd_port_read(di)                          hal_mc6800_read(di)
#endif /* (HAL_MC6800_READ == HAL_ENABLED) */

#endif /* (CLDC_IF_TYPE == CLDC_IF_TYPE_6800) */

#endif /* CLCD_CONFIG_H */
/* EOF */
