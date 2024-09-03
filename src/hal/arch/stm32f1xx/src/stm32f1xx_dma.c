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


struct hal_dma_channel {
    uint32_t flags;
    uint32_t channel;
    DMA_TypeDef* handle;
    DMA_Channel_TypeDef* h_channel;
    IRQn_Type irqn;
    callback_t ht_callback;
    callback_t tc_callback;
    on_failed_t te_callback;
    context_t object;
    uint32_t data_blob;
};


static inline bool hal_dma_is_active_flag_gi(DMA_TypeDef* handle, uint32_t channel) {
    return (handle->ISR & DMA_ISR_GIF1 << (((channel - 1) & 0x07UL) << 2));
}

static inline bool hal_dma_is_active_flag_ht(DMA_TypeDef* handle, uint32_t channel) {
    return (handle->ISR & DMA_ISR_HTIF1 << (((channel - 1) & 0x07UL) << 2));
}

static inline bool hal_dma_is_active_flag_tc(DMA_TypeDef* handle, uint32_t channel) {
    return (handle->ISR & DMA_ISR_TCIF1 << (((channel - 1) & 0x07UL) << 2));
}

static inline bool hal_dma_is_active_flag_te(DMA_TypeDef* handle, uint32_t channel) {
    return (handle->ISR & DMA_ISR_TEIF1 << (((channel - 1) & 0x07UL) << 2));
}

static inline void hal_dma_clear_flag_gi(DMA_TypeDef* handle, uint32_t channel) {
    handle->IFCR = DMA_IFCR_CGIF1 << (((channel - 1) & 0x07UL) << 2);
}

static inline void hal_dma_clear_flag_ht(DMA_TypeDef* handle, uint32_t channel) {
    handle->IFCR = DMA_IFCR_CHTIF1 << (((channel - 1) & 0x07UL) << 2);
}

static inline void hal_dma_clear_flag_tc(DMA_TypeDef* handle, uint32_t channel) {
    handle->IFCR = DMA_IFCR_CTCIF1 << (((channel - 1) & 0x07UL) << 2);
}

static inline void hal_dma_clear_flag_te(DMA_TypeDef* handle, uint32_t channel) {
    handle->IFCR = DMA_IFCR_CTEIF1 << (((channel - 1) & 0x07UL) << 2);
}


static inline int hal_dma_is_enabled_irq_ht(DMA_Channel_TypeDef* context) {
    if (context != NULL) {
        return (HAL_IS_BIT_SET(context->CCR, DMA_CCR_HTIE)) ? 0 : 1;
    }
    return -1;
}

static inline int hal_dma_is_enabled_irq_tc(DMA_Channel_TypeDef* context) {
    if (context != NULL) {
        return (HAL_IS_BIT_SET(context->CCR, DMA_CCR_TCIE)) ? 0 : 1;
    }
    return -1;
}

static inline int hal_dma_is_enabled_irq_te(DMA_Channel_TypeDef* context) {
    if (context != NULL) {
        return (HAL_IS_BIT_SET(context->CCR, DMA_CCR_TEIE)) ? 0 : 1;
    }
    return -1;
}



void hal_dma_irq_handler(context_t ctx) {
    HAL_ASSERT_PTR(ctx)
    HAL_CONEXT(hal_dma_channel_t, ctx);

    if (hal_dma_is_active_flag_ht(context->handle, context->channel)) {
        hal_dma_clear_flag_ht(context->handle, context->channel);
        if ((hal_dma_is_enabled_irq_ht(context->h_channel) == 0)
            && (context->ht_callback != NULL)) {
            context->ht_callback(context->object);
        }
    }

    if (hal_dma_is_active_flag_tc(context->handle, context->channel)) {
        hal_dma_clear_flag_tc(context->handle, context->channel);
        if ((hal_dma_is_enabled_irq_tc(context->h_channel) == 0)
            && (context->tc_callback != NULL)) {
            context->tc_callback(context->object);
        }
    }

    if (hal_dma_is_active_flag_te(context->handle, context->channel)) {
        hal_dma_clear_flag_te(context->handle, context->channel);
        if ((hal_dma_is_enabled_irq_te(context->h_channel) == 0)
            && (context->te_callback != NULL)) {
            context->te_callback(context->object, -1);
        }
    }
}


void hal_dma_enable_clock(uint32_t DMAx) {
    if (DMAx == DMA1_BASE) {
        RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    }
}

void hal_dma_disable_clock(uint32_t dmaOffset) {
    if (dmaOffset == DMA1_BASE) {
        RCC->AHBENR &= ~RCC_AHBENR_DMA1EN;
    }
}

void hal_dma_clear_irq(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    DMA_ClearFlag_GI(context->handle, context->channel);
}

void hal_dma_enable_irq(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    NVIC_EnableIRQ(context->irqn);
}

void hal_dma_disable_irq(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    NVIC_DisableIRQ(context->irqn);
}

