/**
 * @file    cli_exec.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-13
 *
 * @brief   Command line interface run and command completion
 */

#include "cli_exec.h"
#include "cli_history.h"


static int cli_cmd_argc = 0;              /* count of arguments */
static char* cli_cmd_argv[CLI_ARGV_SIZE + 1]; /* array of argument pointers */
static char cli_cmd_buf[CLI_BUF_SIZE];    /* buffer for command name and its arguments */

static cli_node_t* cli_exec_head = NULL;  /* pointer to head of command list */
static cli_node_t* cli_exec_tail = NULL;  /* pointer to tail of command list */

/**
 * Add command to list
 *
 * @param head  pointer to head of list
 * @param tail  pointer to tail of list
 * @param item  pointer to command instance
 */
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

/**
 * Get pointer to command
 *
 * @param head Poniter to head of list
 * @param cmd command name
 *
 * @return pointer to command instance if its found or NULL if not
 */
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

/**
 * Command complete command
 *
 * @param expr expression
 * @param len length of expression
 * @param num in this pointer count of found command will be saved
 *
 * @return pointer to first command instance if something was found, also
 * count of objects will be stored. Returns NULL if nothing found.
 */
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

/**
 * Execute command
 *
 * @param cmd command name
 * @param size size of command
 */
void cli_exec_cmd(const char* cmd, int size) {
    if ((size > 0) && (cmd != NULL)) {
        int flags = 0;
        int ret = 0;
        char* arg_ptr = NULL;
        char* dst_ptr = cli_cmd_buf;

        cli_cmd_argc = 0;
        flags = 0x02;

        // printf("exec: \'%s\'\r\n", cmd);

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
            if (*cmd == '\\') {
                // cmd++;
                if ((flags & 0x04) == 0x04) {
                    if (*(cmd + 1) == '\"') {
                        cmd++;
                    }
                } else {
                    if (((flags & 0x01) == 0x01)) {
                        flags &= ~0x01;
                    } else {
                        flags |= 0x01;
                        cmd++;
                        continue;
                    }
                }
            } else if (*cmd == '\"') {
                if ((flags & 0x01) == 0x01) {
                    flags &= ~0x01;
                } else {
                    if ((flags & 0x04) == 0x04) {
                        flags &= ~0x04;
                    } else if ((flags & 0x04) != 0x04) {
                        flags |= 0x04;
                    }
                    cmd++;
                    continue;
                }
            } else if (*cmd == ' ') {
                if ((flags & 0x01) == 0x01) {
                    flags &= ~0x01;
                } else if ((flags & 0x04) != 0x04) {
                    if ((flags & 0x02) != 0x02) {
                        flags |= 0x02;
                        *dst_ptr = '\0';
                        dst_ptr++;
                    }
                    cmd++;
                    continue;
                }
            }

            *dst_ptr = *cmd;

           if ((flags & 0x02) == 0x02) {
                flags &= ~0x02;
                if (cli_cmd_argc < CLI_ARGV_SIZE) {
                    cli_cmd_argv[cli_cmd_argc++] = dst_ptr;
                    cli_cmd_argv[cli_cmd_argc] = NULL;
                }
                // cli_cmd_argc++;
                // if (cli_cmd_argc < CLI_ARGV_SIZE - 1) {
                // } else {
                //     break;
                // }
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

/**
 * Register command
 *
 * @param item pointer to command instance
 */
void cli_regcmd(cli_node_t* item) {
    if (NULL != item) {
        cli_exec_append(&cli_exec_head, &cli_exec_tail, item);
    }
}

/**
 * Print command name, and spacer
 *
 * @param text command name
 * @param max  width of space
 */
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

/**
 * Main function for 'help' command
 *
 * @param argc count of arguments
 * @param argv arguments
 *
 * @return exec status
 */
CLI_COMMAND_MAIN(help)(int argc, char** argv) {
    if (argc < 2) {
        printf("\r\n");

        cli_node_t* ptr = cli_exec_head;
        while (ptr != NULL) {
            cli_help_cmd_print_cmd_name(ptr->c_name, 16);
            printf("%s\r\n", ptr->c_brief);
            ptr = ptr->next;
        }
        printf("\r\n");
    } else {
        cli_node_t* item = cli_exec_get_cmd(cli_exec_head, *(argv+1));
        if (item != NULL) {
            if (item->c_help != NULL) {
                printf("\r\n%s\r\n\r\n", item->c_help);
            } else {
                printf("\r\n%s\r\n\r\n", item->c_brief);
            }
        }
    }
    return 0;
}

CLI_COMMAND(help,
    "Print help info",
    "Print help info.\r\n" \
    "USAGE: help [<command_name>]"
)

/**
 * Initialize fommand 'help'
 */
void cli_help_cmd_init(void) {
    CLI_REGISTER_COMMAND(help);
}
