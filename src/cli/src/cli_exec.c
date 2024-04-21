/**
 * @file    cli_exec.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-13
 *
 * @brief
 */

#include "cli_exec.h"
#include "cli_history.h"


static int cli_cmd_argc = 0;
static char* cli_cmd_argv[CLI_ARGV_SIZE];
static char cli_cmd_buf[CLI_BUF_SIZE];

static cli_node_t* cli_exec_head = NULL;
static cli_node_t* cli_exec_tail = NULL;

static void cli_exec_append(cli_node_t** head, cli_node_t** tail, cli_node_t* item) {
    if ((head != NULL) || (tail != NULL) || (item != NULL)) {
        if (*head == NULL) {
            *head = item;
            *tail = item;
        } else {
            if (*tail == NULL) {
                *tail = *head;
            }

            (*tail)->next = item;
            *tail = item;
        }
        item->next = NULL;
    }
}


static cli_node_t* cli_exec_get_cmd(cli_node_t* head, const char* cmd) {
    while (head != NULL) {
        if (strcmp(cmd, head->c_name) == 0) {
            break;
        }
        head = head->next;
    }
    return head;
}

#if (CLI_CMDCPLT == 1)

cli_node_t* cli_exec_cplt(const char* expr, int len, int*const num) {
    if ((expr != NULL)) {
        int i = 0;
        cli_node_t* item = cli_exec_head;
        cli_node_t* head = NULL;
        cli_node_t* tail = NULL;
        while (item != NULL) {
            if (strncmp(expr, item->c_name, len) == 0) {
                if (head == NULL) {
                    head = item;
                    tail = item;
                } else {
                    item->cplt_next = NULL;
                    tail->cplt_next = item;
                    tail = item;
                }
                i++;
            }
            item = item->next;
        }

        if ((i > 0) && (head != NULL) && (tail != NULL)) {
            if (num != NULL) {
                *num = i;
            }
            tail->cplt_next = head;
            return head;
        }
    }
    return NULL;
}
#endif /* CLI_CMDCPLT == 1 */

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
        }
#if (CLI_HISTORY == 1)
        else {
            cli_history_add(cmd);
        }
#endif /* CLI_HISTORY == 1 */

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
            cli_node_t* node = cli_exec_get_cmd(cli_exec_head, cli_cmd_argv[0]);
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
        cli_exec_append(&cli_exec_head, &cli_exec_tail, item);
    }
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

        cli_node_t* ptr = cli_exec_head;
        while (ptr != NULL) {
            cli_help_cmd_print_cmd_name(ptr->c_name, 16);
            printf("%s\r\n", ptr->brief);
            ptr = ptr->next;
        }
        printf("\r\n");
    } else {
        cli_node_t* item = cli_exec_get_cmd(cli_exec_head, *(argv+1));
        if ((NULL != item) && (NULL != item->help_fn)) {
            item->help_fn();
        }
    }
    return 0;
}

static void cli_help_cmd_help(void) {
    printf(
        "\r\nPrint help info.\r\n"
        "USAGE: help [<command_name>]\r\n"
        );
}

static cli_node_t cli_help_cmd = {
    .c_name = "help",
    .brief = "Print help info",
    .main_fn = cli_help_cmd_main,
    .help_fn = cli_help_cmd_help,
};

void cli_help_cmd_init(void) {
    cli_regcmd(&cli_help_cmd);
}
