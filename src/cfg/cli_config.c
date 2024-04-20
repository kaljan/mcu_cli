/**
 * @file    cli_config.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-20
 *
 * @brief
 */
#include "cli_config.h"
#include "hal_uart.h"
// #include <stdint.h>


extern void cli_clcd_print_init(void);
extern void cli_echo_init(void);
extern void cli_gpio_info_cmd_init(void);
extern void cli_gpio_dir_cmd_init(void);
extern void cli_gpio_set_cmd_init(void);
extern void cli_gpio_get_cmd_init(void);
extern void cli_info_init(void);
extern void cli_reset_init(void);


static void* redirect_handle;


void cli_register_commands(void) {
    uint32_t baudrate = 115200;
    cli_echo_init();
    cli_info_init();
    cli_reset_init();
    cli_gpio_info_cmd_init();
    cli_gpio_dir_cmd_init();
    cli_gpio_set_cmd_init();
    cli_clcd_print_init();

    redirect_handle = hal_uart_get_handle(2);
    hal_uart_open(redirect_handle, &baudrate);
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
