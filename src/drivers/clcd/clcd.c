/**
 * @file    clcd.c
 * @author  Mikalai Naurotski
 * @version 1.0.5
 * @date    29-01-20
 *
 * @brief   HD44780 driver
 */

#include "clcd.h"
#include "clcd_insructions.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/** Text format buffer size */
#define CLCD_FMT_BUF_SIZE         CLCD_WIDTH * CLCD_LINES

/** Second line bit for DDRAM address */
#define CLCD_SECOND_LINE          0x40

#define CLCD_INIT_FLAG          0x01
#define CLCD_DISPLAY_ON         0x02
#define CLCD_CURSOR_ON          0x04
#define CLCD_BLINK_ON           0x08

static uint8_t m_clcd_flags = 0;

static void clcd_ru_utf8_convert(char* dst, uint32_t dst_size, const char* src, uint32_t src_size);

#if (HAL_CLCD_PWREN == HAL_ENABLED)
extern hal_object_t hal_clcd_pwren_get_pin(void);
static hal_object_t pwren_pin = NULL;

static void clcd_pwren_init(void) {
    pwren_pin = hal_clcd_pwren_get_pin();
    hal_gpio_pin_set_dir(pwren_pin, HAL_GPIO_DIR_OUT);
    hal_gpio_pin_reset(pwren_pin);
}

void clcd_power_enable(void) {
    hal_gpio_pin_set(pwren_pin);
}

void clcd_power_disable(void) {
    hal_gpio_pin_reset(pwren_pin);
    m_clcd_flags = 0;
}

#endif

/**
 * @brief
 * TODO rework for write only mode
 */
#if (HAL_MC6800_READ == HAL_ENABLED)
void clcd_wait_busy(void) {
    uint8_t status = 0;
    do {
        status = clcd_port_read(CLCD_INSTRUCTION);
    } while (status & CLCD_BUSY_FLAG);
}
#endif
/**
 * @brief
 *
 */
void clcd_init(void) {
    uint8_t fnSetValue = CLCD_FN_SET;

    if ((m_clcd_flags & CLCD_INIT_FLAG) == CLCD_INIT_FLAG) {
        return;
    }

#if (HAL_CLCD_PWREN == HAL_ENABLED)
    clcd_pwren_init();
    clcd_delay_us(CLCD_POWER_ON_DELAY_US);
    clcd_power_enable();
#endif

    m_clcd_flags = CLCD_INIT_FLAG | CLCD_DISPLAY_ON;

    clcd_port_init();

    clcd_delay_us(CLCD_POWER_ON_DELAY_US);

#if (MC6800_DATA_LEN == MC6800_DATA_LEN_4BITS)
# if (CLCD_IC_TYPE == CLCD_IC_RS0010)
    /* Note: This initialization sequence for 4 bit mode does not works
     * on SC1602BULT-SH-HS-G, WH1602D-TMI-CT# (Maybe this sequence does
     * not work on other displays like this). This sequence is wor only
     * with REC001602CBPP5N00000 display.
     */
    clcd_port_half_byte_write(CLCD_INSTRUCTION, 0x02);
# elif ((CLCD_IC_TYPE == CLCD_IC_KS0066) || (CLCD_IC_TYPE == CLCD_IC_HD44780))
    /* Note: After researching this issue i found in hd44780 datasheet
     * another sequence that works fine for SC1602BULT-SH-HS-G, WH1602D-TMI-CT#
     */
    clcd_port_half_byte_write(CLCD_INSTRUCTION, 0x03);
    clcd_delay_us(CLCD_CMD_INIT_DELAY);

    clcd_port_half_byte_write(CLCD_INSTRUCTION, 0x03);
    clcd_delay_us(CLCD_CMD_DELAY_US * 2);

    clcd_port_half_byte_write(CLCD_INSTRUCTION, 0x03);
    clcd_delay_us(CLCD_CMD_DELAY_US);

    clcd_port_half_byte_write(CLCD_INSTRUCTION, 0x02);
    clcd_delay_us(CLCD_CMD_DELAY_US);
# endif
#else
    fnSetValue |= CLCD_FN_SET_DL;
#endif

#if (CLCD_FONT == CLCD_FONT_5x10)
    fnSetValue |= CLCD_FN_SET_F;
#endif

#if (CLCD_LINES >= 2)
    fnSetValue |= CLCD_FN_SET_N;
#endif

#if (CLCD_IC_TYPE == CLCD_IC_RS0010)
    fnSetValue |= (CLCD_FONT_TABLE & RS0010_FONT_TABLE_MASK);
#endif

#if ((MC6800_DATA_LEN == MC6800_DATA_LEN_4BITS) && \
    ((CLCD_IC_TYPE == CLCD_IC_KS0066) || \
    (CLCD_IC_TYPE == CLCD_IC_HD44780)))

    clcd_port_write(CLCD_INSTRUCTION, fnSetValue);
    clcd_delay_us(CLCD_CMD_DELAY_US);

    clcd_port_write(CLCD_INSTRUCTION, CLCD_ON_OFF_CTL);
    clcd_delay_us(CLCD_CMD_DELAY_US);

    clcd_port_write(CLCD_INSTRUCTION, CLCD_CLEAR);
    clcd_delay_us(CLCD_CLEAR_DELAY_US);

    clcd_port_write(CLCD_INSTRUCTION, CLCD_EM_SET | CLCD_EM_SET_ID);
    clcd_delay_us(CLCD_CMD_DELAY_US);

    clcd_port_write(CLCD_INSTRUCTION, CLCD_ON_OFF_CTL | CLCD_ON_OFF_CTL_D);
    clcd_delay_us(CLCD_CMD_DELAY_US);
