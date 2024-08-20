/**
 * @file    periph_dma.c
 * @author  kaljan
 * @version 0.0.0
 * @date    Oct 30, 2020
 *
 * @brief
 */
#include "stm32f1xx_dma.h"
#include "stm32f1xx.h"
#include "tools_buffer.h"


#define HAL_DMA_MEM_INC_FLAG        0x10U


struct DMA_Channel {
    uint32_t flags;
    uint32_t channel;
    DMA_TypeDef* handle;
    DMA_Channel_TypeDef* h_channel;
    IRQn_Type irqn;
    callback_t halfTransferCpltClbk;
    callback_t transferCpltClbk;
    on_failed_t errorClbk;
    context_t object;
    uint32_t dataBlob;
};


static inline bool DMA_IsActiveFlag_GI(DMA_TypeDef* handle, uint32_t channel) {
    return (handle->ISR & DMA_ISR_GIF1 << (((channel - 1) & 0x07UL) << 2));
}

static inline bool DMA_IsActiveFlag_HT(DMA_TypeDef* handle, uint32_t channel) {
    return (handle->ISR & DMA_ISR_HTIF1 << (((channel - 1) & 0x07UL) << 2));
}

static inline bool DMA_IsActiveFlag_TC(DMA_TypeDef* handle, uint32_t channel) {
    return (handle->ISR & DMA_ISR_TCIF1 << (((channel - 1) & 0x07UL) << 2));
}

static inline bool DMA_IsActiveFlag_TE(DMA_TypeDef* handle, uint32_t channel) {
    return (handle->ISR & DMA_ISR_TEIF1 << (((channel - 1) & 0x07UL) << 2));
}

static inline void DMA_ClearFlag_GI(DMA_TypeDef* handle, uint32_t channel) {
    handle->IFCR = DMA_IFCR_CGIF1 << (((channel - 1) & 0x07UL) << 2);
}

static inline void DMA_ClearFlag_HT(DMA_TypeDef* handle, uint32_t channel) {
    handle->IFCR = DMA_IFCR_CHTIF1 << (((channel - 1) & 0x07UL) << 2);
}

static inline void DMA_ClearFlag_TC(DMA_TypeDef* handle, uint32_t channel) {
    handle->IFCR = DMA_IFCR_CTCIF1 << (((channel - 1) & 0x07UL) << 2);
}

static inline void DMA_ClearFlag_TE(DMA_TypeDef* handle, uint32_t channel) {
    handle->IFCR = DMA_IFCR_CTEIF1 << (((channel - 1) & 0x07UL) << 2);
}


void DMA_IRQ_Handler(context_t ctx) {
    HAL_ASSERT_PTR_RET(ctx)
    HAL_CONEXT(DMA_Channel_t, ctx);

    if (DMA_IsActiveFlag_HT(context->handle, context->channel)) {
        DMA_ClearFlag_HT(context->handle, context->channel);
        if (LL_DMA_IsEnabledIT_HT(context->handle, context->channel)
            && context->halfTransferCpltClbk) {
            context->halfTransferCpltClbk(context->object);
        }
    }

    if (DMA_IsActiveFlag_TC(context->handle, context->channel)) {
        DMA_ClearFlag_TC(context->handle, context->channel);
        if (LL_DMA_IsEnabledIT_TC(context->handle, context->channel)
            && context->transferCpltClbk) {
            context->transferCpltClbk(context->object);
        }
    }

    if (DMA_IsActiveFlag_TE(context->handle, context->channel)) {
        DMA_ClearFlag_TE(context->handle, context->channel);
        if (LL_DMA_IsEnabledIT_TE(context->handle, context->channel)
            && context->transferCpltClbk) {
            context->errorClbk(context->object, -1);
        }
    }
}


void DMA_EnableClock(uint32_t DMAx)
{
    if (DMAx == DMA1_BASE) {
        RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    }
}

void DMA_DisableClock(uint32_t dmaOffset) {
    if (dmaOffset == DMA1_BASE) {
        RCC->AHBENR &= ~RCC_AHBENR_DMA1EN;
    }
}

void DMA_ClearIRQ(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    DMA_ClearFlag_GI(context->handle, context->channel);
}

void DMA_EnableIRQ(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    NVIC_EnableIRQ(context->irqn);
}

void DMA_DisableIRQ(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    NVIC_DisableIRQ(context->irqn);
}

void DMA_EnableIRQ_HT(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_HTIE);
}

void DMA_DisableIRQ_HT(DMA_Channel_t* context){
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_HTIE);
}

void DMA_EnableIRQ_TC(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_TCIE);
}

void DMA_DisableIRQ_TC(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_TCIE);
}

void DMA_EnableErrorIRQ(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_TEIE);
}

void DMA_DisableErrorIRQ(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_TEIE);
}

void DMA_TransferEnable(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_EN);
}

void DMA_TransferDisable(DMA_Channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_EN);
}

