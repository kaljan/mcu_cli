# Command line interface for MCUs

- [Command line interface for MCUs](#command-line-interface-for-mcus)
  - [Hotkey support](#hotkey-support)
  - [Pinout](#pinout)
    - [`GPIOA`](#gpioa)
    - [`GPIOB`](#gpiob)
    - [`GPIOC`](#gpioc)
    - [`GPIOD`](#gpiod)
  - [Build (Ubuntu 20.04)](#build-ubuntu-2004)
    - [Env setup](#env-setup)
    - [Build](#build)
  - [Flashing](#flashing)
    - [Flashing](#flashing-1)
  - [Build Scruipt](#build-scruipt)
    - [Build for **Bluepill** based on STM32F103C8 with 64K flash](#build-for-bluepill-based-on-stm32f103c8-with-64k-flash)
    - [Build for **Bluepill** based on STM32F103CB with 128K flash](#build-for-bluepill-based-on-stm32f103cb-with-128k-flash)
    - [Build with config shell file](#build-with-config-shell-file)


## Hotkey support

| **KEY**                   | **CODE**           | **FUNCTION**                          |
|:-------------------------:|:------------------:|:--------------------------------------|
| `Enter`<br>`CTRL+M`       | `0x0D`             | Run command                           |
| `TAB`<br>`CTRL+I`         | `0x09`             | Complete command                      |
| `Up`<br>`CTRL+N`          | `\e[A`<br>`0x0E`   | Next item from history                |
| `Down`<br>`CTRL+P`        | `\e[B`<br>`0x10`   | Previous item from history            |
| `Left`<br>`CTRL+B`        | `\e[D`<br>`0x02`   | Move cursor left one character        |
| `Right`<br>`CTRF+F`       | `\e[C`<br>`0x06`   | Move cursor right one character       |
| `CTRL+Left`               | `\e[1;3D`          | Move to beginning of word             |
| `CTRL+Right`              | `\e[1;3C`          | Move cursor to end of word            |
| `End`<br>`CTRL+E`         | `\eOF`<br>`0x05`   | Move cursor to end of line            |
| `Home`<br>`CTRL+A`        | `\e[1~`<br>`0x01`  | Move cursor to beginning of line      |
| `Delete`<br>`CTRL+D`      | `\e[3~`<br>`0x04`  | Delete character under cursor         |
| `Backspace`<br>`CTRL+H`   | `0x08`             | Delete character before cursor        |
| `CTRL+Delete`<br>`ALT+D`  | `\e[3;5~`<br>`\ed` | Delete word after cursor              |
| `CTRL+W`                  | `0x17`             | Delete word before cursor             |
| `CTRL+K`                  | `0x0B`             | Clear all after cursor                |
| `CTRL+U`                  | `0x15`             | Clear all before cursor               |
| `CTRL+C`                  | `0x03`             | At the momen its just skip all input  |



## Pinout

### `GPIOA`

| **MCU Pin** | **Name** | **Function** |
| :---------: | :------: | :----------- |
|     10      |  `PA0`   |              |
|     11      |  `PA1`   |              |
|     12      |  `PA2`   | `USART2_TX`  |
|     13      |  `PA3`   | `USART2_RX`  |
|     14      |  `PA4`   |              |
|     15      |  `PA5`   |              |
|     16      |  `PA6`   |              |
|     17      |  `PA7`   |              |
|     29      |  `PA8`   | `LCD_RS`     |
|     30      |  `PA9`   | `LCD_E`      |
|     31      |  `PA10`  |              |
|     32      |  `PA11`  | `USBDM`      |
|     33      |  `PA12`  | `USBDP`      |
|     34      |  `PA13`  | `SWDIO`      |
|     37      |  `PA14`  | `SWCLK`      |
|     38      |  `PA15`  | `LCD_PE`     |

### `GPIOB`

| **MCU Pin** | **Name** | **Function** |
| :---------: | :------: | :----------- |
|     18      |  `PB0`   |              |
|     19      |  `PB1`   |              |
|     20      |  `PB2`   | `BOOT1`      |
|     39      |  `PB3`   | `LCD_D5`     |
|     40      |  `PB4`   | `LCD_D7`     |
|     41      |  `PB5`   | `LCD_D6`     |
|     42      |  `PB6`   | `LCD_D4`     |
|     43      |  `PB7`   | `RCU_IN`     |
|     45      |  `PB8`   |              |
|     46      |  `PB9`   |              |
|     21      |  `PB10`  |              |
|     22      |  `PB11`  |              |
|     25      |  `PB12`  |              |
|     26      |  `PB13`  |              |
|     27      |  `PB14`  |              |
|     28      |  `PB15`  |              |

### `GPIOC`

| **MCU Pin** | **Name** | **Function** |
| :---------: | :------: | :----------- |
|      2      |  `PC13`  | `LED`        |
|      3      |  `PC14`  | `OSC32I`     |
|      4      |  `PC15`  | `OSC32O`     |

### `GPIOD`

| **MCU Pin** | **Name** | **Function** |
| :---------: | :------: | :----------- |
|      5      |  `PD0`   | `OSCI`       |
|      6      |  `PD1`   | `OSCO`       |

## Build (Ubuntu 20.04)

**Toolchain**: [`Arm GNU Toolchain 13.2.rel1`](https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz?rev=e434b9ea4afc4ed7998329566b764309&hash=CA590209F5774EE1C96E6450E14A3E26)

### Env setup

Install depencies

```bash
sudo apt install build-essential make gcc git
```

[Download toolchain](https://developer.arm.com/downloads/-/gnu-rm)

```bash
wget -O ~/Downloads/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz \
https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz?rev=e434b9ea4afc4ed7998329566b764309&hash=CA590209F5774EE1C96E6450E14A3E26
```

Create tools directory

```bash
mkdir -p $USER/opt
```

unpack it to `$(HOME)/opt` (or other directory)

```bash
tar -xvf ~/Downloads/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz -C $USER/opt/
```

### Build


Clone repositories

```bash
git clone git@github.com:kaljan/mcu_cli.git
cd mcu_cli
submodule update --init --recursive
```

Setup environment:

Get target name from project root directory name

```bash
export TARGET=${PWD##*/}
```

Debug

```bash
export DEBUG=1
```

Release

```bash
export RELEASE=1
```

If on **Bluepill** installed `STM32F103C8` (default)

```bash
export MCU_NAME=stm32f103c8
```

Or if `STM32F103CB` is installed

```bash
export MCU_NAME=stm32f103cb
```

Build

```bash
make
```

In directory `$TARGET"_"$MCU_NAME"_Debug"`

```bash
ls $TARGET"_"$MCU_NAME"_Debug"
```

You can find build artifacts.

* `$TARGET"_"$MCU_NAME.bin`
* `$TARGET"_"$MCU_NAME.hex`
* `$TARGET"_"$MCU_NAME.elf`

## Flashing

* **In circuit debugger** : `ST-Link v2.1`
* **Debug interface** : `SWD`
* **`st-util` version** : `v1.8.0-dirty`

[Installing `st-util`](https://github.com/stlink-org/stlink/blob/testing/doc/compiling.md)

### Flashing

```bash
st-flash --reset write $TARGET"_"$MCU_NAME"_Debug/"$TARGET"_"$MCU_NAME".bin" 0x08000000
```

## Build Scruipt

for building project you can use `build.sh` script

### Build for **Bluepill** based on STM32F103C8 with 64K flash

Release build

```bash
./build.sh
# or
./build -d=stm32f103c8
# or
./build -m=release
# or
./build -m=release -d=stm32f103c8
```

Debug build

```bash
./build.sh -m=debug
# or
./build -m=debug -d=stm32f103c8
```

### Build for **Bluepill** based on STM32F103CB with 128K flash

Release build

```bash
./build -d=stm32f103cb
# or
./build -m=release -d=stm32f103cb
```

Debug build

```bash
./build -m=debug -d=stm32f103cb
```

### Build with config shell file

if ./soruce/ directory you can find templates for build configs.
With `-f` option you can use your own build config. For ex.

Example config `path/to/your/config/file`
```bash
#!/bin/bash
export MCU_NAME=stm32f103cb
```

```bash
./build <other flags> -f path/to/your/config/file
```
