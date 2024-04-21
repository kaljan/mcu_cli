# -------------------------------------------------------------------
# Build tools definitions
# -------------------------------------------------------------------
GCC_PATH = $(HOME)/opt/gcc-arm-none-eabi/bin
PREFIX = arm-none-eabi-

ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S -g
ECHO = echo
MKDIR = mkdir
RM = rm -fRv

# -------------------------------------------------------------------
# Project definitions
# -------------------------------------------------------------------
# Target name
ifndef TARGET
TARGET = mcu_cli
endif

# Project path
PROJECT_PATH = .
SRC_DIR = $(PROJECT_PATH)/src
HAL_DIR = $(SRC_DIR)/hal
CMSIS_DIR = $(HAL_DIR)/arch/CMSIS
CMSIS_DEV_STM32F1XX_DIR = $(CMSIS_DIR)/Device/ST/STM32F1xx
SYSTEM_DIR = $(SRC_DIR)/system
TOOLS_DIR = $(SRC_DIR)/tools
DRIVERS_DIR = $(SRC_DIR)/drivers
CLI_DIR	= $(SRC_DIR)/cli
APP_DIR = $(SRC_DIR)/app

# Build directory
ifeq ($(DEBUG), 1)
BUILD_DIR = $(PROJECT_PATH)/$(TARGET)_Debug
else ifeq ($(RELEASE), 1)
BUILD_DIR = $(PROJECT_PATH)/$(TARGET)_Release
else
BUILD_DIR = $(PROJECT_PATH)/$(TARGET)_Build
endif

# -------------------------------------------------------------------
# Source files
# -------------------------------------------------------------------
# ASM sources
ASM_SOURCES = \
	$(CMSIS_DEV_STM32F1XX_DIR)/Source/Templates/gcc/startup_stm32f103c8tx.s

# C sources
SYSTEM_CSRC = \
	$(SYSTEM_DIR)/syscalls.c \
	$(SYSTEM_DIR)/sysmem.c \

STM32F1XX_PERIPH_SRC = \
	$(CMSIS_DEV_STM32F1XX_DIR)/Source/system_stm32f1xx.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_it.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_bkp.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_config.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_flash.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_gpio.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_pwr.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_rcc.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_rtc.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_tim.c \
	$(HAL_DIR)/arch/stm32f1xx/src/stm32f1xx_uart.c

TOOLS_SRC = \
	$(TOOLS_DIR)/src/tools_fifo.c

HAL_SRC = \
	$(HAL_DIR)/src/hal_tim.c \
	$(HAL_DIR)/src/hal_mc6800.c

DRIVERS_SRC = \
	$(DRIVERS_DIR)/clcd/clcd.c \
	$(DRIVERS_DIR)/rcu/stm32f1xx_rcu.c

CLI_SRC = \
	$(CLI_DIR)/src/cli_history.c \
	$(CLI_DIR)/src/cli_io.c \
	$(CLI_DIR)/src/cli_exec.c

APP_SRC = \
	$(APP_DIR)/cli_gpio.c \
	$(APP_DIR)/cli_info.c \
	$(APP_DIR)/cli_reset.c \
	$(APP_DIR)/cli_echo.c \
	$(APP_DIR)/cli_clcd.c

C_SOURCES = \
	$(SYSTEM_CSRC) \
	$(STM32F1XX_PERIPH_SRC) \
	$(DRIVERS_SRC) \
	$(TOOLS_SRC) \
	$(HAL_SRC) \
	$(CLI_SRC) \
	$(APP_SRC) \
	$(SRC_DIR)/cfg/src/cfg_stm32f1xx_gpio.c \
	$(SRC_DIR)/cfg/src/cli_config.c \
	$(SRC_DIR)/main.c


# -------------------------------------------------------------------
# Include path
# -------------------------------------------------------------------
# C includes
C_INCLUDES = \
	-I$(SRC_DIR) \
	-I$(SRC_DIR)/cfg/inc \
	-I$(CLI_DIR)/inc \
	-I$(CMSIS_DEV_STM32F1XX_DIR)/Include \
	-I$(CMSIS_DIR)/Include \
	-I$(HAL_DIR)/arch/stm32f1xx/inc \
	-I$(SYSTEM_DIR) \
	-I$(DRIVERS_DIR)/clcd \
	-I$(DRIVERS_DIR)/rcu \
	-I$(HAL_DIR)/inc \
	-I$(TOOLS_DIR)/inc

