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
#include "version.h"
#include "stm32f1xx_rcc.h"
#include <stdio.h>

/*
17052     208    3872   21132
17012     240    3840   21092
16972     272    3808   21052
16952     284    3792   21028
16952     284    3792   21028
*/

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
    cli_append_command(&cli_info_cmd);
}


// static int cli_help_cmd_iter_fn(const char* key, void* object) {
//     cli_node_t* context = (cli_node_t*)object;
//     if ((NULL != context) && (NULL != context->c_name) && (NULL != context->brief)) {
//         printf("%s : %s\r\n", context->c_name, context->brief);
//     }
//     return 0;
// }

// static int cli_help_cmd_main(int argc, char** argv) {
//     if (argc < 2) {
//         printf("\r\n");
//         static_map_foreach(&cli_map, cli_help_cmd_iter_fn);
//         printf("\r\n");
//     } else {
//         cli_node_t* item = static_map_get(&cli_map, *(argv+1));
//         if ((NULL != item) && (NULL != item->help_fn)) {
//             item->help_fn();
//         }
//     }
//     return 0;
// }

// static uint32_t cli_info_uid[3];

// /**
//   * @brief  Get Word0 of the unique device identifier (UID based on 96 bits)
//   * @retval UID[31:0]
//   */
// __STATIC_INLINE uint32_t LL_GetUID_Word0(void)
// {
//   return (uint32_t)(READ_REG(*((uint32_t *)UID_BASE_ADDRESS)));
// }

// /**
//   * @brief  Get Word1 of the unique device identifier (UID based on 96 bits)
//   * @retval UID[63:32]
//   */
// __STATIC_INLINE uint32_t LL_GetUID_Word1(void)
// {
//   return (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE_ADDRESS + 4U))));
// }

// /**
//   * @brief  Get Word2 of the unique device identifier (UID based on 96 bits)
//   * @retval UID[95:64]
//   */
// __STATIC_INLINE uint32_t LL_GetUID_Word2(void)
// {
//   return (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE_ADDRESS + 8U))));
// }

// UID_BASE
/**
 * @brief Unique device ID register base address
 */
// #define UID_BASE_ADDRESS              UID_BASE