void hal_dma_enable_irq_ht(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_HTIE);
}

void hal_dma_disable_irq_ht(hal_dma_channel_t* context){
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_HTIE);
}

void hal_dma_enable_irq_tc(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_TCIE);
}

void hal_dma_disable_irq_tc(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_TCIE);
}

void hal_dma_enable_error_irq(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_TEIE);
}

void hal_dma_disable_error_irq(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_TEIE);
}

void hal_dma_transfer_enable(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_SET_BITS(context->h_channel->CCR, DMA_CCR_EN);
}

void hal_dma_transfer_disable(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR(context);
    HAL_ASSERT_PTR(context->h_channel);
    HAL_CLEAR_BITS(context->h_channel->CCR, DMA_CCR_EN);
}

int hal_dma_transfer_enabled(hal_dma_channel_t* context) {
    HAL_ASSERT_PTR_RET(context, -1);
    HAL_ASSERT_PTR_RET(context->h_channel, -1);
    return HAL_IS_BIT_SET(context->h_channel->CCR, DMA_CCR_EN) ? 1 : 0;
}

void hal_dma_set_ht_callback(hal_dma_channel_t *context, hal_callback_t clbk) {
    HAL_ASSERT_PTR(context);
    context->ht_callback = clbk;
}

void hal_dma_set_tc_callback(hal_dma_channel_t *context, hal_callback_t clbk) {
    HAL_ASSERT_PTR(context);
    context->tc_callback = clbk;
}

void hal_dma_set_error_callback(hal_dma_channel_t *context, hal_on_failed_t clbk) {
    HAL_ASSERT_PTR(context);
    context->te_callback = clbk;
}

void hal_dma_set_object(hal_dma_channel_t *context, void* object) {
    HAL_ASSERT_PTR(context);
    context->object = object;
}


