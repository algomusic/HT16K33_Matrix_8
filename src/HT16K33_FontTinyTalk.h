/*
 * HT16K33_FontTinyTalk.h - "Tiny Talk" 4x4 pixel font for 8x8 LED matrix
 *
 * Lowercase-only font (a-z, 0-9) sourced from the Tiny Talk 4x4 Font CSV.
 * Each glyph is 4 rows x 4 columns, stored as 4 bytes.
 * Bit N of each byte corresponds to column N (bit 0 = leftmost).
 *
 * Copyright (c) 2026 algomusic
 * MIT License
 */

#ifndef HT16K33_FONT_TINYTALK_H
#define HT16K33_FONT_TINYTALK_H

#include <Arduino.h>

// Glyph bit encoding (per row byte):
//   bit 0 = col 0 (left)
//   bit 1 = col 1
//   bit 2 = col 2
//   bit 3 = col 3 (right)

static const uint8_t PROGMEM tinyTalk_alpha[][4] = {
    {0x00, 0x06, 0x05, 0x07}, // a  .... .XX. X.X. XXX.
    {0x01, 0x07, 0x05, 0x07}, // b  X... XXX. X.X. XXX.
    {0x00, 0x07, 0x01, 0x07}, // c  .... XXX. X... XXX.
    {0x04, 0x07, 0x05, 0x07}, // d  ..X. XXX. X.X. XXX.
    {0x00, 0x07, 0x05, 0x03}, // e  .... XXX. X.X. XX..
    {0x06, 0x01, 0x03, 0x01}, // f  .XX. X... XX.. X...
    {0x07, 0x05, 0x07, 0x06}, // g  XXX. X.X. XXX. .XX.
    {0x01, 0x07, 0x05, 0x05}, // h  X... XXX. X.X. X.X.
    {0x02, 0x00, 0x02, 0x02}, // i  .X.. .... .X.. .X..
    {0x02, 0x00, 0x02, 0x03}, // j  .X.. .... .X.. XX..
    {0x01, 0x05, 0x03, 0x05}, // k  X... X.X. XX.. X.X.
    {0x02, 0x02, 0x02, 0x02}, // l  .X.. .X.. .X.. .X..
    {0x00, 0x07, 0x07, 0x05}, // m  .... XXX. XXX. X.X.
    {0x00, 0x07, 0x05, 0x05}, // n  .... XXX. X.X. X.X.
    {0x00, 0x07, 0x05, 0x07}, // o  .... XXX. X.X. XXX.
    {0x07, 0x05, 0x07, 0x01}, // p  XXX. X.X. XXX. X...
    {0x07, 0x05, 0x07, 0x04}, // q  XXX. X.X. XXX. ..X.
    {0x00, 0x07, 0x05, 0x01}, // r  .... XXX. X.X. X...
    {0x00, 0x06, 0x02, 0x03}, // s  .... .XX. .X.. XX..
    {0x02, 0x07, 0x02, 0x06}, // t  .X.. XXX. .X.. .XX.
    {0x00, 0x05, 0x05, 0x07}, // u  .... X.X. X.X. XXX.
    {0x00, 0x05, 0x05, 0x06}, // v  .... X.X. X.X. .XX.
    {0x00, 0x05, 0x07, 0x07}, // w  .... X.X. XXX. XXX.
    {0x00, 0x05, 0x02, 0x05}, // x  .... X.X. .X.. X.X.
    {0x05, 0x05, 0x07, 0x06}, // y  X.X. X.X. XXX. .XX.
    {0x00, 0x03, 0x02, 0x06}, // z  .... XX.. .X.. .XX.
};

static const uint8_t PROGMEM tinyTalk_digit[][4] = {
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

// Look up a Tiny Talk glyph for the given ASCII character.
// Supports a-z (lowercase only) and 0-9.
// Uppercase A-Z are mapped to lowercase automatically.
// Copies 4 bytes into `out`. Returns true if the character is supported.
inline bool tinyTalk_getGlyph(char ch, uint8_t out[4]) {
    const uint8_t *src = nullptr;
    if (ch >= 'a' && ch <= 'z') {
        src = tinyTalk_alpha[ch - 'a'];
    } else if (ch >= 'A' && ch <= 'Z') {
        src = tinyTalk_alpha[ch - 'A'];
    } else if (ch >= '0' && ch <= '9') {
        src = tinyTalk_digit[ch - '0'];
    } else {
        return false;
    }
    for (uint8_t i = 0; i < 4; i++) {
        out[i] = pgm_read_byte(&src[i]);
    }
    return true;
}

// Convenience: look up a glyph and draw it at (row, col) in one call.
// Draws blank if character is not found.
#include "HT16K33_Matrix_8.h"
inline void tinyTalk_drawChar(HT16K33_Matrix_8 &m, char ch, uint8_t row, uint8_t col) {
    uint8_t glyph[4] = {0, 0, 0, 0};
    tinyTalk_getGlyph(ch, glyph);
    m.drawBitmap4x4(row, col, glyph);
}

#endif // HT16K33_FONT_TINYTALK_H
