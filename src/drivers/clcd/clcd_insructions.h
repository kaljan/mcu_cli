/**
 * @file    clcd_insructions.h
 * @author  Mikalai Naurotski
 * @version 1.0.4
 * @date    01-12-23
 *
 * @brief   Character display instructions
 */
#ifndef CLCD_INSRUCTIONS_H
#define CLCD_INSRUCTIONS_H

#include "clcd_config.h"

/**
 * Clear Display
 *
 * Clear display writes space code 20H (character pattern for character
 * code 20H must be a blank pattern) into all DDRAM addresses. It then
 * sets DDRAM address 0 into the address counter, and returns the display
 * to its original status if it was shifted. In other words, the display
 * disappears and the cursor or blinking goes to the left edge of the
 * display (in the first line if 2 lines are displayed). It also sets
 * I/D to 1 (increment mode) in entry mode. S of entry mode does not
 * change.
 */
#define CLCD_CLEAR                0x01

/**
 * Return Home
 *
 * Return home sets DDRAM address 0 into the address counter, and
 * returns the display to its original status if it was shifted. The
 * DDRAM contents do not change.
 *
 * The cursor or blinking go to the left edge of the display (in the
 * first line if 2 lines are displayed).
 */
#define CLCD_RET_HOME             0x02

/**
 * Entry Mode Set
 */
#define CLCD_EM_SET               0x04

/**
 * I/D: Increments (I/D = 1) or decrements (I/D = 0) the DDRAM address
 * by 1 when a character code is written into or read from DDRAM.
 *
 * The cursor or blinking moves to the right when incremented by 1 and
 * to the left when decremented by 1. The same applies to writing and
 * reading of CGRAM.
 */
#define CLCD_EM_SET_ID            0x02

/**
 * S: Shifts the entire display either to the right (I/D = 0) or to
 * the left (I/D = 1) when S is 1. The display does not shift if S is 0.
 *
 * If S is 1, it will seem as if the cursor does not move but the display
 * does. The display does not shift when reading from DDRAM. Also,
 * writing into or reading out from CGRAM does not shift the display.
 */
#define CLCD_EM_SET_S             0x01

/**
 * Display On/Off Control
 */
#define CLCD_ON_OFF_CTL           0x08

/**
 * D: The display is on when D is 1 and off when D is 0. When off, the
 * display data remains in DDRAM, but can be displayed instantly by
 * setting D to 1.
 */
#define CLCD_ON_OFF_CTL_D         0x04

/**
 * C: The cursor is displayed when C is 1 and not displayed when C is 0.
 * Even if the cursor disappears, the function of I/D or other specifications
 * will not change during display data write. The cursor is displayed
 * using 5 dots in the 8th line for 5 × 8 dot character font selection
 * and in the 11th line for the 5 × 10 dot character font selection.
 */
#define CLCD_ON_OFF_CTL_C         0x02

/**
 * B: The character indicated by the cursor blinks when B is 1. The
 * blinking is displayed as switching between all blank dots and
 * displayed characters at a speed of 409.6-ms intervals when f_{cp} or
 * f_{OSC} is 250 kHz. The cursor and blinking can be set to display
 * simultaneously. (The blinking frequency changes according to f OSC
 * or the reciprocal of f_{cp} . For example, when f_{cp} is 270 kHz,
 * 409.6 × 250/270 = 379.2 ms.)
 */
#define CLCD_ON_OFF_CTL_B         0x01

/**
 * Cursor or Display Shift
 *
 * Cursor or display shift shifts the cursor position or display to the
 * right or left without writing or reading display data.
 * This function is used to correct or search the display. In a 2-line
 * display, the cursor moves to the second line when it passes the 40th
 * digit of the first line. Note that the first and second line displays
 * will shift at the same time.
 *
 * When the displayed data is shifted repeatedly each line moves only
 * horizontally. The second line display does not shift into the first
 * line position.
 *
 * The address counter (AC) contents will not change if the only action
 * performed is a display shift.
 *
 * +-----+-----+-------------------------------------------------------------------------------+
 * | S/C | R/L | Shift Function                                                                |
 * +-----+-----+-------------------------------------------------------------------------------+
 * |  0  |  0  | Shifts the cursor position to the left. (AC is decremented by one.)           |
 * +-----+-----+-------------------------------------------------------------------------------+
 * |  0  |  1  | Shifts the cursor position to the right. (AC is incremented by one.)          |
 * +-----+-----+-------------------------------------------------------------------------------+
 * |  1  |  0  | Shifts the entire display to the left. The cursor follows the display shift.  |
 * +-----+-----+-------------------------------------------------------------------------------+
 * |  1  |  1  | Shifts the entire display to the right. The cursor follows the display shift. |
 * +-----+-----+-------------------------------------------------------------------------------+
 */
#define CLCD_CDS                  0x10

#define CLCD_CDS_S_C              0x04
#define CLCD_CDS_R_L              0x02

