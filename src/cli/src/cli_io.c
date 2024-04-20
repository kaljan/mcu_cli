/**
 * @file    cli_io.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-13
 *
 * @brief   Command line interface input/output handler
 *
 * +--------------+---------+---------------------------------------+
 * | KEY          | CODE**  | FUNCTION                              |
 * +--------------+---------+---------------------------------------+
 * | Up           | \e[A    | Next item from history                |
 * | Down         | \e[B    | Previous item from history            |
 * | Right        | \e[C    | Move cursor right one character       |
 * | Left         | \e[D    | Move cursor left one character        |
 * | CTRL+Right   | \e[1;3C | Move to end of word                   |
 * | CTRL+Left    | \e[1;3D | Move to beginning of word             |
 * | Home         | \e[1~   | Move to beginning of command line     |
 * | End          | \eOF    | Move to end of command line           |
 * | Delete       | \e[3~   | Delete character under cursor         |
 * | Backspace    | 0x08    | Remove character before cursor        |
 * | Enter        | 0x0D    | Run command                           |
 * | TAB          | 0x09    | Complete command                      |
 * | CTRL+Delete  | \e[3;5~ | Remove word after cursor              |
 * | ALT+D        | \ed     | Remove word after cursor              |
 * | CTRL+A       | 0x01    | Move cursor to obeginning of line     |
 * | CTRL+B       | 0x02    | Move cursor left one character        |
 * | CTRL+C       | 0x03    | At the momen its just skipp all input |
 * | CTRL+D       | 0x04    | Delete character under cursor         |
 * | CTRL+E       | 0x05    | Move cursor to end of line            |
 * | CTRF+F       | 0x06    | Move cursor right                     |
 * | CTRL+H       | 0x08    | Remove character before cursor        |
 * | CTRL+I       | 0x09    | Complete command                      |
 * | CTRL+K       | 0x0B    | Clear all after cursor                |
 * | CTRL+M       | 0x0D    | Run command                           |
 * | CTRL+N       | 0x0E    | Next item from history                |
 * | CTRL+P       | 0x10    | Previous item from history            |
 * | CTRL+U       | 0x15    | Clear all before cursor               |
 * | CTRL+W       | 0x17    | Remove word before cursor             |
 * +--------------+---------+---------------------------------------+
 */

#include "cli.h"
#include "cli_history.h"
#include "cli_exec.h"


/* Private definitions */

/* Control characters */

#define CLI_CC_NUL              0x00 /* [ CTRL+@ ] '\0' NULL */
#define CLI_CC_SOH              0x01 /* [ CTRL+A ] START OF HEADING */
#define CLI_CC_STX              0x02 /* [ CTRL+B ] START OF TEXT */
#define CLI_CC_ETX              0x03 /* [ CTRL+C ] END OF TEXT */
#define CLI_CC_EOT              0x04 /* [ CTRL+D ] END OF TRANSMISSION */
#define CLI_CC_ENQ              0x05 /* [ CTRL+E ] ENQUIRY */
#define CLI_CC_ACK              0x06 /* [ CTRL+F ] ACKNOWLEDGE */
#define CLI_CC_BEL              0x07 /* [ CTRL+G ] '\a' BELL */
#define CLI_CC_BS               0x08 /* [ CTRL+H ] '\b' BACKSPACE */
#define CLI_CC_TAB              0x09 /* [ CTRL+I ] '\t' CHARACTER TABULATION (horizontal tabulation) */
#define CLI_CC_LF               0x0A /* [ CTRL+J ] '\n' LINE FEED*/
#define CLI_CC_VT               0x0B /* [ CTRL+K ] '\v' LINE TABULATION (vertical tabulation) */
#define CLI_CC_FF               0x0C /* [ CTRL+L ] '\f' FORM FEED */
#define CLI_CC_CR               0x0D /* [ CTRL+M ] '\r' CARRIAGE RETURN */
#define CLI_CC_SO               0x0E /* [ CTRL+N ] SHIFT OUT (locking-shift one) */
#define CLI_CC_SI               0x0F /* [ CTRL+O ] SHIFT IN (locking-shift zero) */
#define CLI_CC_DLE              0x10 /* [ CTRL+P ] DATA LINK ESCAPE */
#define CLI_CC_DC1              0x11 /* [ CTRL+Q ] DEVICE CONTROL ONE */
#define CLI_CC_DC2              0x12 /* [ CTRL+R ] DEVICE CONTROL TWO */
#define CLI_CC_DC3              0x13 /* [ CTRL+S ] DEVICE CONTROL THREE */
#define CLI_CC_DC4              0x14 /* [ CTRL+T ] DEVICE CONTROL FOUR */
#define CLI_CC_NAK              0x15 /* [ CTRL+U ] NEGATIVE ACKNOWLEDGE */
#define CLI_CC_SYN              0x16 /* [ CTRL+V ] SYNCHRONOUS IDLE */
#define CLI_CC_ETB              0x17 /* [ CTRL+W ] END OF TRANSMISSION BLOCK */
#define CLI_CC_CAN              0x18 /* [ CTRL+X ] CANCEL */
#define CLI_CC_EM               0x19 /* [ CTRL+Y ] END OF MEDIUM */
#define CLI_CC_SUB              0x1A /* [ CTRL+Z ] SUBSTITUTE */
#define CLI_CC_ESC              0x1B /* [ CTRL+[ ] `\e` ESCAPE */
#define CLI_CC_FS               0x1C /* [ CTRL+\ ] INFORMATION SEPARATOR FOUR (file separator) */
#define CLI_CC_GS               0x1D /* [ CTRL+] ] INFORMATION SEPARATOR THREE (group separator) */
#define CLI_CC_RS               0x1E /* [ CTRL+^ ] INFORMATION SEPARATOR TWO (record separator) */
#define CLI_CC_US               0x1F /* [ CTRL+_ ] INFORMATION SEPARATOR ONE (unit separator) */
#define CLI_CC_DEL              0x7F /* [ CTRL+? ] DELETE */

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
 * This function move text left after remove operation
 *
 * @param n  size of text fo move
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
 * This function copy command to command buffer
 *
 * @param cmd  pointer to command string
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


