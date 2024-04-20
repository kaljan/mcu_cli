/**
 * @file    cli_gpio.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-02
 *
 * @brief
 */

#include "stm32f1xx_gpio.h"
#include "hal_gpio.h"
#include "cli.h"


#define DO_NOT_USE      0   /* Do not use */
#define IN_EN      0x01 /* Input configuration enabled */
#define OUT_EN     0x02 /* Output configuration enabled */

static const char* cli_gpio_undefined = "undefined";

typedef struct cli_gpio_pin {
    int index;
    int pin_no;
    int flags;
    const char* label;
} cli_gpio_pin_t;

typedef struct cli_gpio_port {
    void* port;
    int pin_count;
    cli_gpio_pin_t* pin_list;
} cli_gpio_port_t;

static cli_gpio_pin_t gpio_a_pin_list[16] = {
    {0,  10, IN_EN | OUT_EN, "unused"},
    {1,  11, IN_EN | OUT_EN, "unused"},
    {2,  12, DO_NOT_USE,     "USART2_TX"},
    {3,  13, DO_NOT_USE,     "USART2_RX"},
    {4,  14, IN_EN | OUT_EN, "unused"},
    {5,  15, IN_EN | OUT_EN, "unused"},
    {6,  16, IN_EN | OUT_EN, "unused"},
    {7,  17, IN_EN | OUT_EN, "unused"},
    {8,  29, DO_NOT_USE,     "LCD_RS"},
    {9,  30, DO_NOT_USE,     "LCD_E"},
    {10, 31, IN_EN | OUT_EN, "unused"},
    {11, 32, DO_NOT_USE,     "USB_DM"},
    {12, 33, DO_NOT_USE,     "USB_DP"},
    {13, 34, DO_NOT_USE,     "SWDIO"},
    {14, 37, DO_NOT_USE,     "SWCLK"},
    {15, 38, IN_EN | OUT_EN, "unused"},
};


static cli_gpio_pin_t gpio_b_pin_list[16] = {
    {0,  18, IN_EN | OUT_EN, "unused"},
    {1,  19, IN_EN | OUT_EN, "unused"},
    {2,  20, DO_NOT_USE,     "BOOT1"},
    {3,  39, DO_NOT_USE,     "LCD_D5"},
    {4,  40, DO_NOT_USE,     "LCD_D7"},
    {5,  41, DO_NOT_USE,     "LCD_D6"},
    {6,  42, DO_NOT_USE,     "LCD_D4"},
    {7,  43, DO_NOT_USE,     "RCU_IN"},
    {8,  46, IN_EN | OUT_EN, "unused"},
    {9,  46, IN_EN | OUT_EN, "unused"},
    {10, 21, IN_EN | OUT_EN, "unused"},
    {11, 22, IN_EN | OUT_EN, "unused"},
    {12, 25, IN_EN | OUT_EN, "unused"},
    {13, 26, IN_EN | OUT_EN, "unused"},
    {14, 27, IN_EN | OUT_EN, "unused"},
    {15, 28, IN_EN | OUT_EN, "unused"},
};


static cli_gpio_pin_t gpio_c_pin_list[3] = {
    {13,  2, OUT_EN,         "LED"},
    {14,  3, DO_NOT_USE,     "OSC32I"},
    {15,  4, DO_NOT_USE,     "OSC32O"},
};


static cli_gpio_pin_t gpio_d_pin_list[2] = {
    {0,   5, DO_NOT_USE,     "OSCI"},
    {1,   6, DO_NOT_USE,     "OSCO"},
};

#define CLI_GPIO_LIST_SIZE      4

static cli_gpio_port_t gpio_list[CLI_GPIO_LIST_SIZE] = {
    {GPIOA, 16, gpio_a_pin_list},
    {GPIOB, 16, gpio_b_pin_list},
    {GPIOC, 3, gpio_c_pin_list},
    {GPIOC, 2, gpio_d_pin_list},
};

static cli_gpio_port_t* cli_gpio_port_get(char c) {
    cli_gpio_port_t* ret = NULL;
    if (c >= 'A') {
        if (c >= 'a') {
            c -= 'a';
        } else {
            c -= 'A';
        }
        if (c < CLI_GPIO_LIST_SIZE) {
            ret = &gpio_list[(uint32_t)c];
        }
    }
    return ret;
}

