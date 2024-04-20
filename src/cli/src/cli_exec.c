/**
 * @file    cli_exec.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-13
 *
 * @brief
 */

#include "cli.h"
#include "cli_exec.h"
#include "cli_history.h"


static int cli_cmd_argc = 0;
static char* cli_cmd_argv[CLI_ARGV_SIZE];
static char cli_cmd_buf[CLI_BUF_SIZE];

static sm_bucket_t cli_bucket_array[CLI_CMD_COUNT];
static static_map_t cli_map = {
    .list = cli_bucket_array,
    .size = CLI_CMD_COUNT,
    .count = 0,
};

static cli_node_t* cli_cmd_array[CLI_CMD_COUNT + 1];
static int cli_cmd_array_idx = 0;

static const char* cli_exec_cplt_list[CLI_CMD_COUNT + 1];


static int cli_exec_cmpstr(const char* str1, const char* str2, int len) {
    int ret = -1;
    if ((str1 != NULL) && (str2 != NULL) && (len > 0)) {
        ret = 0;
        while ((*str1 != '\0') && (len > 0)) {
            if (*str2 == '\0' || (*str1 != *str2)) {
                ret = 1;
                break;
            }
            str1++;
            str2++;
            len--;
        }
    }
    return ret;
}

const char** cli_exec_acplt(const char* expr, int len, int*const num) {
    if ((expr != NULL)) {
        int i = 0;
        cli_node_t** ptr = cli_cmd_array;
        while (*ptr != NULL) {
            if (cli_exec_cmpstr(expr, (*ptr)->c_name, len) == 0) {
                cli_exec_cplt_list[i] = (*ptr)->c_name;
                i++;
                if (i >= CLI_CMD_COUNT) {
                    break;
                }
            }
            ptr++;
        }

        if (i > 0) {
            if (num != NULL) {
                *num = i;
            }
            cli_exec_cplt_list[i] = NULL;
            return cli_exec_cplt_list;
        }
    }
    return NULL;
}

void cli_exec_cmd(const char* cmd, int size) {
    if ((size > 0) && (cmd != NULL)) {
        int flags = 0;
        int ret = 0;
        char* dst_ptr = cli_cmd_buf;
        cli_cmd_argc = 0;
        flags = 0x02;

        printf("exec: \'%s\'\r\n", cmd);

        if (*cmd == ' ') {
            while (*cmd == ' ') {
                cmd++;
            }
        } else {
            cli_history_add(cmd);
        }

        while (*cmd != '\0') {

            if (*cmd == ' ') {
                if ((flags & 0x01) == 0x01) {
                    flags &= ~0x01;
                    *dst_ptr = ' ';
                } else {
                    flags |= 0x02;
                    *dst_ptr = '\0';
                }
            } else {
                *dst_ptr = *cmd;

                if ((flags & 0x02) == 0x02) {
                    flags &= ~0x02;
                    cli_cmd_argv[cli_cmd_argc] = dst_ptr;
                    cli_cmd_argc++;
                    if (cli_cmd_argc < CLI_ARGV_SIZE - 1) {
                        cli_cmd_argv[cli_cmd_argc + 1] = NULL;
                    } else {
                        break;
                    }
                }

                if (*cmd == '\\') {
                    flags |= 0x01;
                }
            }
            dst_ptr++;
            cmd++;
        }

        *dst_ptr = 0;

        if (cli_cmd_argc > 0) {
            cli_node_t* node = static_map_get(&cli_map, cli_cmd_argv[0]);
            if ((NULL != node) && (NULL != node->main_fn)) {
                ret = node->main_fn(cli_cmd_argc, cli_cmd_argv);
                if (ret != 0) {
                    printf("command failed with code: %d\r\n", ret);
                }
            } else {
                printf("%s: command not found\r\n", cli_cmd_argv[0]);
            }
        }
    }
}

void cli_regcmd(cli_node_t* item) {
    if (NULL != item) {
        item->node.key = item->c_name;
        item->node.value = item;
        item->node.prev = NULL;
        item->node.next = NULL;
        static_map_put(&cli_map, &item->node);
        if (cli_cmd_array_idx < CLI_CMD_COUNT) {
            cli_cmd_array[cli_cmd_array_idx++] = item;
            cli_cmd_array[cli_cmd_array_idx] = NULL;
        }
    }
}

void cli_exec_init(void) {
    memset(cli_bucket_array, 0, sizeof(sm_bucket_t) * CLI_CMD_COUNT);
}


static void cli_help_cmd_print_cmd_name(const char* text, int max) {
    while (max > 0) {
        if (*text != '\0') {
            putchar_fn(*text);
            text++;
        } else {
            putchar_fn(' ');
        }
        max--;
    }
    putchar_fn(' ');
}

static int cli_help_cmd_main(int argc, char** argv) {
    if (argc < 2) {
        printf("\r\n");

        cli_node_t** ptr = cli_cmd_array;
        while (*ptr != NULL) {
            cli_help_cmd_print_cmd_name((*ptr)->c_name, 16);
            printf("%s\r\n", (*ptr)->brief);
            ptr++;
        }
        printf("\r\n");
    } else {
        cli_node_t* item = static_map_get(&cli_map, *(argv+1));
        if ((NULL != item) && (NULL != item->help_fn)) {
            item->help_fn();
        }
    }
    return 0;
}

static void cli_help_cmd_help(void) {
    printf(
        "\r\nPrint help info.\r\n"
        "USAGE: help <command_name>\r\n"
        "Without arguments this command prints all available commands with short description.\r\n"
        "If you enter as argument command_name this will print help for the command\r\n\r\n"
        );
}

static cli_node_t cli_help_cmd = {
    .c_name = "help",
    .brief = "print help info",
    .main_fn = cli_help_cmd_main,
    .help_fn = cli_help_cmd_help,
};

void cli_help_cmd_init(void) {
    cli_regcmd(&cli_help_cmd);
}
