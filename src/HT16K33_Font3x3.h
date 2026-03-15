/*
 * HT16K33_Font3x3.h - "3x3 Char" 4x4 pixel font for 8x8 LED matrix
 *
 * Lowercase-only font (a-z, 0-9) sourced from 3x3CharFont.csv.
 * Letters use only the top-left 3x3 area of each 4x4 quadrant
 * (column 3 and row 3 always blank) for clear spacing.
 *
 * Digits use a 3x4 layout: left-justified in columns 0-2 with
 * all 4 rows, matching the Tiny Talk digit patterns.
 *
 * Each glyph is stored as 4 bytes (one per row).
 * Bit N of each byte corresponds to column N (bit 0 = leftmost).
 *
 * Copyright (c) 2026 algomusic
 * MIT License
 */

#ifndef HT16K33_FONT_3X3_H
#define HT16K33_FONT_3X3_H

#include <Arduino.h>

static const uint8_t PROGMEM font3x3_alpha[][4] = {
    {0x06, 0x05, 0x07, 0x00}, // a  .XX. X.X. XXX. ....
    {0x01, 0x07, 0x07, 0x00}, // b  X... XXX. XXX. ....
    {0x07, 0x01, 0x07, 0x00}, // c  XXX. X... XXX. ....
    {0x04, 0x07, 0x07, 0x00}, // d  ..X. XXX. XXX. ....
    {0x07, 0x05, 0x03, 0x00}, // e  XXX. X.X. XX.. ....
    {0x07, 0x03, 0x01, 0x00}, // f  XXX. XX.. X... ....
    {0x07, 0x07, 0x06, 0x00}, // g  XXX. XXX. .XX. ....
    {0x01, 0x07, 0x05, 0x00}, // h  X... XXX. X.X. ....
    {0x02, 0x02, 0x02, 0x00}, // i  .X.. .X.. .X.. ....
    {0x02, 0x02, 0x03, 0x00}, // j  .X.. .X.. XX.. ....
    {0x05, 0x03, 0x05, 0x00}, // k  X.X. XX.. X.X. ....
    {0x01, 0x01, 0x03, 0x00}, // l  X... X... XX.. ....
    {0x07, 0x07, 0x05, 0x00}, // m  XXX. XXX. X.X. ....
    {0x07, 0x05, 0x05, 0x00}, // n  XXX. X.X. X.X. ....
    {0x07, 0x05, 0x07, 0x00}, // o  XXX. X.X. XXX. ....
    {0x07, 0x07, 0x01, 0x00}, // p  XXX. XXX. X... ....
    {0x07, 0x07, 0x04, 0x00}, // q  XXX. XXX. ..X. ....
    {0x07, 0x05, 0x01, 0x00}, // r  XXX. X.X. X... ....
    {0x06, 0x02, 0x03, 0x00}, // s  .XX. .X.. XX.. ....
    {0x01, 0x03, 0x01, 0x00}, // t  X... XX.. X... ....
    {0x05, 0x05, 0x07, 0x00}, // u  X.X. X.X. XXX. ....
    {0x05, 0x05, 0x06, 0x00}, // v  X.X. X.X. .XX. ....
    {0x05, 0x07, 0x07, 0x00}, // w  X.X. XXX. XXX. ....
    {0x05, 0x02, 0x05, 0x00}, // x  X.X. .X.. X.X. ....
    {0x05, 0x02, 0x02, 0x00}, // y  X.X. .X.. .X.. ....
    {0x03, 0x02, 0x06, 0x00}, // z  XX.. .X.. .XX. ....
};

// Digits: 3x4 layout (3 columns, 4 rows). Same as Tiny Talk digits,
// already left-justified in columns 0-2.
static const uint8_t PROGMEM font3x3_digit[][4] = {
    {0x07, 0x05, 0x05, 0x07}, // 0  XXX. X.X. X.X. XXX.
    {0x03, 0x02, 0x02, 0x07}, // 1  XX.. .X.. .X.. XXX.
    {0x07, 0x04, 0x03, 0x07}, // 2  XXX. ..X. XX.. XXX.
    {0x07, 0x04, 0x06, 0x07}, // 3  XXX. ..X. .XX. XXX.
    {0x05, 0x05, 0x07, 0x04}, // 4  X.X. X.X. XXX. ..X.
    {0x07, 0x01, 0x06, 0x07}, // 5  XXX. X... .XX. XXX.
    {0x07, 0x01, 0x07, 0x07}, // 6  XXX. X... XXX. XXX.
    {0x07, 0x04, 0x02, 0x01}, // 7  XXX. ..X. .X.. X...
    {0x07, 0x05, 0x07, 0x07}, // 8  XXX. X.X. XXX. XXX.
    {0x07, 0x07, 0x04, 0x07}, // 9  XXX. XXX. ..X. XXX.
};

// Look up a 3x3 font glyph for the given ASCII character.
// Supports a-z (lowercase only) and 0-9.
// Uppercase A-Z are mapped to lowercase automatically.
// Copies 4 bytes into `out`. Returns true if the character is supported.
inline bool font3x3_getGlyph(char ch, uint8_t out[4]) {
    const uint8_t *src = nullptr;
    if (ch >= 'a' && ch <= 'z') {
        src = font3x3_alpha[ch - 'a'];
    } else if (ch >= 'A' && ch <= 'Z') {
        src = font3x3_alpha[ch - 'A'];
    } else if (ch >= '0' && ch <= '9') {
        src = font3x3_digit[ch - '0'];
    } else {
        return false;
    }
    for (uint8_t i = 0; i < 4; i++) {
        out[i] = pgm_read_byte(&src[i]);
    }
    return true;
}

// Convenience: look up a glyph and draw it at (row, col) in one call.
// Draws blank if character is not found or is a space.
#include "HT16K33_Matrix_8.h"
inline void font3x3_drawChar(HT16K33_Matrix_8 &m, char ch, uint8_t row, uint8_t col) {
    uint8_t glyph[4] = {0, 0, 0, 0};
    if (ch != ' ' && ch != '\0') font3x3_getGlyph(ch, glyph);
    m.drawBitmap4x4(row, col, glyph);
}

#endif // HT16K33_FONT_3X3_H
