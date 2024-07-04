/**
 * @file    stm32f1xx_usb.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-28
 *
 * @brief
 */
#ifndef STM32F1XX_USB_H
#define STM32F1XX_USB_H


#include <stdint.h>


/*
 * Common registers
 *
 * These registers affect the general behavior of the USB peripheral
 * defining operating mode, interrupt handling, device address and
 * giving access to the current frame number updated by the host PC.
 */

/*
 * USB control register (USB_CNTR)
 *
 * Address offset : 0x40
 * Reset value    : 0x0003
 */

/*
 * Bit 15 CTRM: Correct transfer interrupt mask
 *
 * 0: Correct Transfer (CTR) Interrupt disabled.
 * 1: CTR Interrupt enabled, an interrupt request is generated when
 *    the corresponding bit in the USB_ISTR register is set.
 */
#define HAL_USB_CNTR_CTRM_OFFSET       15U
#define HAL_USB_CNTR_CTRM_MASK         1U
#define HAL_USB_CNTR_CTRM              (uint32_t)((HAL_USB_CNTR_CTRM_MASK) << (HAL_USB_CNTR_CTRM_OFFSET))

/*
 * Bit 14 PMAOVRM: Packet memory area over / underrun interrupt mask
 *
 * 0: PMAOVR Interrupt disabled.
 * 1: PMAOVR Interrupt enabled, an interrupt request is generated when
 *    the corresponding bit in the USB_ISTR register is set.
 */
#define HAL_USB_CNTR_PMAOVRM_OFFSET    14U
#define HAL_USB_CNTR_PMAOVRM_MASK      1U
#define HAL_USB_CNTR_PMAOVRM           (uint32_t)((HAL_USB_CNTR_PMAOVRM_MASK) << (HAL_USB_CNTR_PMAOVRM_OFFSET))

/*
 * Bit 13 ERRM: Error interrupt mask
 *
 * 0: ERR Interrupt disabled.
 * 1: ERR Interrupt enabled, an interrupt request is generated when
 * the corresponding bit in the USB_ISTR register is set.
 */
#define HAL_USB_CNTR_ERRM_OFFSET       13U
#define HAL_USB_CNTR_ERRM_MASK         1U
#define HAL_USB_CNTR_ERRM              (uint32_t)((HAL_USB_CNTR_ERRM_MASK) << (HAL_USB_CNTR_ERRM_OFFSET))

/*
 * Bit 12 WKUPM: Wakeup interrupt mask
 *
 * 0: WKUP Interrupt disabled.
 * 1: WKUP Interrupt enabled, an interrupt request is generated when
 *    the corresponding bit in the USB_ISTR register is set.
 */
#define HAL_USB_CNTR_WKUPM_OFFSET      12U
#define HAL_USB_CNTR_WKUPM_MASK        1U
#define HAL_USB_CNTR_WKUPM             (uint32_t)((HAL_USB_CNTR_WKUPM_MASK) << (HAL_USB_CNTR_WKUPM_OFFSET))

/*
 * Bit 11 SUSPM: Suspend mode interrupt mask
 *
 * 0: Suspend Mode Request (SUSP) Interrupt disabled.
 * 1: SUSP Interrupt enabled, an interrupt request is generated when
 *    the corresponding bit in the USB_ISTR register is set.
 */
#define HAL_USB_CNTR_SUSPM_OFFSET      11U
#define HAL_USB_CNTR_SUSPM_MASK        1U
#define HAL_USB_CNTR_SUSPM             (uint32_t)((HAL_USB_CNTR_SUSPM_MASK) << (HAL_USB_CNTR_SUSPM_OFFSET))

/*
 * Bit 10 RESETM: USB reset interrupt mask
 *
 * 0: RESET Interrupt disabled.
 * 1: RESET Interrupt enabled, an interrupt request is generated when
 *    the corresponding bit in the USB_ISTR register is set.
 */
#define HAL_USB_CNTR_RESETM_OFFSET     10U
#define HAL_USB_CNTR_RESETM_MASK       1U
#define HAL_USB_CNTR_RESETM            (uint32_t)((HAL_USB_CNTR_RESETM_MASK) << (HAL_USB_CNTR_RESETM_OFFSET))

/*
 * Bit 9 SOFM: Start of frame interrupt mask
 *
 * 0: SOF Interrupt disabled.
 * 1: SOF Interrupt enabled, an interrupt request is generated when
 * the corresponding bit in the USB_ISTR register is set.
 */
#define HAL_USB_CNTR_SOFM_OFFSET       9U
#define HAL_USB_CNTR_SOFM_MASK         1U
#define HAL_USB_CNTR_SOFM              (uint32_t)((HAL_USB_CNTR_SOFM_MASK) << (HAL_USB_CNTR_SOFM_OFFSET))

/*
 * Bit 8 ESOFM: Expected start of frame interrupt mask
 *
 * 0: Expected Start of Frame (ESOF) Interrupt disabled.
 * 1: ESOF Interrupt enabled, an interrupt request is generated when
 * the corresponding bit in the USB_ISTR register is set.
 */
#define HAL_USB_CNTR_ESOFM_OFFSET      8U
#define HAL_USB_CNTR_ESOFM_MASK        1U
#define HAL_USB_CNTR_ESOFM             (uint32_t)((HAL_USB_CNTR_ESOFM_MASK) << (HAL_USB_CNTR_ESOFM_OFFSET))

/*
 * Bit 4 RESUME: Resume request
 *
 * The microcontroller can set this bit to send a Resume signal to the
 * host. It must be activated, according to USB specifications, for no
 * less than 1 mS and no more than 15 mS after which the Host PC is
 * ready to drive the resume sequence up to its end.
 */
