/**
 * @file    cli_echo.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-18
 *
 * @brief
 */

#include "cli.h"
#include "clcd.h"

CLI_COMMAND_MAIN(echo)(int argc, char** argv) {
    argv++;
    while (*argv != NULL) {
        printf("%s", *argv);
        argv++;

        if (*argv != NULL) {
            printf(" ");
        }

        fflush(stdout);
    }
    printf("\r\n");

    return 0;
}

CLI_COMMAND(echo,
    "Print text to terminal",
    NULL
)

void cli_echo_init(void) {
    CLI_REGISTER_COMMAND(echo)
}


