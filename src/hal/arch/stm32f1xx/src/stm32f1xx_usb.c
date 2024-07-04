/**
 * @file    stm32f1xx_usb.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-28
 *
 * @brief
 */
#include "hal_usb.h"
#include "hal_usbd.h"
#include "stm32f1xx_usb.h"
#include "stm32f1xx_config.h"
#include "stm32f1xx_rcc.h"
#include "stm32f1xx_gpio.h"


#define HAL_PMA_ACCESS          2U

/* Pointer to USB device registers */
#define HAL_USB                 ((hal_usb_t*)USB_BASE)

/* Pointer to USB buffers table */
#define HAL_USB_BTABLE          ((hal_usb_buftbl_t*)USB_PMAADDR)

/* Pointer to USB sram */
#define HAL_USB_SRAM            ((uint16_t*)USB_PMAADDR)

/*  */
#define HAL_BTABLE_OFFSET       64U


/* Endpoint config */
#define HAL_USB_EP_BULK         0U
#define HAL_USB_EP_CONTROL      1U
#define HAL_USB_EP_ISO          2U
#define HAL_USB_EP_INTERRUPT    3U

typedef struct ep_cfg {
    uint8_t ep_type;
    uint16_t buf_len;
} ep_cfg_t;
/* Endpoint config */


/* Public functions */

void hal_usb_init(void) {

}

void hal_usb_enumerate(uint8_t addr) {
    HAL_USB->DADDR = (HAL_USB->DADDR & 0xFF80U) | (addr & 0x007FU);
}


void hal_usb_write_pma(uint8_t ep_n, const uint8_t* data, uint16_t size) {
    uint16_t n = (size + 1U) >> 1;
    volatile uint16_t* dst_ptr = HAL_USB_SRAM + (HAL_USB_BTABLE->ep[ep_n].ADDR_TX * HAL_PMA_ACCESS);
    const uint16_t* src_ptr = (const uint16_t*)data;

    while (n != 0) {
        dst_ptr = *src_ptr;
        dst_ptr += HAL_PMA_ACCESS;
        ++src_ptr;
        --n;
    }
}

void hal_usb_read_pma(uint8_t ep_n, uint8_t* buf, uint16_t size) {
    uint16_t n = (size + 1U) >> 1;
    volatile uint16_t* src_ptr = HAL_USB_SRAM + (HAL_USB_BTABLE->ep[ep_n].ADDR_RX * HAL_PMA_ACCESS);
    uint16_t* dst_ptr = (const uint16_t*)buf;

    while (n != 0) {
        dst_ptr = *src_ptr;
        src_ptr += HAL_PMA_ACCESS;
        ++dst_ptr;
        --n;
    }
}

/*
| ATT | REG | VAL |
|  0  |  0  |  0  |
|  0  |  1  |  1  |
|  1  |  0  |  1  |
|  1  |  1  |  0  |
*/

static void hal_usb_ep_reg_write(uint8_t ep, uint16_t value, uint16_t mask, uint16_t offset) {
    uint16_t ep_reg_val = HAL_USB->ep[ep & 0x0F].EPnR &
        (HAL_USB_EPnR_EA | HAL_USB_EPnR_EP_KIND | HAL_USB_EPnR_EPTYPE);
    ep_reg_val = (ep_reg_val & (mask << offset)) | ((value & mask) << offset);
    HAL_USB->ep[ep & 0x0F].EPnR = ep_reg_val | (HAL_USB_EPnR_CTR_RX | HAL_USB_EPnR_CTR_TX);
}

static void hal_usb_ep_reg_toggle(uint8_t ep, uint16_t value, uint16_t mask, uint16_t offset) {
    uint16_t ep_reg_val = HAL_USB->ep[ep & 0x0F].EPnR &
        (HAL_USB_EPnR_EA | HAL_USB_EPnR_EP_KIND | HAL_USB_EPnR_EPTYPE | (mask << offset));
    ep_reg_val = (ep_reg_val & (mask << offset)) | (ep_reg_val ^ ((value & mask) << offset));
    HAL_USB->ep[ep & 0x0F].EPnR = ep_reg_val | (HAL_USB_EPnR_CTR_RX | HAL_USB_EPnR_CTR_TX);
}

int hal_usb_ep_init(uint8_t ep, uint8_t attrs, uint16_t buf_size) {
    static uint32_t offset = 0;

    switch (HAL_READ_FIELD(attrs, HAL_EP_TRANSFER_MASK, HAL_EP_TRANSFER_OFFSET)) {
    case HAL_EP_CONTROL:
        hal_usb_ep_reg_write(ep, HAL_USB_EPnR_EPTYPE_CTL,
            HAL_USB_EPnR_EPTYPE_MASK, HAL_USB_EPnR_EPTYPE_OFFSET);
        break;
    case HAL_EP_ISOCHRONUS:
        hal_usb_ep_reg_write(ep, HAL_USB_EPnR_EPTYPE_ISO,
            HAL_USB_EPnR_EPTYPE_MASK, HAL_USB_EPnR_EPTYPE_OFFSET);
        break;
    case HAL_EP_BULK:
        hal_usb_ep_reg_write(ep, HAL_USB_EPnR_EPTYPE_BULK,
            HAL_USB_EPnR_EPTYPE_MASK, HAL_USB_EPnR_EPTYPE_OFFSET);
        break;
    case HAL_EP_INTERRUPT:
        hal_usb_ep_reg_write(ep, HAL_USB_EPnR_EPTYPE_INT,
            HAL_USB_EPnR_EPTYPE_MASK, HAL_USB_EPnR_EPTYPE_OFFSET);
        break;
    default:
        break;
    }

    hal_usb_ep_reg_write(ep, ep, HAL_USB_EPnR_EA_MASK, HAL_USB_EPnR_EA_OFFSET);


    if (ep & 0x80) {
        HAL_USB_BTABLE->ep[ep & 0x0F].ADDR_RX = (HAL_BTABLE_OFFSET + offset);
        // hal_usb_ep_reg_toggle(ep, 0,USB_EPnR_)
    } else {
        HAL_USB_BTABLE->ep[ep & 0x0F].ADDR_TX = (HAL_BTABLE_OFFSET + offset);
    }
    offset += buf_size;

    return 0;
}

int16_t hal_usb_ep_bytes_avail(uint8_t ep_n) {
    return (HAL_USB_BTABLE->ep[ep_n].COUNT_RX & 0x3FU);
}

void hal_usb_irq_handler(void) {

}

void hal_usb_ep_set_stall(uint8_t ep) {

}

void hal_usb_ep_clear_stall(uint8_t ep) {

}

int hal_usb_ep_start_transfer(uint8_t ep) {
    return 0;
}


/*
0x18  | 24
0x58  | 88
0xC0  |
0x110 |
0x100 |
*/
