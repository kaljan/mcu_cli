/**
 * @file    cli_history.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-10
 *
 * @brief
 */

#ifndef CLI_HISTORY_H
#define CLI_HISTORY_H

#include "cli.h"

#if (CLI_HISTORY == 1)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void cli_history_clear(void);
void cli_history_add(const char* cmd);
const char* cli_history_get_next(void);
const char* cli_history_get_prev(void);
void cli_history_cmd_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CLI_HISTORY == 1 */
#endif /* CLI_HISTORY_H */

