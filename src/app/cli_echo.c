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

#if 1
static void print_args(int argc, char** argv) {
    printf("+--------------------------------\r\n");
    printf("| argc: %d\r\n", argc);
    int index = 0;
    while (*argv != NULL) {
        printf("| %3d: \'%s\'\r\n", index++, *argv);
        argv++;
    }
    printf("+--------------------------------\r\n");
}
#else
#define print_args(argc, argv)
#endif

CLI_COMMAND_MAIN(echo)(int argc, char** argv) {

    print_args(argc, argv);
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