#define HAL_USB_CNTR_RESUME_OFFSET     4U
#define HAL_USB_CNTR_RESUME_MASK       1U
#define HAL_USB_CNTR_RESUME            (uint32_t)((HAL_USB_CNTR_RESUME_MASK) << (HAL_USB_CNTR_RESUME_OFFSET))

/*
 * Bit 3 FSUSP: Force suspend
 *
 * Software must set this bit when the SUSP interrupt is received,
 * which is issued when no traffic is received by the USB peripheral
 * for 3 mS.
 *
 * 0: No effect.
 * 1: Enter suspend mode. Clocks and static power dissipation in the
 * analog transceiver are left unaffected. If suspend power consumption
 * is a requirement (bus-powered device), the application software should
 * set the LP_MODE bit after FSUSP as explained below.
 */
#define HAL_USB_CNTR_FSUSP_OFFSET      3U
#define HAL_USB_CNTR_FSUSP_MASK        1U
#define HAL_USB_CNTR_FSUSP             (uint32_t)((HAL_USB_CNTR_FSUSP_MASK) << (HAL_USB_CNTR_FSUSP_OFFSET))

/*
 * Bit 2 LP_MODE: Low-power mode
 *
 * This mode is used when the suspend-mode power constraints require
 * that all static power dissipation is avoided, except the one required
 * to supply the external pull-up resistor. This condition should be
 * entered when the application is ready to stop all system clocks, or
 * reduce their frequency in order to meet the power consumption
 * requirements of the USB suspend condition. The USB activity during
 * the suspend mode (WKUP event) asynchronously resets this bit (it
 * can also be reset by software).
 *
 * 0: No Low-power mode.
 * 1: Enter Low-power mode.
 */
#define HAL_USB_CNTR_LP_MODE_OFFSET    2U
#define HAL_USB_CNTR_LP_MODE_MASK      1U
#define HAL_USB_CNTR_LP_MODE           (uint32_t)((HAL_USB_CNTR_LP_MODE_MASK) << (HAL_USB_CNTR_LP_MODE_OFFSET))

/*
 * Bit 1 PDWN: Power down
 *
 * This bit is used to completely switch off all USB-related analog
 * parts if it is required to completely disable the USB peripheral
 * for any reason. When this bit is set, the USB peripheral is
 * disconnected from the transceivers and it cannot be used.
 *
 * 0: Exit Power Down.
 * 1: Enter Power down mode.
 */
#define HAL_USB_CNTR_PDWN_OFFSET       1U
#define HAL_USB_CNTR_PDWN_MASK         1U
#define HAL_USB_CNTR_PDWN              (uint32_t)((HAL_USB_CNTR_PDWN_MASK) << (HAL_USB_CNTR_PDWN_OFFSET))

/*
 * Bit 0 FRES: Force USB Reset
 *
 * 0: Clear USB reset.
 * 1: Force a reset of the USB peripheral, exactly like a RESET signalling
 * on the USB. The USB peripheral is held in RESET state until software
 * clears this bit. A “USB-RESET” interrupt is generated, if enabled.
 */
#define HAL_USB_CNTR_FRES_OFFSET       0U
#define HAL_USB_CNTR_FRES_MASK         1U
#define HAL_USB_CNTR_FRES              (uint32_t)((HAL_USB_CNTR_FRES_MASK) << (HAL_USB_CNTR_FRES_OFFSET))


/*
 * USB interrupt status register (USB_ISTR)
 *
 * Address offset : 0x44
 * Reset value    : 0x00000000
 *
 * This register contains the status of all the interrupt sources
 * allowing application software to determine, which events caused an
 * interrupt request.
 *
 * The upper part of this register contains single bits, each of them
 * representing a specific event. These bits are set by the hardware
 * when the related event occurs; if the corresponding bit in the
 * USB_CNTR register is set, a generic interrupt request is generated.
 * The interrupt routine, examining each bit, will perform all necessary
 * actions, and finally it will clear the serviced bits. If any of them
 * is not cleared, the interrupt is considered to be still pending, and
 * the interrupt line will be kept high again. If several bits are set
 * simultaneously, only a single interrupt will be generated.
 *
 * Endpoint transaction completion can be handled in a different way
 * to reduce interrupt response latency. The CTR bit is set by the
 * hardware as soon as an endpoint successfully completes a transaction,
 * generating a generic interrupt request if the corresponding bit in
 * USB_CNTR is set. An endpoint dedicated interrupt condition is activated
 * independently from the CTRM bit in the USB_CNTR register. Both interrupt
 * conditions remain active until software clears the pending bit in the
 * corresponding USB_EPnR register (the CTR bit is actually a read only bit).
 * For endpoint-related interrupts, the software can use the Direction
 * of Transaction (DIR) and EP_ID read-only bits to identify, which
 * endpoint made the last interrupt request and called the corresponding
 * interrupt service routine.
 *
 * The user can choose the relative priority of simultaneously pending
 * USB_ISTR events by specifying the order in which software checks
 * USB_ISTR bits in an interrupt service routine. Only the bits related
 * to events, which are serviced, are cleared. At the end of the service
 * routine, another interrupt will be requested, to service the remaining
 * conditions.
 *
 * To avoid spurious clearing of some bits, it is recommended to clear
 * them with a load instruction where all bits which must not be altered
 * are written with 1, and all bits to be cleared are written with ‘0
 * (these bits can only be cleared by software). Read-modify-write cycles
 * should be avoided because between the read and the write operations
 * another bit could be set by the hardware and the next write will
 * clear it before the microprocessor has the time to serve the event.
 */

