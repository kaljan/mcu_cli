/**
 * @file    cli_opt.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-20
 *
 * @brief
 */
#ifndef CLI_OPT_H
#define CLI_OPT_H

#ifdef CLI_OPT
#include CLI_OPT
#else

#define CLI_CMD_COUNT           10 /* WARNING: Set command count */
#define CLI_BUF_SIZE            128
#define CLI_ARGV_SIZE           32
#define CLI_ESC_BUF_SIZE        16


/* Enable command input history */
#define CLI_HISTORY

/* Enable command complete */
#define CLI_CMDCPLT

#define CLI_HELLO_MSG   "\r\nCommand line interface for MCUs\r\n"
#define CLI_PROMPT      ">"

#define cli_register_commands()
#define putchar_fn(c)
#define getchar_fn()

#endif

#endif /* CLI_OPT_H */
