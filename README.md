# HT16K33_Matrix_8

Non-blocking Arduino driver for an 8x8 LED matrix driven by the Holtek HT16K33 I2C controller. Designed for the ESP32-S3 with ArduinoCore V3, but compatible with any Arduino-supported board.

## Features

- Configurable I2C pins (essential for ESP32-S3 where any GPIO can be I2C)
- Buffered framebuffer - pixel operations are instant, `write()` pushes to hardware
- Individual pixel set / get / toggle
- Bulk row, column, fill, and clear operations
- Column-level scrolling with `shiftLeft()`
- Multi-column rendering with `drawColumns()`
- 4x4 bitmap drawing with `drawBitmap4x4()`
- 16-step hardware brightness control
- Hardware blink rate control (off, 0.5 Hz, 1 Hz, 2 Hz)
- No blocking delays - safe to use alongside real-time audio or other timing-critical code
- All I2C transactions complete in a single call (no multi-frame state machines)
- Hardware column offset correction for common HT16K33 LED matrix modules

## Fonts

Four font headers are included, each with a `_getGlyph()` data function and a `_drawChar()` convenience function:

| Header | Characters | Size | Format | Draw function |
|--------|-----------|------|--------|---------------|
| `HT16K33_FontTinyTalk.h` | a-z, 0-9 | 4x4 | Row bytes for `drawBitmap4x4()` | `tinyTalk_drawChar(matrix, ch, row, col)` |
| `HT16K33_Font3x3.h` | a-z (3x3), 0-9 (3x4) | 4x4 | Row bytes, letters have col 3 & row 3 blank | `font3x3_drawChar(matrix, ch, row, col)` |
| `HT16K33_FontFull.h` | ASCII 32-122 (A-Z, a-z, 0-9, punctuation) | Variable width, 8px tall | Column bytes for `drawColumns()` / `shiftLeft()` | `fontFull_drawChar(matrix, ch, startCol)` |
| `HT16K33_FontDigits4x8.h` | 0-9 | 4 cols, 8px tall | Column bytes, two digits fill 8 columns | `fontDigits4x8_drawDigit(matrix, digit, startCol)` |

The 4x4 fonts divide the display into four quadrants (top-left, top-right, bottom-left, bottom-right) for showing up to 4 characters at once. The full-height fonts use the entire 8-row display for scrolling text or large numerals.

## Hardware

| Signal | Default GPIO | Notes |
|--------|-------------|-------|
| SDA    | 18          | Configurable in `begin()` |
| SCL    | 21          | Configurable in `begin()` |
| VCC    | 3.3V / 5V   | HT16K33 operates at 4.5-5.5V; many breakout boards include a regulator |
| GND    | GND         | |

The I2C address defaults to `0x70`. Solder the A0/A1/A2 jumpers on the backpack to select addresses `0x70`-`0x77`.

## Installation

Copy the `HT16K33_Matrix_8` folder into your Arduino `libraries` directory, or use the Arduino IDE Library Manager to install from a ZIP.

## Quick Start

```cpp
#include <HT16K33_Matrix_8.h>

HT16K33_Matrix_8 matrix;

void setup() {
    matrix.begin(0x70, 18, 21);
    matrix.setBrightness(8);
}

void loop() {
    matrix.clear();
    matrix.setPixel(3, 4, true);
    matrix.write();
    delay(500);

    matrix.togglePixel(3, 4);
    matrix.write();
    delay(500);
}
```

## API Reference

### Initialisation

| Method | Description |
|--------|-------------|
| `bool begin(addr, sda, scl, wireInstance)` | Initialise the display. `sda`/`scl` default to -1 (use Wire defaults). Returns false if the device is not found. |

### Display Control

| Method | Description |
|--------|-------------|
| `void setBrightness(uint8_t level)` | Set brightness 0-15 (PWM duty cycle). |
| `void setBlinkRate(uint8_t rate)` | Set hardware blink: `HT16K33_BLINK_OFF`, `_2HZ`, `_1HZ`, `_0_5HZ`. |
| `void displayOn()` | Enable display output. |
| `void displayOff()` | Disable display output (RAM preserved). |

### Pixel and Buffer Operations

All buffer methods modify the local framebuffer only. Call `write()` to update the hardware.

| Method | Description |
|--------|-------------|
| `void setPixel(row, col, on)` | Set a single LED on or off. |
| `bool getPixel(row, col)` | Query the current state of a pixel. |
| `void togglePixel(row, col)` | Invert a single LED. |
| `void setRow(row, value)` | Set all 8 LEDs in a row from an 8-bit value. |
| `uint8_t getRow(row)` | Read back the 8-bit row value. |
| `void setColumn(col, value)` | Set all 8 LEDs in a column (bit N = row N). |
| `void fill(bool on)` | Turn all LEDs on or off. |
| `void clear()` | Turn all LEDs off. |

### Drawing and Scrolling

| Method | Description |
|--------|-------------|
| `void drawBitmap4x4(row, col, data)` | Draw a 4x4 bitmap at (row, col). `data` is 4 row bytes, bit N = column N. |
| `void drawColumns(startCol, data, count)` | Draw consecutive column bytes. Each byte: bit N = row N. For variable-width font rendering. |
| `void shiftLeft(colData)` | Shift entire display left by one column. Insert `colData` at column 7 (default 0 = blank). Core primitive for smooth text scrolling. |

### Hardware Output

| Method | Description |
|--------|-------------|
| `bool write()` | Push buffer to the HT16K33 via I2C. Returns true on success. Includes a 1-bit column rotation to correct the hardware offset found on common LED matrix modules. |

### Coordinate System

```
        col 0  col 1  col 2  ...  col 7
row 0  [ 0,0 ] [ 0,1 ] [ 0,2 ]    [ 0,7 ]
row 1  [ 1,0 ] [ 1,1 ] [ 1,2 ]    [ 1,7 ]
  ...
row 7  [ 7,0 ] [ 7,1 ] [ 7,2 ]    [ 7,7 ]
```

### 4x4 Quadrant Layout

The 4x4 fonts (`FontTinyTalk`, `Font3x3`) divide the display into four quadrants:

```
+------+------+
| 0,0  | 0,4  |   top-left / top-right
+------+------+
| 4,0  | 4,4  |   bottom-left / bottom-right
+------+------+
```

## Examples

| Example | Description |
|---------|-------------|
| **BasicPixelControl** | Pixel walk, row/column sweeps, checkerboard, brightness fade, `getPixel()` demo. |
| **TinyTalkQuadrants** | Four-quadrant display using the Tiny Talk 4x4 font. Cycles letter pairs with a counter. |
| **Font3x3Quadrants** | Four-quadrant display using the 3x3 font with built-in character spacing. |
| **WordBlocks** | Displays 4-character words across all quadrants using the 3x3 font. Randomly cycles words. |
| **ScrollingWords** | Smooth pixel-by-pixel scrolling of words across the top half (3x3 font) with a counter in the bottom half. |
| **ScrollingFullDisplay** | Full-display text scrolling using the variable-width 8-pixel-tall font. One column per frame via `shiftLeft()`. |
| **PotReadout** | Full-display 0-99 numerical readout driven by a potentiometer on GPIO 3. Uses the 4x8 digit font. |

## Non-Blocking Design

The library never uses `delay()`. All I2C transactions are short single-burst writes (~18 bytes for a full frame update). This makes it safe to call from `loop()` alongside latency-sensitive code such as real-time audio processing. Buffer operations (`setPixel`, `clear`, etc.) are pure memory writes with zero I2C overhead.

## License

MIT License. See [LICENSE](LICENSE) for details.