/*
 * Bit 15 CTR: Correct transfer
 *
 * This bit is set by the hardware to indicate that an endpoint has
 * successfully completed a transaction; using DIR and EP_ID bits
 * software can determine which endpoint requested the interrupt.
 * This bit is read-only.
 */
#define HAL_USB_ISTR_CTR_OFFSET       15U
#define HAL_USB_ISTR_CTR_MASK         1U
#define HAL_USB_ISTR_CTR              (uint32_t)((HAL_USB_ISTR_CTR_MASK) << (HAL_USB_ISTR_CTR_OFFSET))

/*
 * Bit 14 PMAOVR: Packet memory area over/underrun
 *
 * This bit is set if the microcontroller has not been able to respond
 * in time to an USB memory request. The USB peripheral handles this
 * event in the following way: During reception an ACK handshake packet
 * is not sent, during transmission a bit-stuff error is forced on the
 * transmitted stream; in both cases the host will retry the transaction.
 * The PMAOVR interrupt should never occur during normal operations.
 * Since the failed transaction is retried by the host, the application
 * software has the chance to speed-up device operations during this
 * interrupt handling, to be ready for the next transaction retry;
 * however this does not happen during Isochronous transfers (no isochronous
 * transaction is anyway retried) leading to a loss of data in this case.
 * This bit is read/write but only ‘0 can be written and writing ‘1
 * has no effect.
 */
#define HAL_USB_ISTR_PMAOVR_OFFSET    14U
#define HAL_USB_ISTR_PMAOVR_MASK      1U
#define HAL_USB_ISTR_PMAOVR           (uint32_t)((HAL_USB_ISTR_PMAOVR_MASK) << (HAL_USB_ISTR_PMAOVR_OFFSET))

/*
 * Bit 13 ERR: Error
 *
 * This flag is set whenever one of the errors listed below has occurred:
 * NANS: No ANSwer. The timeout for a host response has expired.
 * CRC: Cyclic Redundancy Check error. One of the received CRCs, either
 * in the token or in the data, was wrong.
 * BST: Bit Stuffing error. A bit stuffing error was detected anywhere
 * in the PID, data, and/or CRC.
 * FVIO: Framing format Violation. A non-standard frame was received
 * (EOP not in the right place, wrong token sequence, etc.).
 * The USB software can usually ignore errors, since the USB peripheral
 * and the PC host manage retransmission in case of errors in a fully
 * transparent way. This interrupt can be useful during the software
 * development phase, or to monitor the quality of transmission over
 * the USB bus, to flag possible problems to the user (e.g. loose
 * connector, too noisy environment, broken conductor in the USB cable
 * and so on). This bit is read/write but only ‘0 can be written and
 * writing ‘1 has no effect.
 */
#define HAL_USB_ISTR_ERR_OFFSET       13U
#define HAL_USB_ISTR_ERR_MASK         1U
#define HAL_USB_ISTR_ERR              (uint32_t)((HAL_USB_ISTR_ERR_MASK) << (HAL_USB_ISTR_ERR_OFFSET))

/*
 * Bit 12 WKUP: Wakeup
 *
 * This bit is set to 1 by the hardware when, during suspend mode,
 * activity is detected that wakes up the USB peripheral. This event
 * asynchronously clears the LP_MODE bit in the CTLR register and
 * activates the USB_WAKEUP line, which can be used to notify the rest
 * of the device (e.g. wakeup unit) about the start of the resume process.
 * This bit is read/write but only ‘0 can be written and writing ‘1 has
 * no effect.
 */
#define HAL_USB_ISTR_WKUP_OFFSET      12U
#define HAL_USB_ISTR_WKUP_MASK        1U
#define HAL_USB_ISTR_WKUP             (uint32_t)((HAL_USB_ISTR_WKUP_MASK) << (HAL_USB_ISTR_WKUP_OFFSET))

/*
 * Bit 11 SUSP: Suspend mode request
 *
 * This bit is set by the hardware when no traffic has been received
 * for 3mS, indicating a suspend mode request from the USB bus. The
 * suspend condition check is enabled immediately after any USB reset
 * and it is disabled by the hardware when the suspend mode is active
 * (FSUSP=1) until the end of resume sequence. This bit is read/write
 * but only ‘0 can be written and writing ‘1 has no effect.
 */
#define HAL_USB_ISTR_SUSP_OFFSET      11U
#define HAL_USB_ISTR_SUSP_MASK        1U
#define HAL_USB_ISTR_SUSP             (uint32_t)((HAL_USB_ISTR_SUSP_MASK) << (HAL_USB_ISTR_SUSP_OFFSET))

/*
 * Bit 10 RESET: USB reset request
 *
 * Set when the USB peripheral detects an active USB RESET signal at
 * its inputs. The USB peripheral, in response to a RESET, just resets
 * its internal protocol state machine, generating an interrupt if
 * RESETM enable bit in the USB_CNTR register is set. Reception and
 * transmission are disabled until the RESET bit is cleared. All
 * configuration registers do not reset: the microcontroller must
 * explicitly clear these registers (this is to ensure that the RESET
 * interrupt can be safely delivered, and any transaction immediately
 * followed by a RESET can be completed). The function address and
 * endpoint registers are reset by an USB reset event.
 *
 * This bit is read/write but only ‘0 can be written and writing ‘1
 * has no effect.
 */
#define HAL_USB_ISTR_RESET_OFFSET     10U
#define HAL_USB_ISTR_RESET_MASK       1U
#define HAL_USB_ISTR_RESET            (uint32_t)((HAL_USB_ISTR_RESET_MASK) << (HAL_USB_ISTR_RESET_OFFSET))

