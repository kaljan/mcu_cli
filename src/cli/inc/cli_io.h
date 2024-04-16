/**
 * @file    cli_io.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-13
 *
 * @brief
 */
#ifndef CLI_IO_H
#define CLI_IO_H


/* Control characters */

#define CLI_CC_NUL              0x00 /* [ CTRL+@ ] '\0' NULL */
#define CLI_CC_SOH              0x01 /* [ CTRL+A ] START OF HEADING */
#define CLI_CC_STX              0x02 /* [ CTRL+B ] START OF TEXT */
#define CLI_CC_ETX              0x03 /* [ CTRL+C ] END OF TEXT */
#define CLI_CC_EOT              0x04 /* [ CTRL+D ] END OF TRANSMISSION */
#define CLI_CC_ENQ              0x05 /* [ CTRL+E ] ENQUIRY */
#define CLI_CC_ACK              0x06 /* [ CTRL+F ] ACKNOWLEDGE */
#define CLI_CC_BEL              0x07 /* [ CTRL+G ] '\a' BELL */
#define CLI_CC_BS               0x08 /* [ CTRL+H ] '\b' BACKSPACE */
#define CLI_CC_TAB              0x09 /* [ CTRL+I ] '\t' CHARACTER TABULATION (horizontal tabulation) */
#define CLI_CC_LF               0x0A /* [ CTRL+J ] '\n' LINE FEED*/
#define CLI_CC_VT               0x0B /* [ CTRL+K ] '\v' LINE TABULATION (vertical tabulation) */
#define CLI_CC_FF               0x0C /* [ CTRL+L ] '\f' FORM FEED */
#define CLI_CC_CR               0x0D /* [ CTRL+M ] '\r' CARRIAGE RETURN */
#define CLI_CC_SO               0x0E /* [ CTRL+N ] SHIFT OUT (locking-shift one) */
#define CLI_CC_SI               0x0F /* [ CTRL+O ] SHIFT IN (locking-shift zero) */
#define CLI_CC_DLE              0x10 /* [ CTRL+P ] DATA LINK ESCAPE */
#define CLI_CC_DC1              0x11 /* [ CTRL+Q ] DEVICE CONTROL ONE */
#define CLI_CC_DC2              0x12 /* [ CTRL+R ] DEVICE CONTROL TWO */
#define CLI_CC_DC3              0x13 /* [ CTRL+S ] DEVICE CONTROL THREE */
#define CLI_CC_DC4              0x14 /* [ CTRL+T ] DEVICE CONTROL FOUR */
#define CLI_CC_NAK              0x15 /* [ CTRL+U ] NEGATIVE ACKNOWLEDGE */
#define CLI_CC_SYN              0x16 /* [ CTRL+V ] SYNCHRONOUS IDLE */
#define CLI_CC_ETB              0x17 /* [ CTRL+W ] END OF TRANSMISSION BLOCK */
#define CLI_CC_CAN              0x18 /* [ CTRL+X ] CANCEL */
#define CLI_CC_EM               0x19 /* [ CTRL+Y ] END OF MEDIUM */
#define CLI_CC_SUB              0x1A /* [ CTRL+Z ] SUBSTITUTE */
#define CLI_CC_ESC              0x1B /* [ CTRL+[ ] `\e` ESCAPE */
#define CLI_CC_FS               0x1C /* [ CTRL+\ ] INFORMATION SEPARATOR FOUR (file separator) */
#define CLI_CC_GS               0x1D /* [ CTRL+] ] INFORMATION SEPARATOR THREE (group separator) */
#define CLI_CC_RS               0x1E /* [ CTRL+^ ] INFORMATION SEPARATOR TWO (record separator) */
#define CLI_CC_US               0x1F /* [ CTRL+_ ] INFORMATION SEPARATOR ONE (unit separator) */
#define CLI_CC_DEL              0x7F /* [ CTRL+? ] DELETE */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_IO_H */