static cli_gpio_pin_t* cli_gpio_pin_get(GPIO_Pin_t* dst, char c, int n) {
    cli_gpio_pin_t* ret = NULL;
    if (NULL != dst) {
        cli_gpio_port_t* p = cli_gpio_port_get(c);
        if ((NULL != p) && (NULL != p->pin_list)) {
            for (int i = 0; i < p->pin_count; i++) {
                if (p->pin_list[i].index == n) {
                    dst->port = p->port;
                    dst->pin = n;
                    ret = &p->pin_list[i];
                    break;
                }
            }
        }
    }
    return ret;
}

static const char* gpio_mode_str(uint32_t mode) {
    switch (mode) {
    case GPIO_PIN_MODE_ANALOG: return    "AIN";
    case GPIO_PIN_MODE_INPUT: return     "IN";
    case GPIO_PIN_MODE_OUTPUT: return    "OUT";
    case GPIO_PIN_MODE_ALTERNATE: return "ALT";
    default: break;
    }
    return cli_gpio_undefined;
}

static const char* gpio_speed_str(uint32_t speed) {
    switch (speed) {
    case GPIO_PIN_SPEED_MEDIUM: return "MEDIUM";
    case GPIO_PIN_SPEED_LOW: return    "LOW";
    case GPIO_PIN_SPEED_HIGH: return   "HIGH";
    default: break;
    }
    return "---   ";
}

static const char* gpio_otype_str(uint32_t otype) {
    if (otype == GPIO_PIN_OTYPE_PUSH_PULL) {
        return "PP ";
    } else if (otype == GPIO_PIN_OTYPE_OPEN_DRAIN) {
        return "ODR";
    }
    return cli_gpio_undefined;
}

static const char* gpio_pull_str(uint32_t pull) {
    switch (pull) {
    case GPIO_PIN_PULL_NO: return   "PN";
    case GPIO_PIN_PULL_UP: return   "PU";
    case GPIO_PIN_PULL_DOWN: return "PD";
    default: break;
    }
    return "--";
}

static const char* gpio_flags_str(int flags) {
    switch (flags) {
    case 1: return " I ";
    case 2: return " O ";
    case 3: return "I/O";
    }
    return "N/A";
}

static const char gpio_port_letter(GPIO_TypeDef* GPIOx) {
    if (GPIOx == GPIOA) {
        return 'A';
    } else if (GPIOx == GPIOB) {
        return 'B';
    } else if (GPIOx == GPIOC) {
        return 'C';
    } else if (GPIOx == GPIOD) {
        return 'D';
    }
    return 0;
}

static void gpio_pin_get_name(char* buf, size_t buf_size, GPIO_Pin_t* pin) {
    if (NULL != buf) {
        char p = gpio_port_letter(pin->port);
        if (p != 0) {
            snprintf(buf, buf_size, "P%c%d", p, (int)pin->pin);
            if (strlen(buf) < 4) {
                buf[3] = ' ';
                buf[4] = 0;
            }
        } else {
            *buf = 0;
        }
    }
}

static void cli_print_n(const char* buf, int max) {
    for (int i = 0; i < max; i++) {
        if (*buf != 0) {
            putchar(*buf);
            buf++;
        } else {
            putchar(' ');
        }
    }
}

static uint32_t cli_table_append_cell(
    char* buf, uint32_t buf_size,
    const char* text, uint32_t max) {
    uint32_t i = 0;
    for (i = 0; (i < buf_size) && (i < max); i++) {
        if (*text != '\0') {
            *buf = *text;
            *text++;
        } else {
            *buf = ' ';
        }
        buf++;
    }
    *buf = 0;
    return i;
}

static void cli_gpio_pin_config_to_str(char* buf, uint32_t buf_size, GPIO_PinConfig_t* cfg) {
    if ((NULL != buf) && (buf_size > 0) && (NULL != cfg)) {
        struct data {
            uint32_t len;
            const char* str;
        } d[4];

        d[0].len = 4;
        d[0].str = gpio_mode_str(cfg->mode);
        d[1].len = 4;
        d[1].str = gpio_otype_str(cfg->outType);
        d[2].len = 6;
        d[2].str = gpio_speed_str(cfg->speed);
        d[3].len = 3;
        d[3].str = gpio_pull_str(cfg->pull);

        for (uint8_t i = 0; i < 4; i++) {
            cli_table_append_cell(buf, buf_size, d[i].str, d[i].len);
            if (buf_size <= d[i].len) {
                break;
            }

            buf_size -= d[i].len;
            buf += d[i].len;

            if (buf_size == 0) {
                break;
            }
        }
    }
}