#else
    clcd_port_write(CLCD_INSTRUCTION, fnSetValue);
    clcd_wait_busy();

#if (CLCD_IC_TYPE == CLCD_IC_RS0010)
    clcd_port_write(CLCD_INSTRUCTION, CLCD_CDS | RS0010_CDS_MP | RS0010_CDS_MP_PWR);
    clcd_wait_busy();
#endif

    clcd_port_write(CLCD_INSTRUCTION, CLCD_ON_OFF_CTL | CLCD_ON_OFF_CTL_D);
    clcd_wait_busy();

    clcd_port_write(CLCD_INSTRUCTION, CLCD_CLEAR);
    clcd_wait_busy();

    clcd_port_write(CLCD_INSTRUCTION, CLCD_EM_SET | CLCD_EM_SET_ID);
    clcd_wait_busy();
#endif
}

/**
 * @brief
 *
 */
void clcd_clear(void) {
    clcd_port_write(CLCD_INSTRUCTION, CLCD_CLEAR);
#if (HAL_MC6800_READ != HAL_ENABLED)
    clcd_delay_us(CLCD_CLEAR_DELAY_US);
#else
    clcd_wait_busy();
#endif
}

/**
 * @brief
 *
 * @param is_visible
 */
void clcd_set_cursor_visible(uint8_t is_visible) {
    if ((m_clcd_flags & CLCD_DISPLAY_ON) == CLCD_DISPLAY_ON) {
        uint8_t on_off_ctl = CLCD_ON_OFF_CTL | CLCD_ON_OFF_CTL_D;

        if (is_visible != 0) {
            on_off_ctl |= CLCD_ON_OFF_CTL_C;
        }

        if (m_clcd_flags & CLCD_BLINK_ON) {
            on_off_ctl |= CLCD_ON_OFF_CTL_B;
        }

        clcd_port_write(CLCD_INSTRUCTION, on_off_ctl);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif
    }
}

/**
 * @brief
 *
 * @param is_blink
 */
void clcd_enable_cursor_blink(uint8_t is_blink) {
    if ((m_clcd_flags & CLCD_DISPLAY_ON) == CLCD_DISPLAY_ON) {
        uint8_t on_off_ctl = CLCD_ON_OFF_CTL | CLCD_ON_OFF_CTL_D;

        if (is_blink != 0) {
            on_off_ctl |= CLCD_ON_OFF_CTL_B;
        }

        if (m_clcd_flags & CLCD_CURSOR_ON) {
            on_off_ctl |= CLCD_ON_OFF_CTL_C;
        }

        clcd_port_write(CLCD_INSTRUCTION, on_off_ctl);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif
    }
}

/**
 * @brief
 *
 * @param position
 */
void clcd_set_cursor_position(uint8_t position) {
    if ((m_clcd_flags & CLCD_DISPLAY_ON) == CLCD_DISPLAY_ON) {
        clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR + position);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif
    }
}

/**
 * @brief
 *
 * @param c
 */
