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


#include "cli_opt.h"

#include "static_map.h"
#include "version.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

/* IO functions */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Register command
 *
 * @param cmd
 */
void cli_regcmd(cli_node_t* cmd);

/**
 * Start CLI
 *
 * @param config pointer to configuration structure
 */
void cli_start(void);

/**
 * Updage command line
 */
void cli_update(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_H */