static void cli_gpio_pin_str_info(char* buf, uint32_t buf_size, GPIO_Pin_t* pin, cli_gpio_pin_t* cli_pin) {
    if ((NULL != cli_pin) && (pin != NULL) && (NULL != buf) && (buf_size > 0)) {
        GPIO_PinConfig_t cfg;
        char pin_name[8];
        uint32_t cursor = 0;

        GPIO_PinGetConfig(pin->port, pin->pin, &cfg);
        gpio_pin_get_name(pin_name, 8, pin);

        snprintf(buf, buf_size, "%3d ", cli_pin->pin_no);

        cursor = strlen(buf);

        if (cursor >= buf_size) {
            return;
        }

        cli_table_append_cell(&buf[cursor], buf_size - cursor, pin_name, 5);

        cursor = strlen(buf);

        if (cursor >= buf_size) {
            return;
        }

        cli_table_append_cell(&buf[cursor], buf_size - cursor, cli_pin->label, 10);
        cursor = strlen(buf);
        if (cursor >= buf_size) {
            return;
        }

        cli_gpio_pin_config_to_str(&buf[cursor], buf_size - cursor, &cfg);
    }
}


static void cli_gpio_pin_print_info(char p, int n) {
    GPIO_Pin_t gpio_pin = {NULL, 0};
    cli_gpio_pin_t* cli_pin = cli_gpio_pin_get(&gpio_pin, p, n);
    if (NULL != cli_pin) {
        char str[64];
        cli_gpio_pin_str_info(str, 64, &gpio_pin, cli_pin);
        printf("%s\r\n", str);
    }
}

static void cli_gpio_port_print_info(char p) {
    cli_gpio_port_t* port = cli_gpio_port_get(p);
    if ((NULL != port) && (NULL != port->pin_list)) {
        GPIO_Pin_t pin = {port->port, 0};
        char str[64];
        printf("  GPIO%c\r\n", toupper(p));
        for (int i = 0; i < port->pin_count; i++) {
            pin.pin = port->pin_list[i].index;
            cli_gpio_pin_str_info(str, 64, &pin, &port->pin_list[i]);
            printf("    %s\r\n", str);
        }
        printf("\r\n");
    }
}


static void cli_gpio_info_cmd_help(void) {
    printf("\r\nPrint GPIO info\r\n\r\n");
}

static int cli_gpio_info_cmd_main(int argc, char** argv) {
    if (argc > 1) {
        argv++;
        char port = **argv;
        if (argc > 2) {
            argv++;
            int n = atoi(*argv);
            cli_gpio_pin_print_info(port, n);
        } else {
            cli_gpio_port_print_info(**argv);
        }

    } else {
        cli_gpio_port_print_info('a');
        cli_gpio_port_print_info('b');
        cli_gpio_port_print_info('c');
        cli_gpio_port_print_info('d');
    }
    printf("\r\n");
    return 0;
}

static cli_node_t cli_gpio_info_cmd = {
    .c_name = "gpio_info",
    .brief = "Print GPIO info",
    .main_fn = cli_gpio_info_cmd_main,
    .help_fn = cli_gpio_info_cmd_help,
};

void cli_gpio_info_cmd_init(void) {
    cli_regcmd(&cli_gpio_info_cmd);
}

/* GPIO Direction */

static void cli_gpio_dir_cmd_help(void) {
    printf(
        "\r\n USAGE: gpio_dir <port> <pin> [<in/out> [<0/1>]] \r\n\r\n"
        " If after pin number is no more argument this command\r\n"
        " will print current direction. \r\n\r\n"
        " If you want to set initial value on output, you can set\r\n"
        " this value after direction\r\n\r\n"
        " <port> can be upper or lower case letter of port.\r\n"
        " <in> can be '1', 'I' or 'i'\r\n"
        " <out> can be '0', 'O' or 'o'"
        "\r\n"
    );
}

const char* cli_gpio_pin_dir_str(int dir) {
    if (dir == HAL_GPIO_DIR_OUT) {
        return "OUTPUT";
    } else if (dir == HAL_GPIO_DIR_IN) {
        return "INPUT";
    }
    return "UNKNOWN";
}