void clcd_set_char(uint8_t c) {
    if ((m_clcd_flags & CLCD_DISPLAY_ON) == CLCD_DISPLAY_ON) {
        clcd_port_write(CLCD_DATA, c);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif
    }
}

/**
 * @brief
 *
 * @param addr
 * @param c
 */
void clcd_write_char(uint8_t addr, char c) {
    clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR + addr);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif

    clcd_port_write(CLCD_DATA, c);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif
}

/**
 * @brief
 *
 * @param addr
 * @param text
 * @param size
 */
#if ((CLCD_IC_TYPE == CLCD_IC_RS0010) && (CLCD_FONT_TABLE == RS0010_ENGLISH_RUSSIAN_FONT_TABLE)) || \
    ((CLCD_IC_TYPE == CLCD_IC_KS0066) && (CLCD_FONT_TABLE == CLCD_IC_KS0066_CYRILLIC))
void clcd_write_text(uint8_t addr, const char* text, uint32_t size) {
    uint8_t curr_addr = addr;
    char out_str[CLCD_FMT_BUF_SIZE + 1];

    clcd_ru_utf8_convert(out_str, CLCD_FMT_BUF_SIZE, text, size);

    clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR + addr);
#if (HAL_MC6800_READ != HAL_ENABLED)
    clcd_delay_us(CLCD_CMD_DELAY_US);
#else
    clcd_wait_busy();
#endif

    for (size_t index = 0; index < size; index++) {
        if ((0 == out_str[index]) || ((curr_addr & 0x3F) >= CLCD_WIDTH)) {
            break;
        } else if (out_str[index] == '\n') {
            if ((curr_addr & CLCD_SECOND_LINE) == CLCD_SECOND_LINE) {
                break;
            }

            clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR + CLCD_SECOND_LINE);
#if (HAL_MC6800_READ != HAL_ENABLED)
            clcd_delay_us(CLCD_CMD_DELAY_US);
#else
            clcd_wait_busy();
#endif

            curr_addr = CLCD_SECOND_LINE;
            continue;
        }

        clcd_port_write(CLCD_DATA, out_str[index]);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif

        curr_addr++;
    }
}

void clcd_over_write_text(uint8_t addr, const char* text, uint32_t size) {
    uint8_t curr_addr = addr;
    char out_str[CLCD_FMT_BUF_SIZE + 1];

    clcd_ru_utf8_convert(out_str, CLCD_FMT_BUF_SIZE, text, size);

    clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR + addr);
#if (HAL_MC6800_READ != HAL_ENABLED)
    clcd_delay_us(CLCD_CMD_DELAY_US);
#else
    clcd_wait_busy();
#endif

    for (uint32_t index = 0; index < size; index++) {
        if ((0 == out_str[index]) || ((curr_addr & 0x3F) >= CLCD_WIDTH)) {
            break;
        } else if ((out_str[index] == '\n')) {
            for (uint32_t j = 0; j < (size - index); j++) {
                clcd_port_write(CLCD_DATA, ' ');
#if (HAL_MC6800_READ != HAL_ENABLED)
                clcd_delay_us(CLCD_CMD_DELAY_US);
#else
                clcd_wait_busy();
#endif
            }
            if ((curr_addr & CLCD_SECOND_LINE) == CLCD_SECOND_LINE) {
                break;
            }

            clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR + CLCD_SECOND_LINE);
#if (HAL_MC6800_READ != HAL_ENABLED)
            clcd_delay_us(CLCD_CMD_DELAY_US);
#else
            clcd_wait_busy();
#endif

            curr_addr = CLCD_SECOND_LINE;
            continue;
        } else if (out_str[index] == 0) {
            for (uint32_t j = 0; j < (size - index); j++) {
                clcd_port_write(CLCD_DATA, ' ');
#if (HAL_MC6800_READ != HAL_ENABLED)
                clcd_delay_us(CLCD_CMD_DELAY_US);
#else
                clcd_wait_busy();
#endif
            }
            break;
        }

        clcd_port_write(CLCD_DATA, out_str[index]);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif

        curr_addr++;
    }
}

#else
void clcd_write_text(uint8_t addr, const char* text, uint32_t size) {
    uint8_t curr_addr = addr;

    clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR + addr);
#if (HAL_MC6800_READ != HAL_ENABLED)
    clcd_delay_us(CLCD_CMD_DELAY_US);
#else
    clcd_wait_busy();