/*
 * Bit 9 SOF: Start of frame
 *
 * This bit signals the beginning of a new USB frame and it is set
 * when a SOF packet arrives through the USB bus. The interrupt service
 * routine may monitor the SOF events to have a 1 mS synchronization
 * event to the USB host and to safely read the USB_FNR register which
 * is updated at the SOF packet reception (this could be useful for
 * isochronous applications). This bit is read/write but only ‘0 can
 * be written and writing ‘1 has no effect.
 */
#define HAL_USB_ISTR_SOF_OFFSET       9U
#define HAL_USB_ISTR_SOF_MASK         1U
#define HAL_USB_ISTR_SOF              (uint32_t)((HAL_USB_ISTR_SOF_MASK) << (HAL_USB_ISTR_SOF_OFFSET))

/*
 * Bit 8 ESOF: Expected start of frame
 *
 * This bit is set by the hardware when an SOF packet is expected but
 * not received. The host sends an SOF packet each mS, but if the hub
 * does not receive it properly, the Suspend Timer issues this interrupt.
 * If three consecutive ESOF interrupts are generated (i.e. three SOF
 * packets are lost) without any traffic occurring in between, a SUSP
 * interrupt is generated. This bit is set even when the missing SOF
 * packets occur while the Suspend Timer is not yet locked. This bit
 * is read/write but only ‘0 can be written and writing ‘1 has no effect.
 */
#define HAL_USB_ISTR_ESOF_OFFSET      8U
#define HAL_USB_ISTR_ESOF_MASK        1U
#define HAL_USB_ISTR_ESOF             (uint32_t)((HAL_USB_ISTR_ESOF_MASK) << (HAL_USB_ISTR_ESOF_OFFSET))

/*
 * Bit 4 DIR: Direction of transaction
 *
 * This bit is written by the hardware according to the direction of
 * the successful transaction, which generated the interrupt request.
 *
 * If DIR bit=0, CTR_TX bit is set in the USB_EPnR register related to
 * the interrupting endpoint. The interrupting transaction is of IN type
 * (data transmitted by the USB peripheral to the host PC).
 *
 * If DIR bit=1, CTR_RX bit or both CTR_TX/CTR_RX are set in the
 * USB_EPnR register related to the interrupting endpoint. The
 * interrupting transaction is of OUT type (data received by the USB
 * peripheral from the host PC) or two pending transactions are waiting
 * to be processed.
 *
 * This information can be used by the application software to access
 * the USB_EPnR bits related to the triggering transaction since it
 * represents the direction having the interrupt pending. This bit is
 * read-only.
 */
#define HAL_USB_ISTR_DIR_OFFSET       4U
#define HAL_USB_ISTR_DIR_MASK         1U
#define HAL_USB_ISTR_DIR              (uint32_t)((HAL_USB_ISTR_DIR_MASK) << (HAL_USB_ISTR_DIR_OFFSET))

/*
 * Bits 3:0 EP_ID[3:0]: Endpoint Identifier
 *
 * These bits are written by the hardware according to the endpoint
 * number, which generated the interrupt request. If several endpoint
 * transactions are pending, the hardware writes the endpoint identifier
 * related to the endpoint having the highest priority defined in the
 * following way: Two endpoint sets are defined, in order of priority:
 * Isochronous and double-buffered bulk endpoints are considered first
 * and then the other endpoints are examined. If more than one endpoint
 * from the same set is requesting an interrupt, the EP_ID bits in USB_ISTR
 * register are assigned according to the lowest requesting endpoint
 * register, EP0R having the highest priority followed by EP1R and so
 * on. The application software can assign a register to each endpoint
 * according to this priority scheme, so as to order the concurring
 * endpoint requests in a suitable way. These bits are read only.
 */
#define HAL_USB_ISTR_EP_ID_OFFSET     0U
#define HAL_USB_ISTR_EP_ID_MASK       0x0FU
#define HAL_USB_ISTR_EP_ID            (uint32_t)((HAL_USB_ISTR_EP_ID_MASK) << (HAL_USB_ISTR_EP_ID_OFFSET))


/*
 * USB frame number register (USB_FNR)
 *
 * Address offset : 0x48
 * Reset value    : 0x0XXX where X is undefined
 */

/*
 * Bit 15 RXDP: Receive data + line status
 *
 * This bit can be used to observe the status of received data plus
 * upstream port data line. It can be used during end-of-suspend
 * routines to help determining the wakeup event.
 */
#define HAL_USB_FNR_RXDP_OFFSET        15U
#define HAL_USB_FNR_RXDP_MASK          1U
#define HAL_USB_FNR_RXDP               (uint32_t)((HAL_USB_FNR_RXDP_MASK) << (HAL_USB_FNR_RXDP_OFFSET))

/*
 * Bit 14 RXDM: Receive data - line status
 *
 * This bit can be used to observe the status of received data minus
 * upstream port data line. It can be used during end-of-suspend routines
 * to help determining the wakeup event.
 */
#define HAL_USB_FNR_RXDM_OFFSET        14U
#define HAL_USB_FNR_RXDM_MASK          1U
#define HAL_USB_FNR_RXDM               (uint32_t)((HAL_USB_FNR_RXDM_MASK) << (HAL_USB_FNR_RXDM_OFFSET))

/*
 * Bit 13 LCK: Locked
 *
 * This bit is set by the hardware when at least two consecutive SOF
 * packets have been received after the end of an USB reset condition
 * or after the end of an USB resume sequence. Once locked, the frame
 * timer remains in this state until an USB reset or USB suspend event
 * occurs.
 */
