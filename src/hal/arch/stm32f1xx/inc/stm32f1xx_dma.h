/**
 * @file    stm32f1xx_dma.h
 * @author  kaljan
 * @version 0.0.0
 * @date    Oct 30, 2020
 *
 * @brief
 */

#ifndef STM32F1XX_DMA_H
#define STM32F1XX_DMA_H

#include "hal_types.h"
#include "hal_defs.h"

#define HAL_DMA1_ID                 0U

#define HAL_DMA_CHANNEL1_ID         0U
#define HAL_DMA_CHANNEL2_ID         1U
#define HAL_DMA_CHANNEL3_ID         2U
#define HAL_DMA_CHANNEL4_ID         3U
#define HAL_DMA_CHANNEL5_ID         4U
#define HAL_DMA_CHANNEL6_ID         5U
#define HAL_DMA_CHANNEL7_ID         6U

#define HAL_DMA_CHANNEL_MAX 7

#define HAL_DMA_TRAMSFER_ERROR      1U
#define HAL_DMA_ERROR               2U
#define HAL_DMA_FIFO_ERROR          3U

#define HAL_DMA_PERIPH_TO_MEM       0U
#define HAL_DMA_MEM_TO_PERIPH       1U

#define HAL_DMA_8_BITS              0U
#define HAL_DMA_16_BITS             1U
#define HAL_DMA_32_BITS             2U

#define HAL_DMA_PRIRITY_LOW         0U
#define HAL_DMA_PRIRITY_MEDIUM      1U
#define HAL_DMA_PRIRITY_HIGH        2U
#define HAL_DMA_PRIRITY_VERY_HIGH   3U

#define HAL_DMA_SPI_TX_ID           0U
#define HAL_DMA_SPI_RX_ID           1U

#define HAL_DMA_I2C_TX_ID           0U
#define HAL_DMA_I2C_RX_ID           1U

#define HAL_DMA_USART_TX_ID         0U
#define HAL_DMA_USART_RX_ID         1U

#define HAL_DMA_USART_TX_ID         0U
#define HAL_DMA_USART_RX_ID         1U

#define HAL_DMA_TIM_CH1_ID          0U
#define HAL_DMA_TIM_CH2_ID          1U
#define HAL_DMA_TIM_CH3_ID          2U
#define HAL_DMA_TIM_CH4_ID          3U
#define HAL_DMA_TIM_UP_ID           4U
#define HAL_DMA_TIM_TRIG_ID         5U
#define HAL_DMA_TIM_COM_ID          6U

typedef struct hal_dma_config {
    uint32_t direction;
    uint32_t dataWidth;
    uint32_t priority;
    uint32_t memoryInc;
    uint32_t periphInc;
    uint32_t circMode;
    uint32_t periphAddr;
} hal_dma_config_t;

struct hal_dma_channel;
typedef struct hal_dma_channel hal_dma_channel_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

hal_dma_channel_t* hal_dma_get_channel(uint32_t DMAx, uint32_t channelY);
hal_dma_channel_t* hal_dma_get_channel_by_periph(void* periph, uint32_t id);

void hal_dma_return_channel(hal_dma_channel_t* ptr);

void hal_dma_clear_irq(hal_dma_channel_t* context);
void hal_dma_enable_irq(hal_dma_channel_t* context);
void hal_dma_disable_irq(hal_dma_channel_t* context);
void hal_dma_enable_irq_ht(hal_dma_channel_t* context);
void hal_dma_disable_irq_ht(hal_dma_channel_t* context);
void hal_dma_enable_irq_tc(hal_dma_channel_t* context);
void hal_dma_disable_irq_tc(hal_dma_channel_t* context);
void hal_dma_enable_error_irq(hal_dma_channel_t* context);
void hal_dma_disable_error_irq(hal_dma_channel_t* context);
void hal_dma_transfer_enable(hal_dma_channel_t* context);
void hal_dma_transfer_disable(hal_dma_channel_t* context);
int hal_dma_transfer_enabled(hal_dma_channel_t* context);

void hal_dma_set_ht_callback(hal_dma_channel_t* context, hal_callback_t clbk);
void hal_dma_set_tc_callback(hal_dma_channel_t* context, hal_callback_t clbk);
void hal_dma_set_error_callback(hal_dma_channel_t* context, hal_on_failed_t clbk);
void hal_dma_set_object(hal_dma_channel_t* context, void* object);

void hal_dma_enable_clock(uint32_t DMAx);
void hal_dma_disable_clock(uint32_t DMAx);

int hal_dma_initialize(hal_dma_channel_t* context, hal_dma_config_t * config);
void hal_dma_deinitialize(hal_dma_channel_t* context);
int hal_dma_transfer(hal_dma_channel_t* context, uint32_t data, size_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_DMA_H */