# -------------------------------------------------------------------
# Defines
# -------------------------------------------------------------------
# C defines
C_DEFINES = \
	-DSTM32F1 \
	-DSTM32F103xB \
	-DHSE_STARTUP_TIMEOUT=100 \
	-DHSE_VALUE=8000000 \
	-DHSI_VALUE=8000000 \
	-DLSE_STARTUP_TIMEOUT=5000 \
	-DLSE_VALUE=32768 \
	-DLSI_VALUE=40000 \
	-DVDD_VALUE=3300 \
	-DTOOLS_OPT=\"tools_config.h\" \
	-DCLI_OPT=\"cli_config.h\"


ifeq ($(DEBUG), 1)
C_DEFINES += \
	-DDEBUG
endif

# -------------------------------------------------------------------
# Warning flags
# -------------------------------------------------------------------
# ASM warnings
ASM_WARNINGS = \
	-Wall \
	-Wno-unused-parameter \
	-Wno-unused \
	-Wno-format

# C warnings
C_WARNINGS = \
	-Wall \
	-Wno-unused-parameter \
	-Wno-unused \
	-Wno-format

# -------------------------------------------------------------------
# Optimisation flags
# -------------------------------------------------------------------
# ASM optimisations
ASM_OPTFLAGS = \
	-fdata-sections \
	-ffunction-sections

ifeq ($(DEBUG), 1)
ASM_OPTFLAGS += \
	-g3
endif

# C optimisations
C_OPTFLAGS = \
	-fdata-sections \
	-ffunction-sections

ifeq ($(DEBUG), 1)
C_OPTFLAGS += \
	-O0 \
	-g3
endif

ifeq ($(RELEASE), 1)
C_OPTFLAGS += -Os
endif

# -------------------------------------------------------------------
# Common flags
# -------------------------------------------------------------------
# ASM common flags
ASM_MISCFLAGS = \
	-fstack-usage

# C common flags
C_MISCFLAGS = \
	-fstack-usage
# -------------------------------------------------------------------
# Target architecture
# -------------------------------------------------------------------

# CPU architecture
CPU_ARCH =

# CPU core
CPU = -mcpu=cortex-m3

# CPU instruction set
CPU_ISET = -mthumb

# Float type
FLOAT_ABI = -mfloat-abi=soft

# FPU architecture
FPU =

TARGET_ARCH = $(CPU_ARCH) $(CPU) $(CPU_ISET) $(FLOAT_ABI) $(FPU)

# -------------------------------------------------------------------
# Build flags
# -------------------------------------------------------------------
# ASM flags
ASMFLAGS = $(TARGET_ARCH) $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_OPTFLAGS) $(ASM_WARNINGS) $(ASM_MISCFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs

# C flags
CFLAGS = $(TARGET_ARCH) -std=gnu11 $(C_DEFINES) $(C_INCLUDES) $(C_OPTFLAGS) $(C_WARNINGS) $(C_MISCFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs

# -------------------------------------------------------------------
# Linker flags
# -------------------------------------------------------------------
# Linker script
LDSCRIPT = $(CMSIS_DEV_STM32F1XX_DIR)/Source/Templates/gcc/linker/STM32F103C8TX_FLASH.ld
# libraries
LIBDIR =
LIBS =

# -u_printf_float

# Linker flags
LDFLAGS = \
	$(TARGET_ARCH) \
	-T$(LDSCRIPT) \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
	-Wl,--gc-sections \
	-static \
	--specs=nano.specs \
	-Wl,--start-group \
	$(LIBDIR) \
	$(LIBS) \
	-Wl,--end-group

# -------------------------------------------------------------------
# Objects
# -------------------------------------------------------------------
# list of ASM program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# list of C program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# -------------------------------------------------------------------
# Build section
# -------------------------------------------------------------------
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Build ASM objects
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@$(ECHO) "compiling: "$@"; "$<
	@$(AS) -c $(ASFLAGS) $< -o $@

# Build C objects
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@$(ECHO) "compiling: "$@"; "$<
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

# Linking
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@$(ECHO) "linking: "$@"; "$<
	@$(CC) -o $@ $(LDFLAGS) $(OBJECTS)
	$(SZ) $@

# Create HEX
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(ECHO) "create HEX file: "$@"; "$<
	$(HEX) $< $@

# Create BIN
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(ECHO) "create BIN file: "$@"; "$<
	$(BIN) $< $@

# Create build directory
$(BUILD_DIR):
	@$(ECHO) "create directory: "$@
	@$(MKDIR) $@

# -------------------------------------------------------------------
# Clean up
# -------------------------------------------------------------------
clean:
	@$(ECHO) "clean"
	@$(RM) $(BUILD_DIR)

# -------------------------------------------------------------------
# Dependencies
# -------------------------------------------------------------------
-include $(wildcard $(BUILD_DIR)/*.d)


