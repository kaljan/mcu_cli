/**
 * @file    cli_gpio.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-02
 *
 * @brief
 */
#ifndef CLI_GPIO_H
#define CLI_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void cli_gpio_info_cmd_init(void);
void cli_gpio_dir_cmd_init(void);
void cli_gpio_set_cmd_init(void);
void cli_gpio_get_cmd_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_GPIO_H */
