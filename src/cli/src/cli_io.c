/**
 * @file    cli_io.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-13
 *
 * @brief
 *
 * +--------------+----------+
 * | KEY          | CODE     |
 * +--------------+----------+
 * | Up           | \e[A     |
 * | Down         | \e[B     |
 * | Right        | \e[C     |
 * | Left         | \e[D     |
 * | CTRL + Right | \e[1;3C  |
 * | CTRL + Left  | \e[1;3D  |
 * | Home         | \e[1~    |
 * | End          | \eOF     |
 * | Delete       | \e[3~    |
 * | TAB          | 0x09     |
 * | CTRL+K       | 0x0B     |
 * | CTRL+R       | 0x12     |
 * | CTRL+U       | 0x15     |
 * | CTRL+W       | 0x17     |
 * | CTRL+Delete  | \e[3;5~  |
 * +--------------+----------+
 */
#include "cli_io.h"
#include "cli.h"
#include "cli_history.h"
#include "cli_exec.h"
#include "version.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>


/* Private variables */

static const char* cli_io_prompt = ">";

static int cli_io_flags = 0;

static int cli_io_cursor = 0;
static int cli_io_bytes = 0;
static char cli_io_buf[CLI_BUF_SIZE];

static int cli_io_csi_bytes = 0;
static char cli_io_csi_buf[CLI_ESC_BUF_SIZE];

static int cli_io_acplt_items = 0;
static int cli_io_acplt_curr = 0;
static const char** cli_io_acplt_buf = NULL;


/* Private functions */

/* Common functionality */

static void cli_io_acplt_reset(void) {
    cli_io_acplt_buf = NULL;
    cli_io_acplt_curr = 0;
    cli_io_acplt_items = 0;
}

/**
 * Move cursor back
 *
 * @param n number of position
 */
static void cli_io_mvcrs(int n) {
    if (n < 5) {
        for (int i = 0; i < n; i++) {
            putchar_fn('\b');
        }
    } else {
        printf("\e[%dD", n);
        fflush(stdout);
    }
}

/**
 * Move text
 *
 * @param n
 */
static void cli_io_mvtxt(int n) {
    for (int i = 0; i < n; i++) {
        char c = cli_io_buf[i + cli_io_cursor + 1];
        cli_io_buf[i + cli_io_cursor] = c;
        putchar_fn((int)c);
    }

    printf("\e[J");
    fflush(stdout);

    cli_io_mvcrs(n);
}

/**
 * Set command
 *
 * @param cmd
 */
static void cli_io_setcmd(const char* cmd) {
    if ((cmd != NULL) && (strlen(cmd) > 0)) {
        memmove(cli_io_buf, cmd, strlen(cmd));
        cli_io_bytes = strlen(cmd);
        cli_io_cursor = strlen(cmd);
        printf("\r%s %s\e[J", cli_io_prompt, cmd);
        fflush(stdout);
    }
}


/* Shortcuts functionality */

/**
 * Delete char under cursor
 */
static void cli_io_delchr(void) {
    int n;
    cli_io_bytes--;
    n = cli_io_bytes - cli_io_cursor;
    cli_io_mvtxt(n);
}

/**
 * Delete char under cursor
 */
void cli_io_delchrc(void) {
    if ((cli_io_bytes > 0) && (cli_io_cursor < cli_io_bytes)) {
        cli_io_delchr();
    }
}



static void cli_io_backspace(void) {
    /* CTRL+H */
    if ((cli_io_bytes > 0) && (cli_io_cursor > 0)) {
        if (cli_io_cursor == cli_io_bytes) {
            cli_io_cursor--;
            cli_io_bytes--;
            printf("\b\e[J");
            fflush(stdout);
        } else if (cli_io_cursor < cli_io_bytes) {
            int n = cli_io_bytes - cli_io_cursor;
            cli_io_cursor--;
            cli_io_bytes--;
            putchar_fn('\b');
            cli_io_mvtxt(n);
        }
    }
}

static void cli_io_clra(void) {
    if ((cli_io_cursor > 0) && (cli_io_bytes > 0) &&
        (cli_io_cursor < cli_io_bytes)) {
        cli_io_cursor = 0;
        cli_io_bytes = 0;
        printf("\e[J");
        fflush(stdout);
    }
}

