# Command line interface for MCUs

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
