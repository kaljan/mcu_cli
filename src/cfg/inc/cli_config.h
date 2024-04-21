/**
 * @file    cli_config.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-20
 *
 * @brief
 */
#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

/* Command buffer size */
#define CLI_BUF_SIZE                128

/* Max count of arguments including command name */
#define CLI_ARGV_SIZE               32

/* Escape sequence buffer size */
#define CLI_ESC_BUF_SIZE            16

/* History buffer size */
#define CLI_HISTORY_BUF_LEN         512

/* Enable command input history */
#define CLI_HISTORY                 1

/* Enable command complete */
#define CLI_CMDCPLT                 1

/* Hello message. Prints at startup */
#define CLI_HELLO_MSG               "\r\nCommand line interface for MCUs\r\n"

/* Prompt text */
#define CLI_PROMPT                  "\e[1;34mbluepill\e[37m#\e[0m"

/* Getchar and putchar functions */
#define putchar_fn(c)               __io_putchar(c)
#define getchar_fn()                __io_getchar()

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Register all commands */
void cli_register_commands(void);
int __io_putchar(int ch);
int __io_getchar(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_CONFIG_H */