#define HAL_USB_FNR_LCK_OFFSET         13U
#define HAL_USB_FNR_LCK_MASK           1U
#define HAL_USB_FNR_LCK                (uint32_t)((HAL_USB_FNR_LCK_MASK) << (HAL_USB_FNR_LCK_OFFSET))

/*
 * Bits 12:11 LSOF[1:0]: Lost SOF
 *
 * These bits are written by the hardware when an ESOF interrupt is
 * generated, counting the number of consecutive SOF packets lost.
 * At the reception of an SOF packet, these bits are cleared.
 */
#define HAL_USB_FNR_LSOF_OFFSET        12U
#define HAL_USB_FNR_LSOF_MASK          0x03U
#define HAL_USB_FNR_LSOF               (uint32_t)((HAL_USB_FNR_LSOF_MASK) << (HAL_USB_FNR_LSOF_OFFSET))

/*
 * Bits 10:0 FN[10:0]: Frame number
 *
 * This bit field contains the 11-bits frame number contained in the
 * last received SOF packet. The frame number is incremented for every
 * frame sent by the host and it is useful for Isochronous transfers.
 * This bit field is updated on the generation of an SOF interrupt.
 */
#define HAL_USB_FNR_FN_OFFSET          15U
#define HAL_USB_FNR_FN_MASK            0x07FFU
#define HAL_USB_FNR_FN                 (uint32_t)((HAL_USB_FNR_FN_MASK) << (HAL_USB_FNR_FN_OFFSET))


/*
 * USB device address (USB_DADDR)
 * Address offset : 0x4C
 * Reset value    : 0x0000
 */

/*
 * Bit 7 EF: Enable function
 *
 * This bit is set by the software to enable the USB device. The address
 * of this device is contained in the following ADD[6:0] bits. If this
 * bit is at ‘0 no transactions are handled, irrespective of the settings
 * of USB_EPnR registers.
 */
#define HAL_USB_DADDR_EF_OFFSET         7U
#define HAL_USB_DADDR_EF_MASK           1U
#define HAL_USB_DADDR_EF                (uint32_t)((HAL_USB_DADDR_EF_MASK) << (HAL_USB_DADDR_EF_OFFSET))

/*
 * Bits 6:0 ADD[6:0]: Device address
 *
 * These bits contain the USB function address assigned by the host PC
 * during the enumeration process. Both this field and the Endpoint
 * Address (EA) field in the associated USB_EPnR register must match
 * with the information contained in a USB token in order to handle a
 * transaction to the required endpoint.
 */
#define HAL_USB_DADDR_ADD_OFFSET         0U
#define HAL_USB_DADDR_ADD_MASK           0x003FU
#define HAL_USB_DADDR_ADD                (uint32_t)((HAL_USB_DADDR_ADD_MASK) << (HAL_USB_DADDR_ADD_OFFSET))


/*
 * Buffer table address (USB_BTABLE)
 *
 * Address offset : 0x50
 * Reset value    : 0x0000
 */

/*
 * Bits 15:3 BTABLE[15:3]: Buffer table
 *
 * These bits contain the start address of the buffer allocation table
 * inside the dedicated packet memory. This table describes each endpoint
 * buffer location and size and it must be aligned to an 8 byte boundary
 * (the 3 least significant bits are always ‘0). At the beginning of every
 * transaction addressed to this device, the USP peripheral reads the
 * element of this table related to the addressed endpoint, to get its
 * buffer start location and the buffer size
 */
#define HAL_USB_BTABLE_OFFSET           3U
#define HAL_USB_BTABLE_MASK             0x1FFF
#define HAL_USB_BTABLE                  (uint32_t)((HAL_USB_BTABLE_MASK) << (HAL_USB_BTABLE_OFFSET))


/*
 * Endpoint-specific registers
 *
 * The number of these registers varies according to the number of
 * endpoints that the USB peripheral is designed to handle. The USB
 * peripheral supports up to 8 bidirectional endpoints. Each USB device
 * must support a control endpoint whose address (EA bits) must be set
 * to 0. The USB peripheral behaves in an undefined way if multiple
 * endpoints are enabled having the same endpoint number value. For
 * each endpoint, an USB_EPnR register is available to store the endpoint
 * specific information.
 */

/*
 * USB endpoint n register (USB_EPnR), n=[0..7]
 *
 * Address offset : 0x00 to 0x1C
 * Reset value    : 0x0000
 *
 * They are also reset when an USB reset is received from the USB bus
 * or forced through bit FRES in the CTLR register, except the CTR_RX
 * and CTR_TX bits, which are kept unchanged to avoid missing a correct
 * packet notification immediately followed by an USB reset event. Each
 * endpoint has its USB_EPnR register where n is the endpoint identifier.
 *
 * Read-modify-write cycles on these registers should be avoided because
 * between the read and the write operations some bits could be set by
 * the hardware and the next write would modify them before the CPU has
 * the time to detect the change. For this purpose, all bits affected
 * by this problem have an ‘invariant’ value that must be used whenever
 * their modification is not required. It is recommended to modify these
 * registers with a load instruction where all the bits, which can be
 * modified only by the hardware, are written with their ‘invariant’ value.
 */

/*
 * Bit 15 CTR_RX: Correct Transfer for reception
 *
 * This bit is set by the hardware when an OUT/SETUP transaction is
 * successfully completed on this endpoint; the software can only clear
 * this bit. If the CTRM bit in USB_CNTR register is set accordingly,
 * a generic interrupt condition is generated together with the endpoint
 * related interrupt condition, which is always activated. The type of
 * occurred transaction, OUT or SETUP, can be determined from the SETUP
 * bit described below.
 *
 * A transaction ended with a NAK or STALL handshake does not set this
 * bit, since no data is actually transferred, as in the case of protocol
 * errors or data toggle mismatches.
 *
 * This bit is read/write but only ‘0 can be written, writing 1 has no effect.
 */