#endif

    for (size_t index = 0; index < size; index++) {
        if (!text[index] || ((curr_addr & 0x0F) >= CLCD_WIDTH)) {
            break;
        } else if (text[index] == '\n') {
            if ((curr_addr & CLCD_SECOND_LINE) == CLCD_SECOND_LINE) {
                break;
            }

            clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR + CLCD_SECOND_LINE);
#if (HAL_MC6800_READ != HAL_ENABLED)
            clcd_delay_us(CLCD_CMD_DELAY_US);
#else
            clcd_wait_busy();
#endif

            curr_addr = CLCD_SECOND_LINE;
            continue;
        }

        clcd_port_write(CLCD_DATA, text[index]);
#if (HAL_MC6800_READ != HAL_ENABLED)
        clcd_delay_us(CLCD_CMD_DELAY_US);
#else
        clcd_wait_busy();
#endif

        curr_addr++;
    }
}
#endif

/**
 * @brief
 *
 * @param start
 * @param fmt
 * @param ...
 */
void clcd_print(uint8_t start, const char* fmt, ...) {
    static char m_clcd_fmt_buf[(CLCD_FMT_BUF_SIZE * 2) + 1];

	va_list vaptr;
	va_start(vaptr, fmt);
    memset(m_clcd_fmt_buf, 0, CLCD_FMT_BUF_SIZE);
	vsnprintf(m_clcd_fmt_buf, CLCD_FMT_BUF_SIZE, fmt, vaptr);
	va_end(vaptr);

    clcd_write_text(start, m_clcd_fmt_buf, strlen(m_clcd_fmt_buf));
}

/**
 * @brief
 *
 * @param start
 * @param fmt
 * @param ...
 */
void clcd_over_print(uint8_t start, uint32_t maxLen, const char* fmt, ...) {
    static char m_clcd_fmt_buf[(CLCD_FMT_BUF_SIZE * 2) + 1];

	va_list vaptr;
	va_start(vaptr, fmt);
    memset(m_clcd_fmt_buf, 0, CLCD_FMT_BUF_SIZE);
	vsnprintf(m_clcd_fmt_buf, CLCD_FMT_BUF_SIZE, fmt, vaptr);
	va_end(vaptr);

    clcd_over_write_text(start, m_clcd_fmt_buf, maxLen);
}

/**
 * @brief
 *
 * @param index
 * @param data
 * @param size
 */
void clcd_write_custom_char(uint8_t index, const uint8_t* data, uint32_t size) {
    if (size < 8) {
        return;
    }

    clcd_port_write(CLCD_INSTRUCTION, CLCD_CGRAM_ADDR | ((index & 0x07) << 3));
#if (HAL_MC6800_READ != HAL_ENABLED)
    clcd_delay_us(CLCD_CMD_DELAY_US);
#else
    clcd_wait_busy();
#endif

    for (uint8_t index = 0; index < 8; index++) {
        clcd_port_write(CLCD_DATA, data[index]);
#if (HAL_MC6800_READ != HAL_ENABLED)
    clcd_delay_us(CLCD_CMD_DELAY_US);
#else
    clcd_wait_busy();
    clcd_delay_us(4);
#endif
    }

    clcd_port_write(CLCD_INSTRUCTION, CLCD_DDRAM_ADDR);
#if (HAL_MC6800_READ != HAL_ENABLED)
    clcd_delay_us(CLCD_CMD_DELAY_US);
#else
    clcd_wait_busy();
#endif
}

#if ((CLCD_IC_TYPE == CLCD_IC_RS0010) && (CLCD_FONT_TABLE == RS0010_ENGLISH_RUSSIAN_FONT_TABLE)) || \
    ((CLCD_IC_TYPE == CLCD_IC_KS0066) && (CLCD_FONT_TABLE == CLCD_IC_KS0066_CYRILLIC))

#define CLCD_CYRILLIC_TABLE_SIZE      64