int hal_dma_initialize(hal_dma_channel_t* context, hal_dma_config_t* config) {
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

void hal_dma_deinitialize(hal_dma_channel_t* context) {
    NVIC_DisableIRQ(context->irqn);
    int ret = hal_dma_transfer_enabled(context);
    if (ret == 1) {
        hal_dma_transfer_disable(context);
    } else if (ret == -1) {
        return;
    }

    hal_dma_clear_irq(context);
    hal_dma_disable_irq_ht(context);
    hal_dma_disable_irq_tc(context);
    hal_dma_disable_errorirq(context);
    hal_dma_disable_irq(context);
}

int hal_dma_transfer(hal_dma_channel_t* context, uint32_t data, size_t size) {
    int ret = hal_dma_transfer_enabled(context);
    if (ret == 0) {
        if (data == 0) {
            context->h_channel->CMAR = (uint32_t)(&context->data_blob);
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

static hal_dma_channel_t DMA1_ChannelList[HAL_DMA_CHANNEL_MAX] = {
    {0, 0, DMA1, DMA1_Channel1, DMA1_Channel1_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 1, DMA1, DMA1_Channel2, DMA1_Channel2_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 2, DMA1, DMA1_Channel3, DMA1_Channel3_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 3, DMA1, DMA1_Channel4, DMA1_Channel4_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 4, DMA1, DMA1_Channel5, DMA1_Channel5_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 5, DMA1, DMA1_Channel6, DMA1_Channel6_IRQn, NULL, NULL, NULL, NULL, 0,},
    {0, 6, DMA1, DMA1_Channel7, DMA1_Channel7_IRQn, NULL, NULL, NULL, NULL, 0,},
};

static hal_dma_channel_t* DMA1_ChannelPtrList[HAL_DMA_CHANNEL_MAX] = {
    &DMA1_ChannelList[0], &DMA1_ChannelList[1], &DMA1_ChannelList[2], &DMA1_ChannelList[3],
    &DMA1_ChannelList[4], &DMA1_ChannelList[5], &DMA1_ChannelList[6]
};

hal_dma_channel_t* hal_dma_get_channel(uint32_t DMAx, uint32_t channelY) {
    if ((DMAx == 0) && (channelY < HAL_DMA_CHANNEL_MAX)) {
        hal_dma_channel_t* ptr = DMA1_ChannelPtrList[channelY];
        DMA1_ChannelPtrList[channelY] = NULL;
        return channelY;
    }
    return NULL;
}

/* DMA TABLE BEGIN */
/* SPI */
#define HAL_DMA_SPI1_TX_CH      HAL_DMA_CHANNEL3_ID
#define HAL_DMA_SPI1_RX_CH      HAL_DMA_CHANNEL2_ID

#define HAL_DMA_SPI2_TX_CH      HAL_DMA_CHANNEL5_ID
#define HAL_DMA_SPI2_RX_CH      HAL_DMA_CHANNEL4_ID

/* I2C */
#define HAL_DMA_I2C1_TX_CH      HAL_DMA_CHANNEL6_ID
#define HAL_DMA_I2C1_RX_CH      HAL_DMA_CHANNEL7_ID

#define HAL_DMA_I2C2_TX_CH      HAL_DMA_CHANNEL4_ID
#define HAL_DMA_I2C2_RX_CH      HAL_DMA_CHANNEL5_ID

/* USART */
#define HAL_DMA_USART1_TX_CH    HAL_DMA_CHANNEL4_ID
#define HAL_DMA_USART1_RX_CH    HAL_DMA_CHANNEL5_ID

#define HAL_DMA_USART2_TX_CH    HAL_DMA_CHANNEL6_ID
#define HAL_DMA_USART2_RX_CH    HAL_DMA_CHANNEL7_ID

#define HAL_DMA_USART3_TX_CH    HAL_DMA_CHANNEL2_ID
#define HAL_DMA_USART3_RX_CH    HAL_DMA_CHANNEL3_ID

/* TIM */
#define HAL_DMA_TIM1_CH1_CH     HAL_DMA_CHANNEL2_ID
#define HAL_DMA_TIM1_CH3_CH     HAL_DMA_CHANNEL6_ID
#define HAL_DMA_TIM1_CH4_CH     HAL_DMA_CHANNEL4_ID
#define HAL_DMA_TIM1_UP_CH      HAL_DMA_CHANNEL5_ID
#define HAL_DMA_TIM1_TRIG_CH    HAL_DMA_CHANNEL4_ID
#define HAL_DMA_TIM1_COM_CH     HAL_DMA_CHANNEL4_ID

#define HAL_DMA_TIM2_CH1_CH     HAL_DMA_CHANNEL5_ID
#define HAL_DMA_TIM2_CH2_CH     HAL_DMA_CHANNEL7_ID
#define HAL_DMA_TIM2_CH3_CH     HAL_DMA_CHANNEL1_ID
#define HAL_DMA_TIM2_CH4_CH     HAL_DMA_CHANNEL7_ID
#define HAL_DMA_TIM2_UP_CH      HAL_DMA_CHANNEL2_ID

#define HAL_DMA_TIM3_CH1_CH     HAL_DMA_CHANNEL6_ID
#define HAL_DMA_TIM3_CH3_CH     HAL_DMA_CHANNEL2_ID
#define HAL_DMA_TIM3_CH4_CH     HAL_DMA_CHANNEL3_ID
#define HAL_DMA_TIM3_UP_CH      HAL_DMA_CHANNEL3_ID
#define HAL_DMA_TIM3_TRIG_CH    HAL_DMA_CHANNEL6_ID

#define HAL_DMA_TIM4_CH1_CH     HAL_DMA_CHANNEL1_ID
#define HAL_DMA_TIM4_CH2_CH     HAL_DMA_CHANNEL4_ID
#define HAL_DMA_TIM4_CH3_CH     HAL_DMA_CHANNEL5_ID
#define HAL_DMA_TIM4_UP_CH      HAL_DMA_CHANNEL7_ID
/* DMA TABLE END */

hal_dma_channel_t *hal_dma_get_channel_by_periph(void *periph, uint32_t id)
{
    uint32_t index = 8;
    if (periph == SPI1) {
        if (id == HAL_DMA_SPI_TX_ID) {
            index = HAL_DMA_SPI1_TX_CH;
        } else {
            index = HAL_DMA_SPI1_RX_CH;
        }
    }
    return hal_dma_get_channel(HAL_DMA1_ID, index);
}

void hal_dma_return_channel(hal_dma_channel_t *instance) {
    if (instance == DMA1) {
        if (instance->channel < HAL_DMA_CHANNEL_MAX) {
            hal_dma_deinitialize(instance);
            DMA1_ChannelPtrList[instance->channel] = instance;
        }
    }
}

void DMA1_Channel1_IRQHandler(void) {
    hal_dma_irq_handler(&DMA1_ChannelList[HAL_DMA_CHANNEL1_ID]);
}

void DMA1_Channel2_IRQHandler(void) {
    hal_dma_irq_handler(&DMA1_ChannelList[HAL_DMA_CHANNEL2_ID]);
}

void DMA1_Channel3_IRQHandler(void) {
    hal_dma_irq_handler(&DMA1_ChannelList[HAL_DMA_CHANNEL3_ID]);
}

void DMA1_Channel4_IRQHandler(void) {
    hal_dma_irq_handler(&DMA1_ChannelList[HAL_DMA_CHANNEL4_ID]);
}

void DMA1_Channel5_IRQHandler(void) {
    hal_dma_irq_handler(&DMA1_ChannelList[HAL_DMA_CHANNEL5_ID]);
}

void DMA1_Channel6_IRQHandler(void) {
    hal_dma_irq_handler(&DMA1_ChannelList[HAL_DMA_CHANNEL6_ID]);
}

void DMA1_Channel7_IRQHandler(void) {
    hal_dma_irq_handler(&DMA1_ChannelList[HAL_DMA_CHANNEL7_ID]);
}