static void cli_io_clrb(void) {
    if ((cli_io_cursor > 0) && (cli_io_bytes > 0)) {

        printf("\r%s ", cli_io_prompt);
        fflush(stdout);
        if (cli_io_cursor < cli_io_bytes) {
            for (int i = 0; i < cli_io_bytes - cli_io_cursor; i++) {
                cli_io_buf[i] = cli_io_buf[i + cli_io_cursor];
                putchar_fn(cli_io_buf[i]);
            }
            cli_io_bytes -= cli_io_cursor;
            cli_io_cursor = 0;
            printf("\e[J");
            fflush(stdout);
            cli_io_mvcrs(cli_io_bytes);
        } else {
            cli_io_cursor = 0;
            cli_io_bytes = 0;
            printf("\r%s \e[J", cli_io_prompt);
            fflush(stdout);
        }
    }
}

/**
 * Delete word after cursor
 */
static void cli_io_delworda(void) {
    while ((cli_io_cursor < cli_io_bytes) && (cli_io_buf[cli_io_cursor] == ' ')) {
        cli_io_delchr();
    }

    while ((cli_io_cursor < cli_io_bytes) && (cli_io_buf[cli_io_cursor] != ' ')) {
        cli_io_delchr();
    }
}

/**
 * Delete word before cursor
 */
static void cli_io_delwordb(void) {
    while ((cli_io_cursor > 0) && (cli_io_buf[cli_io_cursor - 1] == ' ')) {
        cli_io_backspace();
    }

    while ((cli_io_cursor > 0) && (cli_io_buf[cli_io_cursor - 1] != ' ')) {
        cli_io_backspace();
    }
}

/**
 * History next
 */
static void cli_io_hstnext(void) {
    const char* ptr = cli_history_get_next();
    cli_io_setcmd(ptr);
}

/**
 * History prev
 */
static void cli_io_hstprev(void) {
    const char* ptr = cli_history_get_prev();
    cli_io_setcmd(ptr);
}

/**
 * Move cursor right
 */
static void cli_io_mvcrsr(void) {
    if (cli_io_cursor < cli_io_bytes) {
        putchar_fn(cli_io_buf[cli_io_cursor]);
        cli_io_cursor++;
    }
}

/**
 * Move cursor left
 */
static void cli_io_mvcrsl(void) {
    if (cli_io_cursor > 0) {
        cli_io_cursor--;
        putchar_fn('\b');
    }
}

/**
 * Move cursor to beginnig of line
 */
static void cli_io_mvcrsh(void) {
    if ((cli_io_cursor > 0) && (cli_io_bytes > 0)) {
        if (cli_io_cursor < 4) {
            while (cli_io_cursor > 0) {
                putchar_fn('\b');
                cli_io_cursor--;
            }
        } else {
            printf("\e[%dD", cli_io_cursor);
            fflush(stdout);
            cli_io_cursor = 0;
        }
    }
}

/**
 * Move cursor to end of line
 */
static void cli_io_mvcrse(void) {
    while (cli_io_cursor < cli_io_bytes) {
        putchar_fn(cli_io_buf[cli_io_cursor]);
        cli_io_cursor++;
    }
}

/**
 * Move cursor to beginning of word
 */
static void cli_io_mvcrsbw(void) {
    if (cli_io_cursor < cli_io_bytes) {
        putchar_fn(cli_io_buf[cli_io_cursor]);
        cli_io_cursor++;
    }

    while ((cli_io_cursor < cli_io_bytes) && (cli_io_buf[cli_io_cursor] == ' ')) {
        putchar_fn(cli_io_buf[cli_io_cursor]);
        cli_io_cursor++;
    }

    while ((cli_io_cursor < cli_io_bytes) && (cli_io_buf[cli_io_cursor] != ' ')) {
        putchar_fn(cli_io_buf[cli_io_cursor]);
        cli_io_cursor++;
    }
}

/**
 * Move cursor to end of word
 */
static void cli_io_mvcrsew(void) {
    if (cli_io_cursor > 0) {
        cli_io_cursor--;
        putchar_fn('\b');
    }
    while ((cli_io_cursor > 0) && (cli_io_buf[cli_io_cursor] == ' ')) {
        cli_io_cursor--;
        putchar_fn('\b');
    }

    while ((cli_io_cursor > 0) && (cli_io_buf[cli_io_cursor - 1] != ' ')) {
        cli_io_cursor--;
        putchar_fn('\b');
    }
}

static void cli_io_kill(void) {
    /* CTRL+C */
    printf("^C\r\n%s ", cli_io_prompt);
    fflush(stdout);
    cli_io_cursor = 0;
    cli_io_bytes = 0;
}


/**
 * Autocomplete
 */