static const char m_clcd_cyrillic_table[CLCD_CYRILLIC_TABLE_SIZE] = {
    0x41, /* А */
    0xA0, /* Б */
    0x42, /* В */
    0xA1, /* Г */
    0xE0, /* Д */
    0x45, /* Е */
    0xA3, /* Ж */
    0xA4, /* З */
    0xA5, /* И */
    0xA6, /* Й */
    0x4B, /* К */
    0xA7, /* Л */
    0x4D, /* М */
    0x48, /* Н */
    0x4F, /* О */
    0xA8, /* П */
    0x50, /* Р */
    0x43, /* С */
    0x54, /* Т */
    0xA9, /* У */
    0xAA, /* Ф */
    0x58, /* Х */
    0xE1, /* Ц */
    0xAB, /* Ч */
    0xAC, /* Ш */
    0xE2, /* Щ */
    0xAD, /* Ъ */
    0xAE, /* Ы */
    0x62, /* Ь */
    0xAF, /* Э */
    0xB0, /* Ю */
    0xB1, /* Я */
    0x61, /* а */
    0xB2, /* б */
    0xB3, /* в */
    0xB4, /* г */
    0xE3, /* д */
    0x65, /* е */
    0xB6, /* ж */
    0xB7, /* з */
    0xB8, /* и */
    0xB9, /* й */
    0xBA, /* к */
    0xBB, /* л */
    0xBC, /* м */
    0xBD, /* н */
    0x6F, /* о */
    0xBE, /* п */
    0x70, /* р */
    0x63, /* с */
    0xBF, /* т */
    0x79, /* у */
    0xE4, /* ф */
    0x78, /* х */
    0xE5, /* ц */
    0xC0, /* ч */
    0xC1, /* ш */
    0xE6, /* щ */
    0xC2, /* ъ */
    0xC3, /* ы */
    0xC4, /* ь */
    0xC5, /* э */
    0xC6, /* ю */
    0xC7, /* я */
};

#if (CLCD_IC_TYPE == CLCD_IC_RS0010) && (CLCD_FONT_TABLE == RS0010_ENGLISH_RUSSIAN_FONT_TABLE)
static inline char clcd_ru_utf8_d0_tochar(uint8_t c) {
    switch(c) {
    case 0x80: return 0x18; /* 'Ѐ' */
    case 0x81: return 0x1B; /* 'Ё' */
    case 0x86: return 0x49; /* 'І' */
    case 0x87: return 0x1F; /* 'Ї' */
    default:
        break;
    }
    return '?';
}

static inline char clcd_ru_utf8_d1_tochar(uint8_t c) {
    switch(c) {
    case 0x90: return 0x98; /* 'ѐ' */
    case 0x91: return 0xB5; /* 'ё' */
    case 0x96: return 0x69; /* 'і' */
    case 0x97: return 0x9F; /* 'ї' */
    default:
        break;
    }
    return '?';
}
#elif (CLCD_IC_TYPE == CLCD_IC_KS0066) && (CLCD_FONT_TABLE == CLCD_IC_KS0066_CYRILLIC)
static inline char clcd_ru_utf8_d0_tochar(uint8_t c) {
    switch(c) {
    case 0x81: return 0xA2; /* 'Ё' */
    case 0x86: return 0x49; /* 'І' */
    default:
        break;
    }
    return '?';
}

static inline char clcd_ru_utf8_d1_tochar(uint8_t c) {
    switch(c) {
    case 0x91: return 0xB5; /* 'ё' */
    case 0x96: return 0x69; /* 'і' */
    default:
        break;
    }
    return '?';
}
#endif

static void clcd_ru_utf8_convert(char* dst, uint32_t dst_size, const char* src, uint32_t src_size) {
    uint32_t dst_index = 0;
    uint32_t src_index = 0;
    while ((dst_index < dst_size) && (src_index < src_size)) {

        if (src[src_index] == 0) {
            break;
        } else if (src[src_index] == 0xD0) {
            src_index++;
            if (src[src_index] >= 0x90 && src[src_index] <= 0xBF) {
                dst[dst_index] = m_clcd_cyrillic_table[src[src_index] - 0x90];
            } else {
                dst[dst_index] = clcd_ru_utf8_d0_tochar(src[src_index]);;
            }
            dst_index++;
            src_index++;
        } else if (src[src_index] == 0xD1) {
            src_index++;
            if (src[src_index] >= 0x80 && src[src_index] <= 0x8F) {
                dst[dst_index] = m_clcd_cyrillic_table[src[src_index] - 80];
            } else {
                dst[dst_index] = clcd_ru_utf8_d1_tochar(src[src_index]);
            }
            dst_index++;
            src_index++;
        } else {
            dst[dst_index] = src[src_index];
            dst_index++;
            src_index++;
        }

        if (dst_index < dst_size) {
            dst[dst_index] = 0;
        }
    }
}
#endif
