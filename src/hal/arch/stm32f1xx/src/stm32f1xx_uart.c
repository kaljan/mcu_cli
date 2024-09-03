/**
 * @file    stm32f1xx_uart.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-03-23
 *
 * @brief
 *
 */
#include "stm32f1xx_uart.h"

#define UART_TX_FIFO_FLAG   0x10

void UART_IRQHandler(UART_t * context) {
    if ((NULL == context) || (NULL == context->handle)) {
        return;
    }

    USART_TypeDef* handle = context->handle;

    if (HAL_IS_BIT_SET(handle->CR1, USART_CR1_RXNEIE) &&
        HAL_IS_BIT_SET(handle->SR, USART_SR_RXNE)) {
        tools_fifo_put(&context->rxFIFO, (uint8_t)handle->DR);
    }

    if (HAL_IS_BIT_SET(handle->CR1, USART_CR1_TXEIE) &&
        HAL_IS_BIT_SET(handle->SR, USART_SR_TXE)) {
        uint8_t data;
        if (tools_fifo_get(&context->txFIFO, &data) < 0) {
            HAL_CLEAR_BITS(handle->CR1, USART_CR1_TXEIE);
            HAL_SET_BITS(handle->CR1, USART_CR1_TCIE);
        } else {
            handle->DR = data;
        }
    }

    if (HAL_IS_BIT_SET(handle->CR1, USART_CR1_TCIE) &&
        HAL_IS_BIT_SET(handle->SR, USART_SR_TC)) {
        handle->SR = ~(USART_SR_TC);
        HAL_CLEAR_BITS(handle->CR1, USART_CR1_TCIE);
        context->flags &= ~UART_TX_FIFO_FLAG;
        uint8_t data;
        if (tools_fifo_get(&context->txFIFO, &data) == 0) {
            HAL_SET_BITS(handle->CR1, USART_CR1_TCIE);
            context->flags |= UART_TX_FIFO_FLAG;
            handle->DR = data;
        }
    }

    if (HAL_IS_BIT_SET(handle->SR, USART_SR_FE) ||
        HAL_IS_BIT_SET(handle->SR, USART_SR_NE) ||
        HAL_IS_BIT_SET(handle->SR, USART_SR_ORE)) {
        volatile uint32_t tmp;
        tmp = handle->SR;
        __DSB();
        tmp = handle->DR;
        __DSB();
    }
}

static inline uint32_t UART_CalcBaud(uint32_t baud, uint32_t pclk) {
    uint32_t ret = 0, m, f;
    if (baud > 0 && pclk > 0) {
        ret = (pclk * 25) / (baud * 4);
        m = ret / 100;
        f = (((ret - (m * 100)) * 16) + 50) / 100;
        ret = ((m << 4) + (f & 0xF0)) + (f & 0x0F);
    }

    return ret;
}

static void UART_Configure(USART_TypeDef * handle, uint32_t baudrate) {
    uint32_t pclk = hal_rcc_get_periph_clock((uint32_t)handle);

    /* USART Config. TODO implement */
    /* Set 8 bit data width */
    HAL_CLEAR_BITS(handle->CR1, USART_CR1_M);

    /* Set 1 stop bit */
    HAL_MODIFY_FIELD(handle->CR2, 0, 0x03, USART_CR2_STOP_Pos);

    /* Disable flow control */
    HAL_CLEAR_BITS(handle->CR3, USART_CR3_RTSE | USART_CR3_CTSE);

    /* Disable parity */
    HAL_CLEAR_BITS(handle->CR1, USART_CR1_PS | USART_CR1_PCE);

    /* Configure baudrate */
    handle->BRR = UART_CalcBaud(baudrate, pclk);

    /* Set transfer direction */
    HAL_SET_BITS(handle->CR1, USART_CR1_RE | USART_CR1_TE);

    /* Enable */
    HAL_SET_BITS(handle->CR1, USART_CR1_UE);

    while (HAL_IS_BIT_CLEAR(handle->SR, USART_SR_TXE));
    while (HAL_IS_BIT_CLEAR(handle->SR, USART_SR_TC));

    HAL_SET_BITS(handle->CR1, USART_CR1_TXEIE);
    HAL_SET_BITS(handle->CR1, USART_CR1_RXNEIE);
    HAL_SET_BITS(handle->CR3, USART_CR3_EIE);
}

int hal_uart_open(hal_object_t ctx, hal_ptr_t cfg) {
    if ((NULL == ctx) || (NULL == cfg)) {
        return -1;
    }

    UART_t* context = (UART_t*)ctx;
    uint32_t baudrate = *((uint32_t *)cfg);

    if (HAL_IS_BIT_SET(context->flags, HAL_FLAG_INIT)) {
        return 0;
    }

    context->flags = HAL_FLAG_INIT;

    if (NULL != context->GPIO_Config) {
        context->GPIO_Config(1);
    }

    if (context->enableClock) {
        context->enableClock(1);
    }

    UART_Configure(context->handle, baudrate);

    NVIC_EnableIRQ(context->irqn);
    NVIC_SetPriority(context->irqn, 5);

    return 0;
}

void hal_uart_close(hal_object_t ctx) {
    if (NULL == ctx) {
        return;
    }

    UART_t* context = (UART_t*)ctx;
    if (HAL_IS_BIT_CLEAR(context->flags, HAL_FLAG_INIT)) {
        return;
    }

    context->flags = 0;
    NVIC_DisableIRQ(context->irqn);

    if (NULL != context->GPIO_Config) {
        context->GPIO_Config(0);
    }

    if (context->enableClock) {
        context->enableClock(0);
    }
}

