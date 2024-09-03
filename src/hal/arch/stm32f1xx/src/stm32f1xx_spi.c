/**
 * @file    stm32f1xx_spi.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-08-22
 *
 * @brief
 */
#include "stm32f1xx_spi.h"


/* Private functions --------------------------------------------- */

/* Poll mode */

static int hal_spi_sync_rx_transfer(void* ctx, const void* data, uint32_t size) {}
static int hal_spi_sync_tx_transfer(void* ctx, void* data, uint32_t size) {}
static int hal_spi_sync_start(void* ctx) {}
static int hal_spi_sync_poll(void* ctx) {}

static int hal_spi_sync_init(hal_spi_t* context) {}

/* Interrupt mode */

static void hal_spi_irq_handler(void* ctx) {
    HAL_ASSERT_OBJECT(hal_spi_t, context, ctx)
    uint32_t sr = context->handle->SR;

}

static int hal_spi_async_irq_rx_transfer(void* ctx, const void* data, uint32_t size) {}
static int hal_spi_async_irq_tx_transfer(void* ctx, void* data, uint32_t size) {}
static int hal_spi_async_irq_start(void* ctx) {}
static int hal_spi_async_irq_poll(void* ctx) {}

static int hal_spi_async_irq_init(hal_spi_t* context) {}

/* DMA mode */
static void hal_spi_async_dma_tc_callback(void* ctx) {
    HAL_ASSERT_OBJECT(hal_spi_t, context, ctx)
}

static int hal_spi_async_dma_rx_transfer(void* ctx, const void* data, uint32_t size) {}
static int hal_spi_async_dma_tx_transfer(void* ctx, void* data, uint32_t size) {}
static int hal_spi_async_dma_start(void* ctx) {}
static int hal_spi_async_dma_poll(void* ctx) {}

static int hal_spi_async_dma_init(hal_spi_t* context) {}

/* Configurations */

static int hal_spi_set_config(SPI_TypeDef* SPIx, hal_spi_cfg_t* config) {
    HAL_ASSERT_PTR_RET(SPIx, -1)
    HAL_ASSERT_PTR_RET(config, -1)

    if (HAL_SPI_IS_ENABLED(SPIx)) {
        return 1;
    }

    if (config->data_width <= 8) {
        HAL_SPI_SET_DFF(SPIx, 0);
    } else {
        HAL_SPI_SET_DFF(SPIx, 1);
    }

    HAL_SPI_SET_LSBFIRST(SPIx, config->lsb_first);
    HAL_SPI_SET_CPOL(SPIx, config->cpol);
    HAL_SPI_SET_CPHA(SPIx, config->cpha);
    HAL_SPI_SET_BAUD(SPIx, config->baud_div);

    if ((config->tx_en == HAL_DISABLED) && (config->rx_en == HAL_ENABLED)) {
        HAL_SPI_SET_RXONLY(SPIx, 1);
    } else {
        HAL_SPI_SET_RXONLY(SPIx, 0);
    }

    return 0;
}

static int hal_spi_dma_init(hal_spi_t* context, hal_spi_cfg_t* config) {
    if (config->rx_en == HAL_ENABLED) {
        context->rx_dma = DMA_GetChannelByPeriph(context->handle, HAL_DMA_SPI_RX_ID);

        DMA_Config_t cfg = {
            .direction = HAL_DMA_PERIPH_TO_MEM,
            .dataWidth = (config->data_width <= 8) ? HAL_DMA_8_BITS : HAL_DMA_16_BITS,
            .priority = HAL_DMA_PRIRITY_VERY_HIGH,
            .memoryInc = HAL_ENABLED,
            .periphInc = HAL_DISABLED,
            .circMode = HAL_DISABLED,
            .periphAddr = (uint32_t)(&context->handle->DR),
        };

        if (DMA_Initialize(context->rx_dma, &cfg) < 0) {
            return -1;
        }

        DMA_Set_TC_Callback(context->rx_dma, hal_spi_async_dma_tc_callback);
        DMA_SetObject(context->rx_dma, context);
        HAL_SPI_RX_DMA_ENABLE(context->handle);
    }

    if (config->tx_en == HAL_ENABLED) {
        context->rx_dma = DMA_GetChannelByPeriph(context->handle, HAL_DMA_SPI_RX_ID);

        DMA_Config_t cfg = {
            .direction = HAL_DMA_MEM_TO_PERIPH,
            .dataWidth = (config->data_width <= 8) ? HAL_DMA_8_BITS : HAL_DMA_16_BITS,
            .priority = HAL_DMA_PRIRITY_VERY_HIGH,
            .memoryInc = HAL_ENABLED,
            .periphInc = HAL_DISABLED,
            .circMode = HAL_DISABLED,
            .periphAddr = (uint32_t)(&context->handle->DR),
        };

        if (DMA_Initialize(context->tx_dma, &cfg) < 0) {
            if (context->rx_dma != NULL) {
                DMA_ReturnChannel(context->rx_dma);
                context->rx_dma = NULL;
            }
            return -1;
        }

        if (config->rx_en == HAL_DISABLED) {
            DMA_Set_TC_Callback(context->tx_dma, hal_spi_async_dma_tc_callback);
        }

        HAL_SPI_TX_DMA_ENABLE(context->handle);
    }
    return 0;
}

/* Public functions ---------------------------------------------- */

void hal_spi_set_tc_clbk(hal_object_t* ctx, hal_callback_t clbk, void* obj) {
    if (ctx != NULL) {
        hal_spi_t* context = (hal_spi_t*)ctx;
        context->tc_clbk = clbk;
        context->tc_ctx = obj;
    }
}

int hal_spi_open(hal_object_t* ctx, hal_spi_cfg_t* config) {
    int ret = 0;

    HAL_ASSERT_OBJECT_RET(hal_spi_t, context, ctx, -1)

    if (context->enableClock != NULL) {
        context->enableClock(1);
    }


    if (HAL_IS_BIT_SET(context->flags, HAL_FLAG_INIT)) {
        return 1;
    } else if ((ret = hal_spi_set_config(context->handle, config)) != 0) {
        return ret;
    }

    if (config->tx_en == HAL_ENABLED) {
        HAL_SET_BITS(context->flags, HAL_SPI_TXEN_FLAG);
    }

    if (config->rx_en == HAL_ENABLED) {
        HAL_SET_BITS(context->flags, HAL_SPI_RXEN_FLAG);
    }

    return 0;
}

int hal_spi_write(hal_object_t* ctx, const void* data, uint32_t size) {
    return 0;
}

int hal_spi_read(hal_object_t* ctx, void* data, uint32_t size) {
    return 0;
}

int hal_spi_transfer(hal_object_t* ctx, const void* data, uint32_t size,
    void* rx_data, uint32_t rx_size, int* const rx_ret) {
    return 0;
}

void hal_spi_poll(hal_object_t* ctx) {

}


/* Implementation section */

#ifdef SPI1
hal_spi_t spi1_instance;
#endif /* SPI1 */

#ifdef SPI2
hal_spi_t spi2_instance;
#endif /* SPI2 */

void SPI1_IRQHandler(void) {

}

void SPI2_IRQHandler(void) {

}


hal_object_t hal_spi_get_instance(int id) {
    return NULL;
}