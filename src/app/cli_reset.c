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


CLI_COMMAND_MAIN(reset)(int argc, char** argv) {
    printf("\r\nSoftware reset\r\n\r\n");
    hal_sys_time_delay_us(100000);
    NVIC_SystemReset();
    return 0;
}

CLI_COMMAND(reset,
    "Software reset",
    NULL
)

void cli_reset_init(void) {
    cli_regcmd(&cli_reset_cmd);
}
