/**
 * @file    cli_reset.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-15
 *
 * @brief
 */
#include "cli.h"
#include "stm32f1xx.h"
#include "hal_tim.h"
#include <stdio.h>

static int cli_reset_main(int argc, char** argv) {
    printf("\r\nSoftware reset\r\n\r\n");
    hal_sys_time_delay_us(100000);
    NVIC_SystemReset();
    return 0;
}

static void cli_reset_cmd_help(void) {
    printf(
        "\r\nSoftware reset\r\n\r\n"
        );
}


static cli_node_t cli_reset_cmd = {
    .c_name = "reset",
    .brief = "Software reset",
    .main_fn = cli_reset_main,
    .help_fn = cli_reset_cmd_help,
};

void cli_reset_init(void) {
    cli_append_command(&cli_reset_cmd);
}
