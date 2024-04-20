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


static int cli_echo_main(int argc, char** argv) {
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

static void cli_echo_help(void) {
    printf("\r\nPrint text to terminal\r\n\r\n");
}

static cli_node_t cli_echo_cmd = {
    .c_name = "echo",
    .brief = "Print text to terminal",
    .main_fn = cli_echo_main,
    .help_fn = cli_echo_help,
};

void cli_echo_init(void) {
    cli_regcmd(&cli_echo_cmd);
}


