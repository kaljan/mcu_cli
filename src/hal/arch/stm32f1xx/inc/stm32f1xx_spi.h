/**
 * @file    stm32f1xx_spi.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-08-22
 *
 * @brief
 */
#ifndef STM32F1XX_SPI_H
#define STM32F1XX_SPI_H


#include "hal_spi.h"
#include "stm32f1xx_rcc.h"
#include "stm32f1xx_dma.h"


/* Defines ------------------------------------------------------- */


/* Flags */

#define HAL_SPI_TXEN_FLAG   0x0010U
#define HAL_SPI_RXEN_FLAG   0x0020U
#define HAL_SPI_SYNC_FLAG   0x0100U


/* SPI Configurations */

/** Clock polarity */
#define HAL_SPI_SET_CPOL(SPIx, CPOL) \
    HAL_ENABLE_BIT(SPIx->CR1, SPI_CR1_CPOL, CPOL)

/* Clock phase */
#define HAL_SPI_SET_CPHA(SPIx, CPHA) \
    HAL_ENABLE_BIT(SPIx->CR1, SPI_CR1_CPHA, CPHA)

/* Data frame format (8 or 16 bit) */
#define HAL_SPI_SET_DFF(SPIx, DFF) \
    HAL_ENABLE_BIT(SPIx->CR1, SPI_CR1_DFF, DFF)

/* LSB first */
#define HAL_SPI_SET_LSBFIRST(SPIx, LSBFIRST) \
    HAL_ENABLE_BIT(SPIx->CR1, SPI_CR1_LSBFIRST, LSBFIRST)

/* Baudrate divisor */
#define HAL_SPI_SET_BAUD(SPIx, BAUD) \
    HAL_MODIFY_FIELD(SPIx->CR1, BAUD, 0x07U, SPI_CR1_BR_Pos)

/* Transfer direction */
#define HAL_SPI_SET_RXONLY(SPIx, RXO) \
    HAL_ENABLE_BIT(SPIx->CR1, SPI_CR1_RXONLY, RXO)


/* Enable/Disable SPI */
#define HAL_SPI_ENABLE(SPIx) \
    HAL_SET_BITS(SPIx->CR1, SPI_CR1_SPE)

#define HAL_SPI_DISABLE(SPIx) \
    HAL_CLEAR_BITS(SPIx->CR1, SPI_CR1_SPE)

#define HAL_SPI_IS_ENABLED(SPIx) \
    HAL_IS_BIT_SET(SPIx->CR1, SPI_CR1_SPE)


/* Enable/Disable TX DMA */
#define HAL_SPI_TX_DMA_ENABLE(SPIx) \
    HAL_SET_BITS(SPIx->CR2, SPI_CR2_TXDMAEN)

#define HAL_SPI_TX_DMA_DISABLE(SPIx) \
    HAL_CLEAR_BITS(SPIx->CR2, SPI_CR2_TXDMAEN)

#define HAL_SPI_TX_DMA_ENABLED(SPIx) \
    HAL_IS_BIT_SET(SPIx->CR2, SPI_CR2_TXDMAEN)


/* Enable/Disable RX DMA */
#define HAL_SPI_RX_DMA_ENABLE(SPIx) \
    HAL_SET_BITS(SPIx->CR2, SPI_CR2_RXDMAEN)

#define HAL_SPI_RX_DMA_DISABLE(SPIx) \
    HAL_CLEAR_BITS(SPIx->CR2, SPI_CR2_RXDMAEN)

#define HAL_SPI_RX_DMA_ENABLED(SPIx) \
    HAL_IS_BIT_SET(SPIx->CR2, SPI_CR2_RXDMAEN)


/* Interrupts */

/* Tx buffer empty (TXE)*/
#define HAL_SPI_IRQ_TXE_ENABLE() \
    HAL_SET_BITS(SPIx->CR2, SPI_CR2_TXEIE)

#define HAL_SPI_IRQ_TXE_DIABLE() \
    HAL_CLEAR_BITS(SPIx->CR2, SPI_CR2_TXEIE)

#define HAL_SPI_IRQ_TXE_ENABLED() \
    HAL_IS_BIT_SET(SPIx->CR2, SPI_CR2_TXEIE)


/* Rx buffer not empty (RXNE)*/
#define HAL_SPI_IRQ_RXNE_ENABLE() \
    HAL_SET_BITS(SPIx->CR2, SPI_CR2_RXNEIE)

#define HAL_SPI_IRQ_RXNE_DIABLE() \
    HAL_CLEAR_BITS(SPIx->CR2, SPI_CR2_RXNEIE)

#define HAL_SPI_IRQ_RXNE_ENABLED() \
    HAL_IS_BIT_SET(SPIx->CR2, SPI_CR2_RXNEIE)


/* Error (ERRIE)*/
#define HAL_SPI_IRQ_ERR_ENABLE() \
    HAL_SET_BITS(SPIx->CR2, SPI_CR2_ERRIE)

#define HAL_SPI_IRQ_ERR_DIABLE() \
    HAL_CLEAR_BITS(SPIx->CR2, SPI_CR2_ERRIE)

#define HAL_SPI_IRQ_ERR_ENABLED() \
    HAL_IS_BIT_SET(SPIx->CR2, SPI_CR2_ERRIE)


/* Types --------------------------------------------------------- */

/* SPI interface */
typedef struct hal_spi_if {
    void* handle;
    int (*rx_transfer)(void*, const void*, uint32_t);
    int (*tx_transfer)(void*, void*, uint32_t);
    int (*start)(void*);
    int (*poll)(void*);
} hal_spi_if_t;

typedef struct hal_spi {
    /* Flags */
    uint32_t flags;

    /* Pointer to SPI registers structure */
    SPI_TypeDef* handle;

    /* Interrupt number */
    IRQn_Type irqn;

    /* Pointer to enable clock function */
    hal_rcc_cken_func_t enableClock;

    /* Pointer to Rx DMA channel */
    hal_dma_channel_t* tx_dma;

    /* Pointer to Tx DMA channel */
    hal_dma_channel_t* rx_dma;

    /* Pointer to data for tc_clbk */
    void* tc_ctx;

    /* Transfer complete callback */
    hal_callback_t tc_clbk;

    /* Tx buffer for interrupt modes */
    const void* tx_buf;

    /* items in Tx buffer */
    uint16_t tx_buf_count;

    /* Rx buffer for interrupt modes */
    const void* tx_buf;

    /* items in Tx buffer */
    uint16_t tx_buf_count;

    /* SPI mode interface. Sync, Async_IRQ, Async_DMA */
    hal_spi_if_t spi_mode_if;
} hal_spi_t;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_SPI_H */
