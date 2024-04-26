/**
 * @file    cli_config.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-20
 *
 * @brief   Configurations and initializations for command line interface
 */
#include "cli_config.h"
#include "hal_uart.h"


extern void cli_clcd_init(void);
extern void cli_echo_init(void);
extern void cli_gpio_init(void);
extern void cli_info_init(void);
extern void cli_reset_init(void);


static void* redirect_handle;


void cli_register_commands(void) {
    uint32_t baudrate = 115200;
    redirect_handle = hal_uart_get_handle(2);
    hal_uart_open(redirect_handle, &baudrate);

    cli_echo_init();
    cli_info_init();
    cli_reset_init();
    cli_gpio_init();
    cli_clcd_init();
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
