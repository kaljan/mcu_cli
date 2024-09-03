/**
 * @file    hal_spi.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-15
 *
 * @brief
 */
#ifndef HAL_SPI_H
#define HAL_SPI_H

#include "hal_config.h"

#define HAL_SPI_CPOL_0          0U
#define HAL_SPI_CPOL_1          1U

#define HAL_SPI_CPHA_1ST_EDGE   0U
#define HAL_SPI_CPHA_2ND_EDGE   1U

#define HAL_SPI_MSBFIRS         0U
#define HAL_SPI_LSBFIRS         1U

#define HAL_SPI_BAUD_DIV2       0U
#define HAL_SPI_BAUD_DIV4       1U
#define HAL_SPI_BAUD_DIV8       2U
#define HAL_SPI_BAUD_DIV16      3U
#define HAL_SPI_BAUD_DIV32      4U
#define HAL_SPI_BAUD_DIV64      5U
#define HAL_SPI_BAUD_DIV128     6U
#define HAL_SPI_BAUD_DIV256     7U

typedef struct hal_spi_cfg {
    /* Clock polarity */
    uint8_t cpol;

    /* Clock phase */
    uint8_t cpha;

    /* Frame format */
    uint8_t lsb_first;

    /* Receive enable */
    uint8_t rx_en;

    /* Transmitt enable */
    uint8_t tx_en;

    /* Baudrate divisor */
    uint8_t baud_div;

    /* Data width (bits) */
    uint8_t data_width;
} hal_spi_cfg_t;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

hal_object_t hal_spi_get_instance(int id);
void hal_spi_set_tc_clbk(hal_object_t* ctx, hal_callback_t clbk, void* obj);
int hal_spi_open(hal_object_t* ctx, hal_spi_cfg_t* config);
int hal_spi_write(hal_object_t* ctx, const void* data, uint32_t size);
int hal_spi_read(hal_object_t* ctx, void* data, uint32_t size);
int hal_spi_transfer(hal_object_t* ctx, const void* data, uint32_t size,
    void* rx_data, uint32_t rx_size, int* const rx_ret);
void hal_spi_poll(hal_object_t* ctx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAL_SPI_H */