static int cli_gpio_dir_cmd_main(int argc, char** argv) {
    if (argc < 3) {
        printf("USAGE: gpio_dir <port> <pin> [<in/out> <0/1>]");
        return -1;
    }
    char p = *(*(argv + 1));
    int n = atoi(*(argv + 2));
    GPIO_Pin_t pin;
    cli_gpio_pin_t* pin_dsc_ptr = cli_gpio_pin_get(&pin, p, n);
    if (NULL == pin_dsc_ptr) {
        printf("UNKNOWN GPIO: \'%c\' <%d>\r\n", p, n);
        return -1;
    } else if (argc < 4) {
        int dir = hal_gpio_pin_get_dir(&pin);
        const char* dir_str = cli_gpio_pin_dir_str(dir);

        printf("P%c%d direction %s\r\n", toupper(p), n, dir_str);
    } else {
        int dir = *(*(argv + 3));
        const char* dir_str = "UNDEFINED";

        if (argc > 4) {
            int init_val = *(*(argv + 4));
            if (init_val == '1') {
                init_val = 1;
            } else if (init_val == '0') {
                init_val = 0;
            } else {
                printf("Bad initial value argument: \'%c\'\r\n", init_val);
                return -1;
            }
            printf("set P%c%d initial value %d\r\n", toupper(p), n, init_val);
            hal_gpio_pin_set_value(&pin, init_val);
        }

        if (dir == 'i' || dir == 'I' || dir == '1') {
            dir = 1;
        } else if (dir == 'o' || dir == 'O' || dir == '0') {
            dir = 0;
        } else {
            printf("Bad direction argument: \'%c\'\r\n", dir);
            return -1;
        }

        if (HAL_IS_BIT_CLEAR(pin_dsc_ptr->flags, OUT_EN)) {
            printf("P%c%d can\'t be set as output\r\n", toupper(p), n);
            return -1;
        }

        dir_str = cli_gpio_pin_dir_str(dir);
        printf("set P%c%d direction %d\r\n", toupper(p), n, dir);
        hal_gpio_pin_set_dir(&pin, dir);
    }

    return 0;
}

static cli_node_t cli_gpio_dir_cmd = {
    .c_name = "gpio_dir",
    .brief = "GPIO get or set gpio direction",
    .main_fn = cli_gpio_dir_cmd_main,
    .help_fn = cli_gpio_dir_cmd_help,
};

void cli_gpio_dir_cmd_init(void) {
    cli_regcmd(&cli_gpio_dir_cmd);
}


/* GPIO Set output value */
static void cli_gpio_set_cmd_help(void) {
    printf(
        "\r\nUSAGE: gpio_set <port> <pin> <value>\r\n\r\n"
        );
}

static int cli_gpio_set_cmd_main(int argc, char** argv) {
    if (argc < 4) {
        printf("USAGE: gpio_set <port> <pin> <value>\r\n");
        return -1;
    }

    char p = *(*(argv + 1));
    int n = atoi(*(argv + 2));
    char v =  *(*(argv + 3));
    GPIO_Pin_t pin;
    cli_gpio_pin_t* pin_dsc_ptr = cli_gpio_pin_get(&pin, p, n);
    int dir = hal_gpio_pin_get_dir(&pin);
    int val = -1;

    if (dir != HAL_GPIO_DIR_OUT) {
        const char* dir_str = cli_gpio_pin_dir_str(dir);
        printf("P%c%d is not set as out\r\n");
        return -1;
    }

    if ((v == 'h') || (v == 'H') || (v == '1')) {
        printf("set P%c%d to HIGH\r\n");
        val = 1;
    } else if ((v == 'l') || (v == 'L') || (v == '0')) {
        printf("set P%c%d to LOW\r\n");
        val = 0;
    } else {
        printf("Bad value argument: \'%c\'\r\n", v);
        return -1;
    }

    hal_gpio_pin_set_value(&pin, val);
    return 0;
}

static cli_node_t cli_gpio_set_cmd = {
    .c_name = "gpio_set",
    .brief = "Set GPIO output value",
    .main_fn = cli_gpio_set_cmd_main,
    .help_fn = cli_gpio_set_cmd_help,
};

void cli_gpio_set_cmd_init(void) {
    cli_regcmd(&cli_gpio_set_cmd);
}

/* GPIO Get input value */

static void cli_gpio_get_cmd_help(void) {
    printf("\r\nPrint GPIO info\r\n\r\n");
}

static int cli_gpio_get_cmd_main(int argc, char** argv) {
    return 0;
}

static cli_node_t cli_gpio_get_cmd = {
    .c_name = "gpio_dir",
    .brief = "Print GPIO info",
    .main_fn = cli_gpio_get_cmd_main,
    .help_fn = cli_gpio_get_cmd_help,
};

void cli_gpio_get_cmd_init(void) {
    cli_regcmd(&cli_gpio_get_cmd);
}
