/**
 * @file    cli.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-03-23
 *
 * @brief
 */
#ifndef CLI_H
#define CLI_H

#include "static_map.h"


#define CLI_CMD_COUNT           10 /* WARNING: Set command count */
#define CLI_BUF_SIZE            255
#define CLI_ARGV_SIZE           32
#define CLI_ESC_BUF_SIZE        16

/* Flags */
#define CLI_ESC_MASK            0x0F
#define CLI_ESC_FLAG            0x01
#define CLI_ESC_TYPE_FLAG       0x02
#define CLI_ESC_CSI_FLAG        0x04
#define CLI_ESC_CSI_SEC_FLAG    0x08


typedef struct cli_node {
    /* Command name */
    const char* c_name;

    /* Brief description */
    const char* brief;

    /* Poniter to main command function */
    int (*main_fn)(int, char**);

    /* Pointer to print help function */
    void (*help_fn)(void);

    /* Static map node */
    sm_node_t node;
} cli_node_t;

typedef struct cli_config {
    const char* hello_msg;
    const char* prompt;
    void (*init_cmds_fn)(void);
} cli_config_t;

/* IO functions */

#define putchar_fn(c)   __io_putchar(c)
#define getchar_fn()    __io_getchar()


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int __io_putchar(int ch);
int __io_getchar(void);


void cli_append_command(cli_node_t* cmd);
void cli_start(cli_config_t* config);
void cli_update(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_H */
