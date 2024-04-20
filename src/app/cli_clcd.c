/**
 * @file    cli_clcd.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-02
 *
 * @brief
 */

#include "cli.h"
#include "clcd.h"


static int cli_clcd_print_main(int argc, char** argv) {
    if (argc < 3) {
        printf("USAGE: clcd_print <cursor position> <text>\r\n");
        return -1;
    }
    argv++;
    int cursor_position = atoi(*argv);
    char clcd_text[33];
    int index = 0;

    if (cursor_position < 0 || cursor_position >= 32) {
        printf("Wrong cursor position: %d", cursor_position);
        return -1;
    }

    argv++;

    while (*argv != NULL) {
        int len = strlen(*argv);
        if (len > 32) {
        }
    }

    return 0;
}

static void cli_clcd_print_help(void) {
    printf("\r\nPrint text to LCD\r\n\r\n");
}

static cli_node_t cli_clcd_print_cmd = {
    .c_name = "clcd_print",
    .brief = "Print text to LCD",
    .main_fn = cli_clcd_print_main,
    .help_fn = cli_clcd_print_help,
};

void cli_clcd_print_init(void) {
    cli_regcmd(&cli_clcd_print_cmd);
}
