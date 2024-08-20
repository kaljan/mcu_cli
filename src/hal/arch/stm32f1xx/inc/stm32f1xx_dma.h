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

#define DMA_TRAMSFER_ERROR          1U
#define DMA_ERROR                   2U
#define DMA_FIFO_ERROR              3U

#define HAL_DMA_PERIPH_TO_MEM       0U
#define HAL_DMA_MEM_TO_PERIPH       1U

#define HAL_DMA_8_BITS              0U
#define HAL_DMA_16_BITS             1U
#define HAL_DMA_32_BITS             2U

#define HAL_DMA_PRIRITY_LOW         0U
#define HAL_DMA_PRIRITY_MEDIUM      1U
#define HAL_DMA_PRIRITY_HIGH        2U
#define HAL_DMA_PRIRITY_VERY_HIGH   3U

typedef struct DMA_Config {
    uint32_t direction;
    uint32_t dataWidth;
    uint32_t priority;
    uint32_t memoryInc;
    uint32_t periphInc;
    uint32_t circMode;
    uint32_t periphAddr;
} DMA_Config_t;

struct DMA_Channel;
typedef struct DMA_Channel DMA_Channel_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

DMA_Channel_t* Get_DMA_Channel(uint32_t DMAx, uint32_t channelY);
DMA_Channel_t* Get_DMA_ChannelByPeriph(uint32_t offset);

void DMA_ClearIRQ(DMA_Channel_t* context);
void DMA_EnableIRQ(DMA_Channel_t* context);
void DMA_DisableIRQ(DMA_Channel_t* context);
void DMA_EnableIRQ_HT(DMA_Channel_t* context);
void DMA_DisableIRQ_HT(DMA_Channel_t* context);
void DMA_EnableIRQ_TC(DMA_Channel_t* context);
void DMA_DisableIRQ_TC(DMA_Channel_t* context);
void DMA_EnableErrorIRQ(DMA_Channel_t* context);
void DMA_DisableErrorIRQ(DMA_Channel_t* context);
void DMA_TransferEnable(DMA_Channel_t* context);
void DMA_TransferDisable(DMA_Channel_t* context);
int DMA_TransferEnabled(DMA_Channel_t* context);

void DMA_Set_HT_Callback(DMA_Channel_t* context, callback_t clbk);
void DMA_Set_TC_Callback(DMA_Channel_t* context, callback_t clbk);
void DMA_SetErrorCallback(DMA_Channel_t* context, on_failed_t clbk);
void DMA_SetObject(DMA_Channel_t* context, context_t object);

void DMA_EnableClock(uint32_t DMAx);
void DMA_DisableClock(uint32_t DMAx);

int DMA_Initialize(DMA_Channel_t* context, DMA_Config_t * config);
void DMA_Deinitialize(DMA_Channel_t* context);
int DMA_Transfer(DMA_Channel_t* context, uint32_t data, size_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_DMA_H */