#define HAL_USB_EPnR_CTR_RX_OFFSET      15U
#define HAL_USB_EPnR_CTR_RX_MASK        1U
#define HAL_USB_EPnR_CTR_RX             (uint32_t)((HAL_USB_EPnR_CTR_RX_MASK) << (HAL_USB_EPnR_CTR_RX_OFFSET))

/*
 * Bit 14 DTOG_RX: Data Toggle, for reception transfers
 *
 * If the endpoint is not Isochronous, this bit contains the expected
 * value of the data toggle bit (0=DATA0, 1=DATA1) for the next data
 * packet to be received. Hardware toggles this bit, when the ACK
 * handshake is sent to the USB host, following a data packet reception
 * having a matching data PID value; if the endpoint is defined as a
 * control one, hardware clears this bit at the reception of a SETUP PID
 * addressed to this endpoint.
 *
 * If the endpoint is using the double-buffering feature this bit is
 * used to support packet buffer swapping too
 *
 * If the endpoint is Isochronous, this bit is used only to support
 * packet buffer swapping since no data toggling is used for this sort
 * of endpoints and only DATA0 packet are transmitted. Hardware toggles
 * this bit just after the end of data packet reception, since no
 * handshake is used for isochronous transfers.
 *
 * If the endpoint is Isochronous, this bit is used only to support
 * packet buffer swapping since no data toggling is used for this sort
 * of endpoints and only DATA0 packet are transmitted. Hardware toggles
 * this bit just after the end of data packet reception, since no handshake
 * is used for isochronous transfers.
 *
 * This bit can also be toggled by the software to initialize its value
 * (mandatory when the endpoint is not a control one) or to force specific
 * data toggle/packet buffer usage. When the application software writes
 * ‘0, the value of DTOG_RX remains unchanged, while writing ‘1 makes
 * the bit value toggle. This bit is read/write but it can be only toggled
 * by writing 1.
 */
#define HAL_USB_EPnR_DTOG_RX_OFFSET     14U
#define HAL_USB_EPnR_DTOG_RX_MASK       1U
#define HAL_USB_EPnR_DTOG_RX            (uint32_t)((HAL_USB_EPnR_DTOG_RX_MASK) << (HAL_USB_EPnR_DTOG_RX_OFFSET))

/*
 * Bits 13:12 STAT_RX [1:0]: Status bits, for reception transfers
 *
 * These bits contain information about the endpoint status, which are
 * listed in Table 173. These bits can be toggled by software to initialize
 * their value. When the application software writes ‘0, the value remains
 * unchanged, while writing ‘1 makes the bit value toggle.
 *
 * Hardware sets the STAT_RX bits to NAK when a correct transfer has
 * occurred (CTR_RX=1) corresponding to a OUT or SETUP (control only)
 * transaction addressed to this endpoint, so the software has the time
 * to elaborate the received data before it acknowledge a new transaction.
 *
 * Double-buffered bulk endpoints implement a special transaction flow
 * control, which control the status based upon buffer availability
 * condition.
 *
 * If the endpoint is defined as Isochronous, its status can be only
 * “VALID” or “DISABLED”, so that the hardware cannot change the status
 * of the endpoint after a successful transaction. If the software sets
 * the STAT_RX bits to ‘STALL’ or ‘NAK’ for an Isochronous endpoint, the
 * USB peripheral behavior is not defined. These bits are read/write but
 * they can be onlytoggled by writing ‘1.
 */
#define HAL_USB_EPnR_STAT_RX_OFFSET     12U
#define HAL_USB_EPnR_STAT_RX_MASK       0x03U
#define HAL_USB_EPnR_STAT_RX            (uint32_t)((HAL_USB_EPnR_STAT_RX_MASK) << (HAL_USB_EPnR_STAT_RX_OFFSET))

/*
 * Bit 11 SETUP: Setup transaction completed
 *
 * This bit is read-only and it is set by the hardware when the last
 * completed transaction is a SETUP. This bit changes its value only
 * for control endpoints. It must be examined, in the case of a successful
 * receive transaction (CTR_RX event), to determine the type of transaction
 * occurred. To protect the interrupt service routine from the changes
 * in SETUP bits due to next incoming tokens, this bit is kept frozen
 * while CTR_RX bit is at 1; its state changes when CTR_RX is at 0.
 * This bit is read-only.
 */
#define HAL_USB_EPnR_SETUP_OFFSET       11U
#define HAL_USB_EPnR_SETUP_MASK         1U
#define HAL_USB_EPnR_SETUP              (uint32_t)((HAL_USB_EPnR_SETUP_MASK) << (HAL_USB_EPnR_SETUP_OFFSET))

/*
 * Bits 10:9 EP_TYPE[1:0]: Endpoint type
 *
 * These bits configure the behavior of this endpoint as described in
 * Table 174. Endpoint 0 must always be a control endpoint and each USB
 * function must have at least one control endpoint which has address 0,
 * but there may be other control endpoints if required. Only control
 * endpoints handle SETUP transactions, which are ignored by endpoints
 * of other kinds. SETUP transactions cannot be answered with NAK or
 * STALL. If a control endpoint is defined as NAK, the USB peripheral
 * will not answer, simulating a receive error, in the receive direction
 * when a SETUP transaction is received. If the control endpoint is
 * defined as STALL in the receive direction, then the SETUP packet
 * will be accepted anyway, transferring data and issuing the CTR
 * interrupt. The reception of OUT transactions is handled in the
 * normal way, even if the endpoint is a control one. Bulk and interrupt
 * endpoints have very similar behavior and they differ only in the special
 * feature available using the EP_KIND configuration bit.
 * The usage of Isochronous endpoints is explained in Section 23.4.4
*/
#define HAL_USB_EPnR_EPTYPE_OFFSET      9U
#define HAL_USB_EPnR_EPTYPE_MASK        0x03U
#define HAL_USB_EPnR_EPTYPE             (uint32_t)((HAL_USB_EPnR_EPTYPE_MASK) << (HAL_USB_EPnR_EPTYPE_OFFSET))

