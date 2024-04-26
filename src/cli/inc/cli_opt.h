/**
 * @file    cli_opt.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-20
 *
 * @brief   Default CLI options and configurations
 */
#ifndef CLI_OPT_H
#define CLI_OPT_H

/* Set this macro in makefile or global build
   config if you need to use your own config */
#ifdef CLI_OPT
#include CLI_OPT
#else

/* Command buffer size */
#define CLI_BUF_SIZE                128

/* Max count of arguments including command name */
#define CLI_ARGV_SIZE               32

/* Escape sequence buffer size */
#define CLI_ESC_BUF_SIZE            16

/* History buffer size */
#define CLI_HISTORY_BUF_LEN         1024

/* Enable command input history */
#define CLI_HISTORY                 1

/* Enable command complete */
#define CLI_CMDCPLT                 1

/* Hello message. Prints at startup */
#define CLI_HELLO_MSG               "\r\nCommand line interface for MCUs\r\n"

/* Prompt text */
#define CLI_PROMPT                  ">"

/* Register all commands */
#define cli_register_commands()

/* Getchar and putchar functions */
#define putchar_fn(c)
#define getchar_fn()

#endif

#endif /* CLI_OPT_H */