int hal_uart_put(hal_object_t ctx, uint8_t ch) {
    int ret = -1;
    UART_t* context = (UART_t*)ctx;
    if (NULL == context) {
        return ret;
    }
    NVIC_DisableIRQ(context->irqn);
    ret = tools_fifo_put(&context->txFIFO, ch);

    if (HAL_IS_BIT_CLEAR(context->flags, UART_TX_FIFO_FLAG)) {
        uint8_t c;
        if (tools_fifo_get(&context->txFIFO, &c) == 0) {
            context->flags |= UART_TX_FIFO_FLAG;
            context->handle->DR = c;
            HAL_SET_BITS(context->handle->CR1, USART_CR1_TXEIE);
        }
    }
    NVIC_EnableIRQ(context->irqn);
    return ret;
}

int hal_uart_get(hal_object_t ctx, uint8_t* c) {
    int ret = -1;
    UART_t* context = (UART_t*)ctx;
    if (NULL == context) {
        return ret;
    }
    NVIC_DisableIRQ(context->irqn);
    ret = tools_fifo_get(&context->rxFIFO, c);
    NVIC_EnableIRQ(context->irqn);
    return ret;
}

#define UART2_TX_PORT   GPIOA
#define UART2_TX_PIN    2

#define UART2_RX_PORT   GPIOA
#define UART2_RX_PIN    3

static void UART2_EnableGPIO(int enable) {
    if (enable == 0) {

    } else {
        hal_gpio_pin_config_t cfg = {
            .mode = GPIO_PIN_MODE_ALTERNATE,
            .speed = GPIO_PIN_SPEED_HIGH,
            .outType = GPIO_PIN_OTYPE_PUSH_PULL,
            .pull = GPIO_PIN_PULL_UP,
        };

        hal_gpio_pin_set_config(UART2_TX_PORT, UART2_TX_PIN, &cfg);

        cfg.mode = GPIO_PIN_MODE_INPUT;
        hal_gpio_pin_set_config(UART2_RX_PORT, UART2_RX_PIN, &cfg);
    }
}

#define UART2_RX_FIFO_SIZE  128
#define UART2_TX_FIFO_SZIE  1536

static uint8_t uart2_rx_fifo_area[UART2_RX_FIFO_SIZE];
static uint8_t uart2_tx_fifo_area[UART2_TX_FIFO_SZIE];

static UART_t uart2_instance = {
    .flags = 0,
    .handle = USART2,
    .irqn = USART2_IRQn,
    .rxFIFO = {
        .data = uart2_rx_fifo_area,
        .size = UART2_RX_FIFO_SIZE,
        .bytes = 0,
        .head = 0,
        .tail = 0,
    },
    .txFIFO = {
        .data = uart2_tx_fifo_area,
        .size = UART2_TX_FIFO_SZIE,
        .bytes = 0,
        .head = 0,
        .tail = 0,
    },
    .enableClock = hal_rcc_usart2_enable_clock,
    .GPIO_Config = UART2_EnableGPIO,
};

void USART2_IRQHandler(void) {
    UART_IRQHandler(&uart2_instance);
}

hal_object_t hal_uart_get_handle(int id) {
    return &uart2_instance;
}





#if 0
int UART_Write(context_t ctx, const void* data, size_t size) {
    ASSERT_RET(data, -1)

    int ret = 0;
    while (0 != (size--)) {
        if (p_UART_Put((UART_t *)ctx, *((const uint8_t*)data + ret)) < 0) {
            return ret;
        }
        ret++;
    }
    return ret;
}

int UART_Read(context_t ctx, void* data, size_t size) {
    ASSERT_RET(data, -1)

    int ret = 0;
    while (0 != (size--)) {
        if (p_UART_Get((UART_t *)ctx, data + ret) < 0) {
            return ret;
        }
        ret++;
    }
    return ret;
}
#endif


	// Mantissa:				37
	// Fraction:				15
	// BRR Value:				37.9375
	// Calculated Baud Rate:	115321
	// Baud Rate:				115200
	// Error: 0.105254 %
	//USART1->BRR = (unsigned short)0x025F;

	// 1. div: 39.0625
	// -------------------------------
	// Real div: 39.0625
	// Mantissa: 39
	// Fraction: 0.0625
	// -------------------------------
	// Mantissa: 39
	// Fraction: 1
	// fr:       39.0625


/*
BAUD = FCK / (16 * UDIV)

UDIV = FCK / 16 * BAUD


#define UART_DIV_SMP16_100(pclk, baud)  (((pclk) * 25) / (4 * (baud)))
#define UART_DIVM_SMP16(pclk, baud)     (UART_DIV_SMP16_100((pclk), (baud)) / 100)
#define UART_DIVF_SMP16(pclk, baud) \
    ((((UART_DIV_SMP16_100((pclk), (baud)) - (UART_DIVM_SMP16((pclk), (baud)) * 100)) * 16) + 50) / 100)

#define __LL_USART_DIV_SAMPLING16(pclk, baud) \
    (((UART_DIVM_SMP16((pclk), (baud)) << 4) + \
      (UART_DIVF_SMP16((pclk), (baud)) & 0xF0)) + \
      (UART_DIVF_SMP16((pclk), (baud)) & 0x0F))
*/