#define HAL_USB_EPnR_EPTYPE_BULK        0x00U
#define HAL_USB_EPnR_EPTYPE_CTL         0x01U
#define HAL_USB_EPnR_EPTYPE_ISO         0x02U
#define HAL_USB_EPnR_EPTYPE_INT         0x03U

/*
 * Bit 8 EP_KIND: Endpoint kind
 *
 * The meaning of this bit depends on the endpoint type configured by
 * the EP_TYPE bits. Table 175 summarizes the different meanings.
 *
 * DBL_BUF: This bit is set by the software to enable the double-buffering
 * feature for this bulk endpoint. The usage of double-buffered bulk
 * endpoints is explained in Section 23.4.3.
 *
 * STATUS_OUT: This bit is set by the software to indicate that a status
 * out transaction is expected: in this case all OUT transactions containing
 * more than zero data bytes are answered ‘STALL’ instead of ‘ACK’. This
 * bit may be used to improve the robustness of the application to protocol
 * errors during control transfers and its usage is intended for control
 * endpoints only. When STATUS_OUT is reset, OUT transactions can have
 * any number of bytes, as required.
 */
#define HAL_USB_EPnR_EP_KIND_OFFSET     8U
#define HAL_USB_EPnR_EP_KIND_MASK       1U
#define HAL_USB_EPnR_EP_KIND            (uint32_t)((HAL_USB_EPnR_EP_KIND_MASK) << (HAL_USB_EPnR_EP_KIND_OFFSET))

/*
 * Bit 7 CTR_TX: Correct Transfer for transmission
 *
 * This bit is set by the hardware when an IN transaction is successfully
 * completed on this endpoint; the software can only clear this bit. If
 * the CTRM bit in the USB_CNTR register is set accordingly, a generic
 * interrupt condition is generated together with the endpoint related
 * interrupt condition, which is always activated. A transaction ended
 * with a NAK or STALL handshake does not set this bit, since no data is
 * actually transferred, as in the case of protocol errors or data toggle
 * mismatches. This bit is read/write but only ‘0 can be written.
 */
#define HAL_USB_EPnR_CTR_TX_OFFSET      7U
#define HAL_USB_EPnR_CTR_TX_MASK        1U
#define HAL_USB_EPnR_CTR_TX             (uint32_t)((HAL_USB_EPnR_CTR_TX_MASK) << (HAL_USB_EPnR_CTR_TX_OFFSET))

/*
 * Bit 6 DTOG_TX: Data Toggle, for transmission transfers
 *
 * If the endpoint is non-isochronous, this bit contains the required
 * value of the data toggle bit (0=DATA0, 1=DATA1) for the next data
 * packet to be transmitted. Hardware toggles this bit when the ACK
 * handshake is received from the USB host, following a data packet
 * transmission. If the endpoint is defined as a control one, hardware
 * sets this bit to 1 at the reception of a SETUP PID addressed to this
 * endpoint.
 *
 * If the endpoint is using the double buffer feature, this bit is used
 * to support packet buffer swapping too (Refer to Section 23.4.3)
 *
 * If the endpoint is Isochronous, this bit is used to support packet
 * buffer swapping since no data toggling is used for this sort of
 * endpoints and only DATA0 packet are transmitted (Refer to Section
 * 23.4.4). Hardware toggles this bit just after the end of data packet
 * transmission, since no handshake is used for Isochronous transfers.
 *
 * This bit can also be toggled by the software to initialize its value
 * (mandatory when the endpoint is not a control one) or to force a
 * specific data toggle/packet buffer usage. When the application software
 * writes ‘0, the value of DTOG_TX remains unchanged, while writing ‘1
 * makes the bit value toggle. This bit is read/write but it can only
 * be toggled by writing 1.
 */
#define HAL_USB_EPnR_DTOG_TX_OFFSET     6U
#define HAL_USB_EPnR_DTOG_TX_MASK       1U
#define HAL_USB_EPnR_DTOG_TX            (uint32_t)((HAL_USB_EPnR_DTOG_TX_MASK) << (HAL_USB_EPnR_DTOG_TX_OFFSET))

/*
 * Bits 5:4 STAT_TX [1:0]: Status bits, for transmission transfers
 *
 * These bits contain the information about the endpoint status, listed
 * in Table 176. These bits can be toggled by the software to initialize
 * their value. When the application software writes ‘0, the value remains
 * unchanged, while writing ‘1 makes the bit value toggle. Hardware sets
 * the STAT_TX bits to NAK, when a correct transfer has occurred (CTR_TX=1)
 * corresponding to a IN or SETUP (control only) transaction addressed
 * to this endpoint. It then waits for the software to prepare the next
 * set of data to be transmitted.
 *
 * Double-buffered bulk endpoints implement a special transaction flow
 * control, which controls the status based on buffer availability condition
 * (Refer to Section 23.4.3).
 *
 * If the endpoint is defined as Isochronous, its status can only be
 * “VALID” or “DISABLED”. Therefore, the hardware cannot change the
 * status of the endpoint after a successful transaction. If the software
 * sets the STAT_TX bits to ‘STALL’ or ‘NAK’ for an Isochronous endpoint,
 * the USB peripheral behavior is not defined. These bits are read/write
 * but they can be only toggled by writing ‘1.
 */
