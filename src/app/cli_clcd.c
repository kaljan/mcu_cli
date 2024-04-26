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


CLI_COMMAND_MAIN(clcd_print)(int argc, char** argv) {
    if (argc < 2) {
        printf("USAGE: clcd_print <cursor position> <text>\r\n");
        return -1;
    }
    char clcd_buf[33];
    int cursor = 0;
    argv++;

    while (*argv != NULL && cursor < 32) {
        int ret = snprintf(&clcd_buf[cursor], 33 - cursor, "%s ", *argv);
        argv++;
        if (ret > 0) {
            cursor += ret;
        }
    }

    clcd_over_print(0, 32, clcd_buf);

    return 0;
}

CLI_COMMAND(clcd_print,
    "Print text to LCD",
    NULL
)

#if (HAL_CLCD_PWREN == HAL_ENABLED)
CLI_COMMAND_MAIN(clcd_power)(int argc, char** argv) {
    static int power_enbled = 1;
    if (argc < 2) {
        printf("USAGE: %s <on/off>", *argv);
        return -1;
    }

    argv++;
    if (strcmp("on", *argv) == 0) {
        if (power_enbled == 0) {
            power_enbled = 1;
            clcd_init();
        } else {
            printf("LCD already powered on\r\n");
        }
    } else if (strcmp("off", *argv) == 0) {
        if (power_enbled == 1) {
            power_enbled = 0;
            clcd_power_disable();
        } else {
            printf("LCD already powered off\r\n");
        }
    } else {
        printf("bad argumen\r\n");
        return -1;
    }

    return 0;
}

CLI_COMMAND(clcd_power,
    "Print text to LCD",
    NULL
)
#endif

void cli_clcd_init(void) {
    CLI_REGISTER_COMMAND(clcd_print);
#if (HAL_CLCD_PWREN == HAL_ENABLED)
    CLI_REGISTER_COMMAND(clcd_power);
#endif
}
