/**
 * @file    main.c
 * @author  kaljan
 * @version 1.0.0
 * @date    27-11-20 14:35:40
 *
 * @brief
 *
 */

#include "hal.h"
#include "hal_gpio.h"

#include "clcd.h"
#include "tools.h"
#include "drivers_rcu.h"
#include "stm32f1xx.h"
#include "version.h"
#include "cli.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if defined DEBUG || defined RELEASE
#if DEBUG == 1
const char* BUILD_MODE="debug";
#elif RELEASE == 1
const char* BUILD_MODE="release";
#else
const char* BUILD_MODE="build";
#endif
#endif

static void RCU_EventCallback(void* context, uint32_t event) {
    clcd_clear();
    if (event > 0xFFFF) {
        clcd_print(0x00, "RCU: 0x%06X", event);
    } else {
        clcd_print(0x00, "RCU: 0x%04X", event);
    }
}


int main(void) {
    char esc_seq = 0;
    char esc_buf[8];
    uint32_t esc_cursor = 0;

    hal_init();
    cli_start();

    clcd_pwren_init();
    clcd_power_enable();

    clcd_init();
    clcd_print(0x00
        , "v%d.%d.%d %s"
        , MCLI_VERSION_MAJOR
        , MCLI_VERSION_MINOR
        , MCLI_VERSION_BUILD
        , BUILD_MODE);

    RCU_Initialize();
    RCU_SetCallback(RCU_EventCallback, NULL);

    while (1) {
        RCU_Update();
        cli_update();
    }
}

