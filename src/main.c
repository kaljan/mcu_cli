/**
 * @file    main.c
 * @author  kaljan
 * @version 1.0.0
 * @date    27-11-20 14:35:40
 *
 * @brief
 *
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
#include "cli_info.h"
#include "cli_gpio.h"
#include "cli_reset.h"
#include "cli_clcd.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


static void* redirect_handle;

void redirect_init(void) {
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


        // if ((' ' <= c) && ('~' >= c) && (CLI_BUF_SIZE > cli_buf_bytes)) {
        //     cli_buf[cli_buf_bytes] = (char)c;
        //     cli_buf_bytes++;
        //     putchar_fn(c);
        // } else if (c == '\r') {
        //     if (cli_buf_bytes > 0) {
        //         cli_buf[cli_buf_bytes] = 0;
        //         printf("\r\nCMD: %s\r\n", cli_buf);
        //     } else {
        //         putchar_fn('\r');
        //         putchar_fn('\n');
        //     }
        //     cli_buf_bytes = 0;
        //     printf("%s ", cli_prompt);
        //     fflush(stdout);
        // }

/* VT102
 * ------------------------------------------------------------------
 * 1. Backspace
 *
 * 1.Если есть что стирать
 *
 * TX : 0x08
 * RX : 0x08, 0x1B, 0x5B '[', 0x4A 'J',
 *
 * Если упёрлись в приглашение командной строки, то обратно просто ничего не отправляется
 *
 * ------------------------------------------------------------------
 * DEL
 *
 * Если символ стоит в конце строки, то обратно ничего не отправляем
 *
 *
 *
 */
// static int cli_check_char(int c) {
//     if (c < 0) {
//         return 0;
//     } else if ((' ' <= c) && ('~' >= c) && (CLI_BUF_SIZE > cli_buf_bytes)) {
//         cli_buf[cli_buf_bytes] = (char)c;
//         cli_buf_bytes++;
//         return c;
//     } else if (c == '\r') {
//         if (cli_buf_bytes > 0) {
//             cli_buf[cli_buf_bytes] = 0;
//             printf("\r\nCMD: %s\r\n", cli_buf);
//         } else {
//             __io_putchar('\r');
//             __io_putchar('\n');
//         }
//         cli_buf_bytes = 0;
//         printf("%s ", cli_prompt);
//         fflush(stdout);
//     }
//     return 0;
// }

    // int c = cli_check_char(__io_getchar());
    // if (c == 0) {
    //     return;
    // }
    // __io_putchar(c);

    // printf("Char test\r\n");
        // int c = __io_getchar();
        // if (c >= 0) {

        //     if (c == 0x1B) {
        //         printf("\e[1;41m 0x%02X \e[0m ", (char)c);
        //         fflush(stdout);
        //     } else if (c == '\n' || c == '\r') {
        //         printf("\r\n\e[45m 0x%02X \e[0m\r\n", c);
        //         // fflush(stdout);
        //     } else if (isprint(c) != 0) {
        //         printf("\e[1;44m 0x%02X \'%c\' \e[0m ", (char)c, (char)c);
        //         fflush(stdout);
        //     } else {
        //         printf("\e[1;45m 0x%02X \e[0m ", (char)c);
        //         fflush(stdout);
        //     }

        // }
            // if (esc_seq == 0) {
                // if (c == 0x1B) {
                //     esc_seq = 1;
                //     esc_cursor = 0;
                // } else
                // if (isprint(c) == 0) {
                //     printf("\e[1;44m %02X \e[0m ", (char)c);
                // } else if (c == '\n') {
                //     __io_putchar('\r');
                //     __io_putchar(c);
                // } else {
                //     __io_putchar(c);
                // }
            // } else {

            // }
// #if 1
//             if (c == '\r' || c == '\n') {
//                 printf("\r\n`0x%02X`\r\n", c);
//             } else {
//                 if (isprint(c) != 0) {
//                     printf("`0x%02X` ", (char)c);
//                 } else {
//                     printf("`0x%02X` ", (char)c);
//                 }
//                 fflush(stdout);
//             }
// #else
//         __io_putchar(c);
// #endif

            // if (c == 0x1B) {
                // printf("\e[1;41m %02X \e[0m ", (char)c);
            //     // __io_putchar(' ');
            //     // putchar(' ');
                // fflush(stdout);
            // } else {
            //     printf("\e[1;44m %02X \e[0m ", (char)c);
            //     fflush(stdout);
            //     // __io_putchar(' ');
            //     // putchar(' ');
            // }