/* Shortcuts functionality section */

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


/**
 * Delete character before cursor
 */
static void cli_io_backspace(void) {
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

/**
 * Clear all after cursor
 */
static void cli_io_clra(void) {
    if ((cli_io_cursor > 0) && (cli_io_bytes > 0) &&
        (cli_io_cursor < cli_io_bytes)) {
        cli_io_cursor = 0;
        cli_io_bytes = 0;
        printf("\e[J");
        fflush(stdout);
    }
}

/**
 * Clear all before cursor
 */
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
 * Next item in the command history
 */
static void cli_io_hstnext(void) {
    const char* ptr = cli_history_get_next();
    cli_io_setcmd(ptr);
}

/**
 * Previous item in the command history
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

/**
 * At the momen its just skip all input
 *
 * TODO: implemen kill of program execution.
 */
static void cli_io_kill(void) {
    printf("^C\r\n%s ", cli_io_prompt);
    fflush(stdout);
    cli_io_cursor = 0;
    cli_io_bytes = 0;
}

/**
 * Complete command
 */
static void cli_io_acplt(void) {
    if (cli_io_bytes > 0) {
        if (cli_io_acplt_buf == NULL) {
            cli_io_buf[cli_io_bytes] = '\0';
            cli_io_acplt_buf = cli_exec_acplt(cli_io_buf, cli_io_cursor, &cli_io_acplt_items);
            cli_io_acplt_curr = 0;
        }

        if ((cli_io_acplt_buf != NULL) && (cli_io_acplt_items > 0)) {
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

/**
 * Run command
 */
static inline void cli_io_ret(void) {
    putchar_fn('\r');
    putchar_fn('\n');

    cli_io_buf[cli_io_bytes] = '\0';
    cli_exec_cmd(cli_io_buf, cli_io_bytes);

    cli_io_bytes = 0;
    cli_io_cursor = 0;
    printf("%s ", cli_io_prompt);
    fflush(stdout);
}


/* Input/outout functionality section */

/**
 * Handle special characters.
 *
 * This function handle received character codes from 0 to 0x1F
 *
 * @param c received character code
 */
static void cli_io_ctrl(int c) {
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
        return;

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
        break;

    default:
        break;
    }

    cli_io_acplt_buf = NULL;
    cli_io_acplt_curr = 0;
    cli_io_acplt_items = 0;
}

/**
 * CSI escape sequence handler
 */
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

/**
 * Escape sequence handler
 *
 * @param c received character code
 */
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
                if (c == 'F') { /* END */
                    cli_io_mvcrse();
                }
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


/**
 * Handle received printable characters
 *
 * @param c  received character code
 */
static void cli_io(int c) {
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
    }
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
    } else if (c < ' ') {
        cli_io_ctrl(c);
    } else {
        cli_io(c);
    }
}
