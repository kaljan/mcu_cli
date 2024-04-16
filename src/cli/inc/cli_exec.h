/**
 * @file    cli_exec.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-13
 *
 * @brief
 */
#ifndef CLI_EXEC_H
#define CLI_EXEC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

const char** cli_exec_acplt(const char* expr, int len, int*const num);
void cli_exec_cmd(const char* cmd, int size);
void cli_exec_init(void);
void cli_help_cmd_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_EXEC_H */
