/*
 * HT16K33_FontDigits4x8.h - 4-column-wide, 8-pixel-tall digit font (0-9)
 *
 * Designed to display two digits side-by-side across the full 8x8 matrix
 * (4 + 4 = 8 columns). Derived from the MD_MAX72XX system font,
 * compressed to 4 columns.
 *
 * Each column byte encodes 8 rows: bit 0 = row 0 (top), bit 7 = row 7.
 *
 * Copyright (c) 2026 algomusic
 * MIT License
 */

#ifndef HT16K33_FONT_DIGITS4X8_H
#define HT16K33_FONT_DIGITS4X8_H

#include <Arduino.h>

#define FONTDIGITS4X8_WIDTH 4

static const uint8_t PROGMEM fontDigits4x8_data[][4] = {
    {0x3E, 0x41, 0x41, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40}, // 1
    {0x62, 0x51, 0x49, 0x46}, // 2
    {0x41, 0x49, 0x49, 0x36}, // 3
    {0x0F, 0x08, 0x08, 0x7F}, // 4
    {0x4F, 0x49, 0x49, 0x31}, // 5
    {0x3E, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x0D, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x3E}, // 9
};

// Get the 4 column bytes for a digit (0-9).
// Returns true if the digit is valid.
inline bool fontDigits4x8_getDigit(uint8_t digit, uint8_t out[4]) {
    if (digit > 9) return false;
    for (uint8_t i = 0; i < 4; i++) {
        out[i] = pgm_read_byte(&fontDigits4x8_data[digit][i]);
    }
    return true;
}

// Convenience: draw a digit at startCol using drawColumns().
#include "HT16K33_Matrix_8.h"
inline void fontDigits4x8_drawDigit(HT16K33_Matrix_8 &m, uint8_t digit, uint8_t startCol) {
    uint8_t cols[4];
    if (fontDigits4x8_getDigit(digit, cols)) {
        m.drawColumns(startCol, cols, FONTDIGITS4X8_WIDTH);
    }
}

#endif // HT16K33_FONT_DIGITS4X8_H