int DMA_TransferEnabled(DMA_Channel_t* context) {
    HAL_ASSERT_PTR_RET(context, -1);
    HAL_ASSERT_PTR_RET(context->h_channel, -1);
    return HAL_IS_BIT_SET(context->h_channel->CCR, DMA_CCR_EN) ? 1 : 0;
}

void DMA_Set_HT_Callback(DMA_Channel_t *context, callback_t clbk) {
    HAL_ASSERT_PTR(context);
    context->halfTransferCpltClbk = clbk;
}

void DMA_Set_TC_Callback(DMA_Channel_t *context, callback_t clbk) {
    HAL_ASSERT_PTR(context);
    context->transferCpltClbk = clbk;
}

void DMA_SetErrorCallback(DMA_Channel_t *context, on_failed_t clbk) {
    HAL_ASSERT_PTR(context);
    context->errorClbk = clbk;
}

void DMA_SetObject(DMA_Channel_t *context, context_t object) {
    HAL_ASSERT_PTR(context);
    context->object = object;
}


int DMA_Initialize(DMA_Channel_t* context, DMA_Config_t* config) {
    HAL_ASSERT_PTR_RET(config, -1);
    int ret = DMA_TransferEnabled(context);
    if (ret != 0) {
        return ret;
    }

    uint32_t ccr_reg = 0;

    DMA_EnableClock((uint32_t)context->handle);

    if (config->memoryInc == HAL_DISABLED) {
        HAL_SET_BITS(context->flags, HAL_DMA_MEM_INC_FLAG);
        HAL_SET_BITS(ccr_reg, DMA_CCR_MINC);
    } else {
        HAL_CLEAR_BITS(context->flags, HAL_DMA_MEM_INC_FLAG);
    }

    HAL_SET_BITS(ccr_reg, config->periphInc << DMA_CCR_PINC_Pos);
    HAL_SET_BITS(ccr_reg, config->circMode << DMA_CCR_CIRC_Pos);
    HAL_SET_BITS(ccr_reg, config->direction << DMA_CCR_DIR);
    HAL_MODIFY_FIELD(ccr_reg, config->dataWidth, 3U, DMA_CCR_MSIZE_Pos);
    HAL_MODIFY_FIELD(ccr_reg, config->dataWidth, 3U, DMA_CCR_PSIZE_Pos);
    HAL_MODIFY_FIELD(ccr_reg, config->priority, 3U, DMA_CCR_PL_Pos);

    context->h_channel->CCR = ccr_reg;
    context->h_channel->CNDTR = 0;
    context->h_channel->CMAR = 0;
    context->h_channel->CPAR = config->periphAddr;

    return 0;
}

void DMA_Deinitialize(DMA_Channel_t* context) {
    NVIC_DisableIRQ(context->irqn);
    int ret = DMA_TransferEnabled(context);
    if (ret == 1) {
        DMA_TransferDisable(context);
    } else if (ret == -1) {
        return;
    }

    DMA_ClearIRQ(context);
    DMA_DisableIRQ_HT(context);
    DMA_DisableIRQ_TC(context);
    DMA_DisableErrorIRQ(context);
    DMA_DisableIRQ(context);
}

int DMA_Transfer(DMA_Channel_t* context, uint32_t data, size_t size) {
    int ret = DMA_TransferEnabled(context);
    if (ret == 0) {
        if (data == 0) {
            context->h_channel->CMAR = (uint32_t)(&context->dataBlob);
            HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_MINC);
        } else {
            context->h_channel->CMAR = data;
            if (HAL_IS_BIT_SET(context->flags, HAL_DMA_MEM_INC_FLAG)) {
                HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_MINC);
            } else {
                HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_MINC);
            }
        }

        context->h_channel->CNDTR = size;
    }
    return ret;
}

DMA_Channel_t DMA1_ChannelList[HAL_DMA_CHANNEL_MAX] = {
    {0, 0, DMA1, DMA1_Channel1, DMA1_Channel1_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 1, DMA1, DMA1_Channel2, DMA1_Channel2_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 2, DMA1, DMA1_Channel3, DMA1_Channel3_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 3, DMA1, DMA1_Channel4, DMA1_Channel4_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 4, DMA1, DMA1_Channel5, DMA1_Channel5_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 5, DMA1, DMA1_Channel6, DMA1_Channel6_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 6, DMA1, DMA1_Channel7, DMA1_Channel7_IRQn, NULL, NULL, NULL, NULL, 0,},
};

DMA_Channel_t* Get_DMA_Channel(uint32_t DMAx, uint32_t channelY) {
    if ((DMAx == 0) && (channelY < HAL_DMA_CHANNEL_MAX)) {
        return &DMA1_ChannelList[channelY];
    }
    return NULL;
}

DMA_Channel_t* Get_DMA_ChannelByPeriph(uint32_t offset) {
    return NULL;
}