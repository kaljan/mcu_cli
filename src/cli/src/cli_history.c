/**
 * @file    cli_history.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-10
 *
 * @brief
 */

#include "cli_history.h"

#if (CLI_HISTORY == 1)

static int ch_cursor = 0;
static char ch_buf[CLI_HISTORY_BUF_LEN];

static char* ch_head_ptr = NULL;
static char* ch_tail_ptr = NULL;
static char* ch_curr_ptr = NULL;

static char* const ch_buf_start = ch_buf;
static char* const ch_buf_end = &ch_buf[CLI_HISTORY_BUF_LEN - 1];


void cli_history_clear(void) {
    ch_head_ptr = NULL;
    ch_tail_ptr = NULL;
    ch_curr_ptr = NULL;
    ch_cursor = 0;
    memset(ch_buf, 0, CLI_HISTORY_BUF_LEN);
}

void cli_history_add(const char* cmd) {
    if ((NULL != cmd) && (strlen(cmd) > 0)) {
        char* ptr = NULL;
        ch_curr_ptr = NULL;
        if (NULL == ch_head_ptr) {
            if (strlen(cmd) >= CLI_HISTORY_BUF_LEN) {
                return;
            }
            ch_head_ptr = ch_buf;
            while (*cmd != 0) {
                ch_buf[ch_cursor++] = *cmd;
                cmd++;
            }
            ch_cursor++;
            return;
        } else if (strlen(cmd) >= CLI_HISTORY_BUF_LEN - ch_cursor) {
            ch_cursor = 0;
            ptr = &ch_buf[ch_cursor];
        }

        ch_tail_ptr = &ch_buf[ch_cursor];
        while (*cmd != 0) {
            if (&ch_buf[ch_cursor] == ch_head_ptr) {
                ptr = ch_head_ptr;
            }
            ch_buf[ch_cursor++] = *cmd;
            cmd++;
            if (ptr != NULL) {
                ptr++;
            }
        }

        if (NULL != ptr) {
            while (*ptr != 0) {
                *ptr = 0;
                ptr++;
                if (ptr >= ch_buf_end) {
                    ptr = ch_buf_start;
                    break;
                }
            }

            while (*ptr == 0) {
                if (ptr >= ch_buf_end) {
                    ptr = ch_buf_start;
                }
                ptr++;
            }
            ch_head_ptr = ptr;
        }
        ch_cursor++;
    }
}

const char* cli_history_get_next(void) {
    if (NULL != ch_tail_ptr) {
        if (NULL == ch_curr_ptr) {
            ch_curr_ptr = ch_tail_ptr;
        } else if (ch_curr_ptr == ch_head_ptr) {
            ch_curr_ptr = ch_tail_ptr;
        } else {
            if (ch_curr_ptr == ch_buf_start) {
                ch_curr_ptr = ch_buf_end;
            }
            ch_curr_ptr--;

            while (*ch_curr_ptr == '\0') {
                ch_curr_ptr--;
            }
            while (*(ch_curr_ptr - 1) != '\0') {
                ch_curr_ptr--;
                if (ch_curr_ptr < ch_buf_start) {
                    ch_curr_ptr = ch_buf_start;
                    break;
                }
            }
        }
        return ch_curr_ptr;
    } else if (NULL != ch_head_ptr) {
        return ch_head_ptr;
    }
    return NULL;
}

const char* cli_history_get_prev(void) {
    if (NULL != ch_head_ptr) {
        if (NULL == ch_curr_ptr) {
            ch_curr_ptr = ch_head_ptr;
        } else if (ch_curr_ptr == ch_tail_ptr) {
            ch_curr_ptr = ch_head_ptr;
        } else {
            while (*ch_curr_ptr != 0) {
                ch_curr_ptr++;
                if (ch_curr_ptr >= ch_buf_end) {
                    ch_curr_ptr = ch_buf_start;
                    return ch_curr_ptr;
                }
            }
            while (*ch_curr_ptr == 0) {
                ch_curr_ptr ++;
                if (ch_curr_ptr >= ch_buf_end) {
                    ch_curr_ptr = ch_buf_start;
                    break;
                }
            }

        }
        return ch_curr_ptr;
    }
    return NULL;
}

static int cli_history_cmd_main(int argc, char** argv) {
    if (argc > 1) {
        argv++;
        if (strcmp(*argv, "clear") == 0) {
            cli_history_clear();
        } else {
            printf("Bad argument: \'%s\'\r\n", *argv);
            return -1;
        }
    } else if ((NULL != ch_head_ptr) && (NULL != ch_tail_ptr)) {
        char* ptr = ch_head_ptr;
        int index = 0;
        do {
            printf("%2d %s\r\n", index++, ptr);
            while (*ptr != 0) {
                ptr++;
            }

            while (*ptr == 0) {
                ptr++;
                if (ptr >= ch_buf_end) {
                    ptr = ch_buf_start;
                }
            }

        } while (ptr != ch_tail_ptr);
        printf("%2d %s\r\n", index++, ch_tail_ptr);
    }
    return 0;
}

static void cli_history_cmd_help(void) {
    printf(
        "\r\nPrint or clear command history.\r\n"
        );
}

static cli_node_t cli_history_cmd = {
    .c_name = "history",
    .brief = "print or clear command history",
    .main_fn = cli_history_cmd_main,
    .help_fn = cli_history_cmd_help,
};

void cli_history_cmd_init(void) {
    cli_history_clear();
    cli_regcmd(&cli_history_cmd);
}

#endif /* CLI_HISTORY == 1 */
