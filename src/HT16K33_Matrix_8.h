/*
 * HT16K33_Matrix_8.h - Driver for 8x8 LED matrix with HT16K33 controller
 *
 * Non-blocking I2C driver targeting ESP32-S3 (ArduinoCore V3).
 * Maintains a local framebuffer; call write() to push to hardware.
 *
 * Copyright (c) 2026 algomusic
 * MIT License
 */

#ifndef HT16K33_MATRIX_8_H
#define HT16K33_MATRIX_8_H

#include <Arduino.h>
#include <Wire.h>

// HT16K33 commands (from datasheet)
#define HT16K33_CMD_SYSTEM_SETUP 0x20
#define HT16K33_CMD_DISPLAY_SETUP 0x80
#define HT16K33_CMD_DIMMING 0xE0
#define HT16K33_CMD_ROW_INT 0xA0
#define HT16K33_CMD_RAM 0x00

// System setup bits
#define HT16K33_OSCILLATOR_ON 0x01
#define HT16K33_OSCILLATOR_OFF 0x00

// Display on/off
#define HT16K33_DISPLAY_ON 0x01
#define HT16K33_DISPLAY_OFF 0x00

// Blink rates
#define HT16K33_BLINK_OFF 0x00
#define HT16K33_BLINK_2HZ 0x02
#define HT16K33_BLINK_1HZ 0x04
#define HT16K33_BLINK_0_5HZ 0x06

// Default I2C address (A0=A1=A2 grounded)
#define HT16K33_DEFAULT_ADDR 0x70

// Matrix dimensions
#define HT16K33_MATRIX_ROWS 8
#define HT16K33_MATRIX_COLS 8

class HT16K33_Matrix_8 {
public:
    HT16K33_Matrix_8();

    // Initialise the display.
    // addr: 7-bit I2C address (0x70-0x77)
    // sda/scl: GPIO pins (-1 = use Wire defaults)
    // wireInstance: TwoWire bus to use (default Wire)
    // Returns true on success.
    bool begin(uint8_t addr = HT16K33_DEFAULT_ADDR,
               int sda = -1, int scl = -1,
               TwoWire &wireInstance = Wire);

    // Set global brightness (0-15).
    void setBrightness(uint8_t level);

    // Set hardware blink rate. Use HT16K33_BLINK_* constants.
    void setBlinkRate(uint8_t rate);

    // Turn the display on or off (preserves RAM contents).
    void displayOn();
    void displayOff();

    // Pixel operations (modify local buffer only; call write() to update hardware).
    void setPixel(uint8_t row, uint8_t col, bool on);
    bool getPixel(uint8_t row, uint8_t col) const;
    void togglePixel(uint8_t row, uint8_t col);

    // Bulk operations
    void setRow(uint8_t row, uint8_t value);
    uint8_t getRow(uint8_t row) const;
    void setColumn(uint8_t col, uint8_t value);
    void fill(bool on);
    void clear();

    // Shift the entire display left by one column. A new column of data is
    // inserted at the right edge (column 7). Use for scrolling text/graphics.
    // colData: bit N = row N pixel state (0 = default, blank column).
    void shiftLeft(uint8_t colData = 0);

    // Draw a 4x4 bitmap at (startRow, startCol). Each byte in data[] is one row,
    // with bit N = column N within the 4x4 block. Sets and clears pixels.
    void drawBitmap4x4(uint8_t startRow, uint8_t startCol, const uint8_t data[4]);

    // Draw consecutive column bytes starting at startCol. Each byte in data[]
    // encodes one column with bit N = row N. Useful for variable-width fonts
    // that store characters as column bytes (e.g. HT16K33_FontFull).
    void drawColumns(uint8_t startCol, const uint8_t *data, uint8_t count);

    // Push the entire framebuffer to the HT16K33 over I2C.
    // Returns true if the I2C transmission succeeded.
    bool write();

private:
    TwoWire *_wire;
    uint8_t _addr;
    uint8_t _buffer[HT16K33_MATRIX_ROWS]; // 8 bytes, one per COM row

    bool sendCommand(uint8_t cmd);
};

#endif // HT16K33_MATRIX_8_H