#define HAL_USB_EPnR_STAT_TX_OFFSET     4U
#define HAL_USB_EPnR_STAT_TX_MASK       0x03U
#define HAL_USB_EPnR_STAT_TX            (uint32_t)((HAL_USB_EPnR_STAT_TX_MASK) << (HAL_USB_EPnR_STAT_TX_OFFSET))

#define HAL_USB_EPnR_STAT_DISABLED      0U
#define HAL_USB_EPnR_STAT_STALL         1U
#define HAL_USB_EPnR_STAT_NAK           2U
#define HAL_USB_EPnR_STAT_VALID         3U

/*
 * Bits 3:0 EA[3:0]: Endpoint address
 *
 * Software must write in this field the 4-bit address used to identify
 * the transactions directed to this endpoint. A value must be written
 * before enabling the corresponding endpoint.
 */
#define HAL_USB_EPnR_EA_OFFSET          0U
#define HAL_USB_EPnR_EA_MASK            0x0FU
#define HAL_USB_EPnR_EA                 (uint32_t)((HAL_USB_EPnR_EA_MASK) << (HAL_USB_EPnR_EA_OFFSET))

/*
 * Table 173. Reception status encoding
 * +--------------+-------------------------------------------------+
 * | STAT_RX[1:0] | Meaning                                         |
 * +--------------+-------------------------------------------------+
 * |      00      | DISABLED: all reception requests addressed to   |
 * |              | this endpoint are ignored.                      |
 * +--------------+-------------------------------------------------+
 * |      01      | STALL: the endpoint is stalled and all reception|
 * |              | requests result in a STALL handshake.           |
 * +--------------+-------------------------------------------------+
 * |      10      | NAK: the endpoint is naked and all reception    |
 * |              | requests result in a NAK handshake.             |
 * +--------------+-------------------------------------------------+
 * |      11      | VALID: this endpoint is enabled for reception.  |
 * |              |                                                 |
 * +--------------+-------------------------------------------------+
 *
 * Table 174. Endpoint type encoding
 * +--------------+-------------------------------------------------+
 * | EP_TYPE[1:0] | Meaning                                         |
 * +--------------+-------------------------------------------------+
 * |      00      | BULK                                            |
 * +--------------+-------------------------------------------------+
 * |      01      | CONTROL                                         |
 * +--------------+-------------------------------------------------+
 * |      10      | ISO                                             |
 * +--------------+-------------------------------------------------+
 * |      11      | INTERRUPT                                       |
 * +--------------+-------------------------------------------------+
 *
 * Table 175. Endpoint kind meaning
 * +----------------+-----------------------------------------------+
 * |  EP_TYPE[1:0]  | Meaning                                       |
 * +----+-----------+-----------------------------------------------+
 * | 00 | BULK      | DBL_BUF                                       |
 * +----+-----------+-----------------------------------------------+
 * | 01 | CONTROL   | STATUS_OUT                                    |
 * +----+-----------+-----------------------------------------------+
 * | 10 | ISO       | Not used                                      |
 * +----+-----------+-----------------------------------------------+
 * | 11 | INTERRUPT | Not used                                      |
 * +----+-----------+-----------------------------------------------+
 *
 * Table 176. Transmission status encoding
 * +--------------+-------------------------------------------------+
 * | STAT_RX[1:0] | Meaning                                         |
 * +--------------+-------------------------------------------------+
 * |      00      | DISABLED: all transmission requests addressed to|
 * |              | this endpoint are ignored.                      |
 * +--------------+-------------------------------------------------+
 * |      01      | STALL: the endpoint is stalled and all transmis-|
 * |              | sion requests result in a STALL handshake.      |
 * +--------------+-------------------------------------------------+
 * |      10      | NAK: the endpoint is naked and all transmission |
 * |              | requests result in a NAK handshake.             |
 * +--------------+-------------------------------------------------+
 * |      11      | VALID: this endpoint is enabled for             |
 * |              | transmission.                                   |
 * +--------------+-------------------------------------------------+
 */

/**
 * Endpoint register
 */
typedef struct hal_epreg {
    volatile uint16_t EPnR;
    volatile uint16_t reserved;
} hal_epreg_t;

/**
 * USB device registers
 */
typedef struct hal_usb {
    hal_epreg_t ep[8];
    volatile uint16_t reserved[16];
    volatile uint16_t CNTR;
    volatile uint16_t reserved0;
    volatile uint16_t ISTR;
    volatile uint16_t reserved1;
    volatile uint16_t FNR;
    volatile uint16_t reserved2;
    volatile uint16_t DADDR;
    volatile uint16_t reserved3;
    volatile uint16_t BTABLE;
    volatile uint16_t reserved4;
} hal_usb_t;

/**
 * Buffer descriptor
 */
typedef struct hal_usb_bufdes {
    volatile uint16_t ADDR_TX;
    volatile uint16_t reserved0;
    volatile uint16_t COUNT_TX;
    volatile uint16_t reserved1;
    volatile uint16_t ADDR_RX;
    volatile uint16_t reserved2;
    volatile uint16_t COUNT_RX;
    volatile uint16_t reserved3;
} hal_usb_bufdes_t;

typedef struct hal_usb_buftbl {
    hal_usb_bufdes_t ep[8];
} hal_usb_buftbl_t;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32F1XX_USB_H */
