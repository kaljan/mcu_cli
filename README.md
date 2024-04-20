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