static void cli_io_acplt(void) {
    /* TAB */
    if (cli_io_bytes > 0) {
        if (cli_io_acplt_buf == NULL) {
            cli_io_buf[cli_io_bytes] = '\0';
            cli_io_acplt_buf = cli_exec_acplt(cli_io_buf, cli_io_cursor, &cli_io_acplt_items);
            cli_io_acplt_curr = 0;
        }
        // else {
            // cli_io_bytes = cli_io_cursor;
        // }

        if ((cli_io_acplt_buf != NULL) && (cli_io_acplt_items > 0)) {
            // int old_bytes = cli_io_bytes;
            const char * src = cli_io_acplt_buf[cli_io_acplt_curr];
            cli_io_bytes = cli_io_cursor;
            while (src[cli_io_bytes] != 0) {
                cli_io_buf[cli_io_bytes] = src[cli_io_bytes];
                putchar_fn(cli_io_buf[cli_io_bytes]);
                cli_io_bytes++;
            }

            if (cli_io_acplt_items > 1) {
                cli_io_acplt_curr++;
                if (cli_io_acplt_curr >= cli_io_acplt_items) {
                    cli_io_acplt_curr = 0;
                }
                printf("\e[J");
                fflush(stdout);
                cli_io_mvcrs(cli_io_bytes - cli_io_cursor);
            } else {
                cli_io_buf[cli_io_bytes] = ' ';
                putchar_fn(cli_io_buf[cli_io_bytes]);
                cli_io_bytes++;
                cli_io_cursor = cli_io_bytes;
                cli_io_acplt_buf = NULL;
                cli_io_acplt_items = 0;
            }
        }
    }
}

static inline void cli_io_ret(void) {
    /* Enter */
    putchar_fn('\r');
    putchar_fn('\n');

    cli_io_buf[cli_io_bytes] = '\0';
    cli_exec_cmd(cli_io_buf, cli_io_bytes);

    cli_io_bytes = 0;
    cli_io_cursor = 0;
    printf("%s ", cli_io_prompt);
    fflush(stdout);
}


/* Keyboard functionality */

static int cli_io_ctrl(int c) {
    int ret = 0;
    switch (c) {
    case CLI_CC_SOH:
        cli_io_mvcrsh();
        break;

    case CLI_CC_STX:
        cli_io_mvcrsl();
        break;

    case CLI_CC_ETX:
        cli_io_kill();
        break;

    case CLI_CC_EOT:
        cli_io_delchrc();
        break;

    case CLI_CC_ENQ:
        cli_io_mvcrse();
        break;

    case CLI_CC_ACK:
        cli_io_mvcrsr();
        break;

    case CLI_CC_BS:
        cli_io_backspace();
        break;

    case CLI_CC_TAB:
        cli_io_acplt();
        return 0;

    case CLI_CC_VT:
        cli_io_clra();
        break;

    case CLI_CC_CR:
        cli_io_ret();
        break;

    case CLI_CC_SO:
        cli_io_hstnext();
        break;

    case CLI_CC_DLE:
        cli_io_hstprev();
        break;

    case CLI_CC_NAK:
        cli_io_clrb();
        break;

    case CLI_CC_ETB:
        cli_io_delwordb();
        break;

    case CLI_CC_ESC:
        cli_io_flags &= ~(CLI_ESC_MASK);
        cli_io_flags |= CLI_ESC_FLAG;
        ret = 0;
        break;

    default:
        ret = c;
        break;
    }
    cli_io_acplt_reset();
    return ret;
}


static void cli_io_csi(void) {
    if (cli_io_csi_bytes > 0) {
        if (cli_io_csi_buf[0] == 'A') { /* Up */
            cli_io_hstnext();
        } else if (cli_io_csi_buf[0] == 'B') { /* Down */
            cli_io_hstprev();
        } else if (cli_io_csi_buf[0] == 'C') { /* Right */
            cli_io_mvcrsr();
        } else if (cli_io_csi_buf[0] == 'D') { /* Left */
            cli_io_mvcrsl();
        } else if ((cli_io_csi_buf[0] == '1')) {
            if (cli_io_csi_buf[1] == '~') { /* Home */
                cli_io_mvcrsh();
            } else if (cli_io_csi_buf[1] == ';') {
                if (cli_io_csi_buf[2] == '5') {
                    if (cli_io_csi_buf[3] == 'C') { /* CTRL + LEFT */
                        cli_io_mvcrsbw();
                    } else if (cli_io_csi_buf[3] == 'D') { /* CTRL + RIGHT */
                        cli_io_mvcrsew();
                    }
                }
            }
        } else if ((cli_io_csi_buf[0] == '3')) {
            if (cli_io_csi_buf[1] == '~') { /* Delete */
                cli_io_delchrc();
            } else if (cli_io_csi_buf[1] == ';') {
                if ((cli_io_csi_buf[2] == '5') && (cli_io_csi_buf[3] == '~')) { /* CTRL + Delete */
                    cli_io_delworda();
                }
            }
        }
    }
}

