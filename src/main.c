/**
 * @file    main.c
 * @author  kaljan
 * @version 1.0.0
 * @date    27-11-20 14:35:40
 *
 * @brief
 *
 */


/*
   text    data     bss     dec     hex filename
  26016    1172    5576   32764    7ffc mcu_cli_Debug/mcu_cli.elf
  26000    1172    5576   32748    7fec mcu_cli_Debug/mcu_cli.elf
  25996    1172    5576   32744    7fe8 mcu_cli_Debug/mcu_cli.elf
  25980    1172    5576   32728    7fd8 mcu_cli_Debug/mcu_cli.elf
  25960    1172    5320   32452    7ec4 mcu_cli_Debug/mcu_cli.elf
  25952    1172    5320   32444    7ebc mcu_cli_Debug/mcu_cli.elf
*/



#include "hal.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "clcd.h"
#include "tools.h"
#include "drivers_rcu.h"
#include "stm32f1xx.h"
#include "version.h"
#include "cli.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


extern void cli_clcd_print_init(void);
extern void cli_echo_init(void);
extern void cli_gpio_info_cmd_init(void);
extern void cli_gpio_dir_cmd_init(void);
extern void cli_gpio_set_cmd_init(void);
extern void cli_gpio_get_cmd_init(void);
extern void cli_info_init(void);
extern void cli_reset_init(void);

static void* redirect_handle;

static void redirect_init(void) {
    uint32_t baudrate = 115200;
    redirect_handle = hal_uart_get_handle(2);
    hal_uart_open(redirect_handle, &baudrate);
}

static void RCU_EventCallback(void* context, uint32_t event) {
    clcd_clear();
    if (event > 0xFFFF) {
        clcd_print(0x00, "RCU: 0x%06X", event);
    } else {
        clcd_print(0x00, "RCU: 0x%04X", event);
    }
}

static void cli_init_fn(void) {
    cli_echo_init();
    cli_info_init();
    cli_reset_init();
    cli_gpio_info_cmd_init();
    cli_gpio_dir_cmd_init();
    cli_gpio_set_cmd_init();
    cli_clcd_print_init();
}

int main(void) {
    char esc_seq = 0;
    char esc_buf[8];
    uint32_t esc_cursor = 0;
    cli_config_t cli_cfg = {
        .hello_msg = "\r\nCommand line interface for MCUs\r\n",
        .prompt = "\e[1;34mbluepill\e[37m#\e[0m", //"\e[1;34mbluepill\e[37m:~\e[0m$",
        .init_cmds_fn = cli_init_fn
    };

    hal_init();
    redirect_init();

    cli_start(&cli_cfg);
    clcd_init();
    clcd_print(0x00, "CLI develop\nv%d.%d.%d"
        , MCLI_VERSION_MAJOR, MCLI_VERSION_MINOR, MCLI_VERSION_BUILD);

    RCU_Initialize();
    RCU_SetCallback(RCU_EventCallback, NULL);

    while (1) {
        RCU_Update();
        cli_update();
    }
}

int __io_putchar(int ch) {
    int ret = -1;
    if (ch >= 0) {
        hal_uart_put(redirect_handle, (uint8_t)ch);
    }
    return ret;
}

int __io_getchar(void) {
	int ret = -1;
    uint8_t c = 0;
    ret = hal_uart_get(redirect_handle, &c);
	if (ret == 0) {
        ret = c;
    }
	return ret;
}