/**
 * Function Set
 *
 * +---+---+---------+----------------+--------+--------------------------------------------------------+
 * |   |   | No. of  |                |        |                                                        |
 * |   |   | Display |                | Duty   |                                                        |
 * | N | F | Lines   | Character Font | Factor | Remarks                                                |
 * +---+---+---------+----------------+--------+--------------------------------------------------------+
 * | 0 | 0 |    1    | 5 × 8 dots     | 1/8    |                                                        |
 * +---+---+---------+----------------+--------+--------------------------------------------------------+
 * | 0 | 1 |    1    | 5 × 10 dots    | 1/11   |                                                        |
 * +---+---+---------+----------------+--------+--------------------------------------------------------+
 * | 1 | * |    2    | 5 × 8 dots     | 1/16   | Cannot display two lines for 5 × 10 dot character font |
 * +---+---+---------+----------------+--------+--------------------------------------------------------+
 *
 * Note: * Indicates don’t care.
 */
#define CLCD_FN_SET               0x20

/**
 * DL: Sets the interface data length. Data is sent or received in 8-bit
 * lengths (DB7 to DB0) when DL is 1, and in 4-bit lengths (DB7 to DB4)
 * when DL is 0.When 4-bit length is selected, data must be sent or
 * received twice.
 */
#define CLCD_FN_SET_DL            0x10

/**
 * N: Sets the number of display lines.
 */
#define CLCD_FN_SET_N             0x08

/**
 * F: Sets the character font.
 *
 * Note: Perform the function at the head of the program before executing
 *       any instructions (except for the read busy flag and address
 *       instruction). From this point, the function set instruction
 *       cannot be executed unless the interface data length is changed.
 */
#define CLCD_FN_SET_F             0x04

/**
 * Set CGRAM Address
 *
 * Set CGRAM address sets the CGRAM address binary AAAAAA into the
 * address counter.
 *
 * Data is then written to or read from the MPU for CGRAM.
 */

#define CLCD_CGRAM_ADDR           0x40

/**
 * Set DDRAM Address
 *
 * Set DDRAM address sets the DDRAM address binary AAAAAAA into the
 * address counter.
 *
 * Data is then written to or read from the MPU for DDRAM.
 *
 * However, when N is 0 (1-line display), AAAAAAA can be 00H to 4FH.
 * When N is 1 (2-line display), AAAAAAA can be 00H to 27H for the first
 * line, and 40H to 67H for the second line.
 */
#define CLCD_DDRAM_ADDR           0x80

/**
 * Read Busy Flag and Address
 *
 * Read busy flag and address reads the busy flag (BF) indicating that
 * the system is now internally operating on a previously received
 * instruction. If BF is 1, the internal operation is in progress.
 * The next instruction will not be accepted until BF is reset to 0.
 * Check the BF status before the next write operation. At the same
 * time, the value of the address counter in binary AAAAAAA is read out.
 * This address counter is used by both CG and DDRAM addresses, and its
 * value is determined by the previous instruction. The address contents
 * are the same as for instructions set CGRAM address and set DDRAM address.
 */
#define CLCD_BUSY_FLAG            0x80

#if (CLCD_IC_TYPE == CLCD_IC_RS0010)

/**
 * G/C: GRAPHIC MODE / CHARACTER MODE SELECTION
 * This bit is used to select the display mode for further process.
 * When G/C = 1, the GRAPHIC MODE will be selected.
 * When G/C = 0, the CHARACTER MODE will be selected.
 *
 * PWR: ENABLE/DISABLE INTERNAL POWER
 * This bit is used to turn ON or turn OFF the internal power.
 * When PWR = 1, the internal power is turned ON.
 * When PWR = 0, the internal power is turned OFF.
 */
#define RS0010_CDS_MP       0x03
#define RS0010_CDS_MP_G_C   0x08
#define RS0010_CDS_MP_PWR   0x04

/**
 * FT1, FT0: FONT TABLE SELECTION
 * These two bits are used to select one font table out of the three
 * for further process.
 *
 * +-----+-----+------------------------------------------+
 * | FT1 | FT0 | Character font table                     |
 * +-----+-----+------------------------------------------+
 * |  0  |  0  | ENGLISH_JAPANESE CHARACTER FONT TABLE    |
 * +-----+-----+------------------------------------------+
 * |  0  |  1  | WESTERN EUROPEAN CHARACTER FONT TABLE-I  |
 * +-----+-----+------------------------------------------+
 * |  1  |  0  | ENGLISH_RUSSIAN CHARACTER FONT TABLE     |
 * +-----+-----+------------------------------------------+
 * |  1  |  1  | WESTERN EUROPEAN CHARACTER FONT TABLE-II |
 * +-----+-----+------------------------------------------+
 *
 * Note: The default setting for FT1 and FT0 is 0 and 0 respectively
 *       which means the default Font Table is ENGLISH_JAPANESE CHARACTER FONT TABLE.
 */
#define RS0010_FN_SET_FT1   0x02
#define RS0010_FN_SET_FT0   0x01
#endif

#endif /* CLCD_INSRUCTIONS_H */
/* EOF */
