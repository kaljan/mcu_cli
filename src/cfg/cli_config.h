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

#define CLI_CMD_COUNT           10
#define CLI_BUF_SIZE            128
#define CLI_ARGV_SIZE           32
#define CLI_ESC_BUF_SIZE        16

/* Enable command input history */
#define CLI_HISTORY

/* Enable command complete */
#define CLI_CMDCPLT

#define CLI_HELLO_MSG   "\r\nCommand line interface for MCUs\r\n"
#define CLI_PROMPT      "\e[1;34mbluepill\e[37m#\e[0m"

#define putchar_fn(c)   __io_putchar(c)
#define getchar_fn()    __io_getchar()

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void cli_register_commands(void);
int __io_putchar(int ch);
int __io_getchar(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_CONFIG_H */
