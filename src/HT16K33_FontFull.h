/*
 * HT16K33_FontFull.h - Variable-width 8-pixel-tall font for full-display use
 *
 * Font data derived from the MD_MAX72XX system font.
 * Each character is stored as [width, col0, col1, ...] with up to 5 columns.
 * Each column byte encodes 8 vertical pixels: bit 0 = row 0 (top),
 * bit 7 = row 7 (bottom).
 *
 * Covers ASCII 32-122 (space, punctuation, 0-9, A-Z, a-z).
 *
 * Copyright (c) 2026 algomusic
 * MIT License
 */

#ifndef HT16K33_FONT_FULL_H
#define HT16K33_FONT_FULL_H

#include <Arduino.h>

#define FONTFULL_FIRST_CHAR 32
#define FONTFULL_LAST_CHAR  122
#define FONTFULL_MAX_WIDTH  5

// Each entry: [width, col0, col1, col2, col3, col4]
// Narrow characters have trailing zero-padding.
static const uint8_t PROGMEM fontFull_data[][6] = {
    {2, 0,   0,   0,   0,   0  }, // 32 ' '
    {1, 95,  0,   0,   0,   0  }, // 33 '!'
    {3, 7,   0,   7,   0,   0  }, // 34 '"'
    {5, 20,  127, 20,  127, 20 }, // 35 '#'
    {5, 68,  74,  255, 74,  50 }, // 36 '$'
    {5, 99,  19,  8,   100, 99 }, // 37 '%'
    {5, 54,  73,  73,  54,  72 }, // 38 '&'
    {1, 7,   0,   0,   0,   0  }, // 39 '''
    {3, 62,  65,  65,  0,   0  }, // 40 '('
    {3, 65,  65,  62,  0,   0  }, // 41 ')'
    {5, 8,   42,  28,  42,  8  }, // 42 '*'
    {5, 8,   8,   62,  8,   8  }, // 43 '+'
    {2, 96,  224, 0,   0,   0  }, // 44 ','
    {4, 8,   8,   8,   8,   0  }, // 45 '-'
    {2, 96,  96,  0,   0,   0  }, // 46 '.'
    {5, 96,  16,  8,   4,   3  }, // 47 '/'
    {5, 62,  81,  73,  69,  62 }, // 48 '0'
    {3, 4,   2,   127, 0,   0  }, // 49 '1'
    {5, 113, 73,  73,  73,  70 }, // 50 '2'
    {5, 65,  73,  73,  73,  54 }, // 51 '3'
    {5, 15,  8,   8,   8,   127}, // 52 '4'
    {5, 79,  73,  73,  73,  49 }, // 53 '5'
    {5, 62,  73,  73,  73,  48 }, // 54 '6'
    {5, 3,   1,   1,   1,   127}, // 55 '7'
    {5, 54,  73,  73,  73,  54 }, // 56 '8'
    {5, 6,   73,  73,  73,  62 }, // 57 '9'
    {2, 108, 108, 0,   0,   0  }, // 58 ':'
    {2, 108, 236, 0,   0,   0  }, // 59 ';'
    {3, 8,   20,  34,  0,   0  }, // 60 '<'
    {4, 20,  20,  20,  20,  0  }, // 61 '='
    {3, 34,  20,  8,   0,   0  }, // 62 '>'
    {5, 1,   89,  9,   9,   6  }, // 63 '?'
    {5, 62,  65,  93,  89,  78 }, // 64 '@'
    {5, 126, 9,   9,   9,   126}, // 65 'A'
    {5, 127, 73,  73,  73,  54 }, // 66 'B'
    {5, 62,  65,  65,  65,  65 }, // 67 'C'
    {5, 127, 65,  65,  65,  62 }, // 68 'D'
    {5, 127, 73,  73,  73,  65 }, // 69 'E'
    {5, 127, 9,   9,   9,   1  }, // 70 'F'
    {5, 62,  65,  65,  73,  121}, // 71 'G'
    {5, 127, 8,   8,   8,   127}, // 72 'H'
    {3, 65,  127, 65,  0,   0  }, // 73 'I'
    {5, 48,  65,  65,  65,  63 }, // 74 'J'
    {5, 127, 8,   20,  34,  65 }, // 75 'K'
    {5, 127, 64,  64,  64,  64 }, // 76 'L'
    {5, 127, 2,   12,  2,   127}, // 77 'M'
    {5, 127, 4,   8,   16,  127}, // 78 'N'
    {5, 62,  65,  65,  65,  62 }, // 79 'O'
    {5, 127, 9,   9,   9,   6  }, // 80 'P'
    {5, 62,  65,  65,  97,  126}, // 81 'Q'
    {5, 127, 9,   25,  41,  70 }, // 82 'R'
    {5, 70,  73,  73,  73,  49 }, // 83 'S'
    {5, 1,   1,   127, 1,   1  }, // 84 'T'
    {5, 63,  64,  64,  64,  63 }, // 85 'U'
    {5, 31,  32,  64,  32,  31 }, // 86 'V'
    {5, 63,  64,  56,  64,  63 }, // 87 'W'
    {5, 99,  20,  8,   20,  99 }, // 88 'X'
    {5, 3,   4,   120, 4,   3  }, // 89 'Y'
    {5, 97,  81,  73,  69,  67 }, // 90 'Z'
    {3, 127, 65,  65,  0,   0  }, // 91 '['
    {5, 3,   4,   8,   16,  96 }, // 92 '\'
    {3, 65,  65,  127, 0,   0  }, // 93 ']'
    {5, 4,   2,   1,   2,   4  }, // 94 '^'
    {4, 128, 128, 128, 128, 0  }, // 95 '_'
    {3, 1,   2,   4,   0,   0  }, // 96 '`'
    {4, 56,  68,  68,  124, 0  }, // 97 'a'
    {4, 127, 68,  68,  56,  0  }, // 98 'b'
    {4, 56,  68,  68,  68,  0  }, // 99 'c'
    {4, 56,  68,  68,  127, 0  }, // 100 'd'
    {4, 56,  84,  84,  88,  0  }, // 101 'e'
    {4, 4,   126, 5,   1,   0  }, // 102 'f'
    {4, 24,  164, 164, 124, 0  }, // 103 'g'
    {4, 127, 4,   4,   120, 0  }, // 104 'h'
    {1, 125, 0,   0,   0,   0  }, // 105 'i'
    {3, 132, 133, 124, 0,   0  }, // 106 'j'
    {4, 127, 16,  40,  68,  0  }, // 107 'k'
    {1, 127, 0,   0,   0,   0  }, // 108 'l'
    {5, 124, 4,   120, 4,   120}, // 109 'm'
    {4, 124, 4,   4,   120, 0  }, // 110 'n'
    {4, 56,  68,  68,  56,  0  }, // 111 'o'
    {4, 252, 36,  36,  24,  0  }, // 112 'p'
    {4, 24,  36,  36,  252, 0  }, // 113 'q'
    {4, 124, 4,   4,   8,   0  }, // 114 'r'
    {4, 88,  84,  84,  52,  0  }, // 115 's'
    {3, 4,   127, 4,   0,   0  }, // 116 't'
    {4, 60,  64,  64,  124, 0  }, // 117 'u'
    {4, 28,  32,  64,  124, 0  }, // 118 'v'
    {5, 60,  64,  48,  64,  60 }, // 119 'w'
    {4, 108, 16,  16,  108, 0  }, // 120 'x'
    {4, 28,  160, 160, 124, 0  }, // 121 'y'
    {4, 100, 84,  84,  76,  0  }, // 122 'z'
};

// Get the column data for a character.
// Returns the character width (number of valid bytes in buf).
// buf must have room for FONTFULL_MAX_WIDTH bytes.
// Returns 0 for unsupported characters.
inline uint8_t fontFull_getChar(char ch, uint8_t *buf) {
    if (ch < FONTFULL_FIRST_CHAR || ch > FONTFULL_LAST_CHAR) return 0;
    uint8_t idx = (uint8_t)ch - FONTFULL_FIRST_CHAR;
    uint8_t width = pgm_read_byte(&fontFull_data[idx][0]);
    for (uint8_t i = 0; i < width && i < FONTFULL_MAX_WIDTH; i++) {
        buf[i] = pgm_read_byte(&fontFull_data[idx][1 + i]);
    }
    return width;
}

// Convenience: look up a character and draw its columns at startCol.
// Returns the character width (number of columns drawn).
#include "HT16K33_Matrix_8.h"
inline uint8_t fontFull_drawChar(HT16K33_Matrix_8 &m, char ch, uint8_t startCol) {
    uint8_t cols[FONTFULL_MAX_WIDTH];
    uint8_t width = fontFull_getChar(ch, cols);
    m.drawColumns(startCol, cols, width);
    return width;
}

#endif // HT16K33_FONT_FULL_H