static void cli_io_ss3(int c) {
    if (c == 'F') { /* END */
        cli_io_mvcrse();
    }
}

static void cli_io_esc(int c) {
    do {
        if ((cli_io_flags & CLI_ESC_TYPE_FLAG) == CLI_ESC_TYPE_FLAG) {
            if ((cli_io_flags & CLI_ESC_CSI_FLAG) == CLI_ESC_CSI_FLAG) {
                if ((c >= 0x20) && (c <= 0x2F)) {
                    cli_io_flags |= CLI_ESC_CSI_SEC_FLAG;
                    cli_io_csi_buf[cli_io_csi_bytes] = (char)c;
                    cli_io_csi_bytes++;
                } else if ((c >= 0x30) && (c <= 0x3F)) {
                    if ((cli_io_flags & CLI_ESC_CSI_SEC_FLAG) == CLI_ESC_CSI_SEC_FLAG) {
                        break;
                    }
                    cli_io_csi_buf[cli_io_csi_bytes] = (char)c;
                    cli_io_csi_bytes++;
                } else if (c >= 0x40 && c <= 0x7E) {
                    cli_io_csi_buf[cli_io_csi_bytes] = (char)c;
                    cli_io_csi_buf[cli_io_csi_bytes + 1] = 0;
                    cli_io_csi_bytes++;
                    cli_io_csi();
                    break;
                }
            } else {
                cli_io_ss3(c);
                break;
            }
        } else if (c == '[') {
            cli_io_flags |= CLI_ESC_TYPE_FLAG | CLI_ESC_CSI_FLAG;
        } else if (c == 'O') {
            cli_io_flags |= CLI_ESC_TYPE_FLAG;
        } else if (c == 'd') {
            cli_io_delworda();
            break;
        } else {
            break;
        }

        return;
    } while (0);

    cli_io_csi_bytes = 0;
    cli_io_flags &= ~(CLI_ESC_MASK);
}

/* Text functionality */

static int cli_io(int c) {
    if ((c >= ' ') && (c <= '~') && (cli_io_bytes < CLI_BUF_SIZE)) {
        if (cli_io_bytes == cli_io_cursor) {
            cli_io_buf[cli_io_bytes] = (char)c;
            cli_io_bytes++;
            cli_io_cursor++;
            putchar_fn(c);
        } else if (cli_io_cursor < cli_io_bytes) {
            if (cli_io_bytes < (CLI_BUF_SIZE - 2)) {
                int n = cli_io_bytes - cli_io_cursor;
                char c1 = 0;

                for (int i = 0; i < n; i++) {
                    char c2 = cli_io_buf[i + cli_io_cursor];
                    if (c1 == 0) {
                        cli_io_buf[i + cli_io_cursor] = c;
                        putchar_fn(c);
                    } else {
                        cli_io_buf[i + cli_io_cursor] = c1;
                    }
                    c1 = c2;
                    putchar_fn(c1);
                }

                cli_io_buf[cli_io_bytes] = c1;
                cli_io_bytes++;
                cli_io_cursor++;
                n = cli_io_bytes - cli_io_cursor;
                cli_io_mvcrs(n);
            }
        } else {
            cli_io_cursor = cli_io_bytes;
        }
        return 0;
    }
    return c;
}


/* Public functions */

void cli_start(cli_config_t* config) {
    const char* hello_msg = "\r\nCommand line interface for MCUs\r\n";
    cli_history_clear();
    cli_exec_init();
    cli_history_cmd_init();

    if (config != NULL) {
        if (config->prompt != NULL) {
            cli_io_prompt = config->prompt;
        }

        if (config->hello_msg != NULL) {
            hello_msg = config->hello_msg;
        }

        if (config->init_cmds_fn != NULL) {
            config->init_cmds_fn();
        }
    }

    cli_help_cmd_init();

    printf("\r\n%sVersion %d.%d.%d\r\n\r\n%s "
        , hello_msg
        , MCLI_VERSION_MAJOR
        , MCLI_VERSION_MINOR
        , MCLI_VERSION_BUILD
        , cli_io_prompt);
    fflush(stdout);
}

void cli_update(void) {
    int c = getchar_fn();
    if (c < 0) {
        return;
    } else if ((cli_io_flags & CLI_ESC_FLAG) == CLI_ESC_FLAG) {
        cli_io_esc(c);
    } else {
        if (cli_io_ctrl(c) != 0) {
            cli_io(c);
        }
    }
}
