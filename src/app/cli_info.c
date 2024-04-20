/**
 * @file    cli_info.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-02
 *
 * @brief
 */

#include "stm32f1xx.h"
#include "cli.h"
#include "stm32f1xx_rcc.h"


static int cli_info_main(int argc, char** argv) {
    RCC_ClockFreq_t ckcfg;
    RCC_ClockGetFreq(&ckcfg);
    printf(
        "\r\nDEVICE INFO\r\n\r\n"
        "Revision ID   : 0x%04X\r\n"
        "Device ID     : 0x%04X\r\n"
        "Device UID    : %08X%08X%08X\r\n"
        "Flash size    : %d KiB\r\n"
        "\r\n"
        "Implementer   : 0x%02X\r\n"
        "Variant       : 0x%1X\r\n"
        "Part number   : 0x%04X\r\n"
        "Patch release : 0x%1X\r\n"
        "\r\n"
        "System clock  : %u Hz\r\n"
        "AHP clock     : %u Hz\r\n"
        "APB1 clock    : %u Hz\r\n"
        "APB2 clock    : %u Hz\r\n"
        "\r\n"
        , (((DBGMCU->IDCODE) >> 16) & 0x0000FFFFU)
        , (DBGMCU->IDCODE) & 0x00000FFFU
        , *((uint32_t *)UID_BASE)
        , *((uint32_t *)(UID_BASE + 4U))
        , *((uint32_t *)(UID_BASE + 8U))
        , (int)(*((uint16_t*)FLASHSIZE_BASE))
        , (SCB->CPUID >> 24) & 0x000000FF
        , (SCB->CPUID >> 20) & 0x0000000F
        , (SCB->CPUID >> 4) & 0x00000FFF
        , SCB->CPUID & 0x0000000F
        , ckcfg.SYSCLK
        , ckcfg.AHB
        , ckcfg.APB1
        , ckcfg.APB2
    );

    return 0;
}

static void cli_info_cmd_help(void) {
    printf(
        "\r\nPrint device info:\r\n\r\n"
        "- MCU UID\r\n"
        "- FLash size\r\n"
        "- Firmware version\r\n"
        "- Firmware release date\r\n"
        "- Hardware version\r\n"
        "- Hardware release date\r\n"
        );
}

static cli_node_t cli_info_cmd = {
    .c_name = "info",
    .brief = "print device info",
    .main_fn = cli_info_main,
    .help_fn = cli_info_cmd_help,
};

void cli_info_init(void) {
    cli_regcmd(&cli_info_cmd);
}
