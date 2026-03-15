/*
 * HT16K33_Matrix_8.cpp - Driver for 8x8 LED matrix with HT16K33 controller
 *
 * Copyright (c) 2026 algomusic
 * MIT License
 */

#include "HT16K33_Matrix_8.h"

HT16K33_Matrix_8::HT16K33_Matrix_8()
    : _wire(nullptr), _addr(HT16K33_DEFAULT_ADDR) {
    memset(_buffer, 0, sizeof(_buffer));
}

bool HT16K33_Matrix_8::begin(uint8_t addr, int sda, int scl, TwoWire &wireInstance) {
    _wire = &wireInstance;
    _addr = addr;

    // Initialise I2C with custom pins if provided
    if (sda >= 0 && scl >= 0) {
        _wire->begin(sda, scl);
    } else {
        _wire->begin();
    }

    // Turn on the system oscillator (required before any other command)
    if (!sendCommand(HT16K33_CMD_SYSTEM_SETUP | HT16K33_OSCILLATOR_ON)) {
        return false;
    }

    // Set ROW/INT pins to ROW driver output (not interrupt)
    if (!sendCommand(HT16K33_CMD_ROW_INT)) {
        return false;
    }

    // Clear the display RAM
    clear();
    if (!write()) {
        return false;
    }

    // Turn display on, no blinking
    setBlinkRate(HT16K33_BLINK_OFF);

    // Max brightness
    setBrightness(15);

    return true;
}

void HT16K33_Matrix_8::setBrightness(uint8_t level) {
    if (level > 15) level = 15;
    sendCommand(HT16K33_CMD_DIMMING | level);
}

void HT16K33_Matrix_8::setBlinkRate(uint8_t rate) {
    // rate must be one of the HT16K33_BLINK_* constants (0x00, 0x02, 0x04, 0x06)
    sendCommand(HT16K33_CMD_DISPLAY_SETUP | HT16K33_DISPLAY_ON | (rate & 0x06));
}

void HT16K33_Matrix_8::displayOn() {
    sendCommand(HT16K33_CMD_DISPLAY_SETUP | HT16K33_DISPLAY_ON);
}

void HT16K33_Matrix_8::displayOff() {
    sendCommand(HT16K33_CMD_DISPLAY_SETUP | HT16K33_DISPLAY_OFF);
}

// --- Pixel operations ---

void HT16K33_Matrix_8::setPixel(uint8_t row, uint8_t col, bool on) {
    if (row >= HT16K33_MATRIX_ROWS || col >= HT16K33_MATRIX_COLS) return;
    if (on) {
        _buffer[row] |= (1 << col);
    } else {
        _buffer[row] &= ~(1 << col);
    }
}

bool HT16K33_Matrix_8::getPixel(uint8_t row, uint8_t col) const {
    if (row >= HT16K33_MATRIX_ROWS || col >= HT16K33_MATRIX_COLS) return false;
    return (_buffer[row] >> col) & 1;
}

void HT16K33_Matrix_8::togglePixel(uint8_t row, uint8_t col) {
    if (row >= HT16K33_MATRIX_ROWS || col >= HT16K33_MATRIX_COLS) return;
    _buffer[row] ^= (1 << col);
}

// --- Bulk operations ---

void HT16K33_Matrix_8::setRow(uint8_t row, uint8_t value) {
    if (row >= HT16K33_MATRIX_ROWS) return;
    _buffer[row] = value;
}

uint8_t HT16K33_Matrix_8::getRow(uint8_t row) const {
    if (row >= HT16K33_MATRIX_ROWS) return 0;
    return _buffer[row];
}

void HT16K33_Matrix_8::setColumn(uint8_t col, uint8_t value) {
    if (col >= HT16K33_MATRIX_COLS) return;
    for (uint8_t row = 0; row < HT16K33_MATRIX_ROWS; row++) {
        if (value & (1 << row)) {
            _buffer[row] |= (1 << col);
        } else {
            _buffer[row] &= ~(1 << col);
        }
    }
}

void HT16K33_Matrix_8::fill(bool on) {
    memset(_buffer, on ? 0xFF : 0x00, sizeof(_buffer));
}

void HT16K33_Matrix_8::clear() {
    memset(_buffer, 0, sizeof(_buffer));
}

// --- Scroll ---

void HT16K33_Matrix_8::shiftLeft(uint8_t colData) {
    for (uint8_t row = 0; row < HT16K33_MATRIX_ROWS; row++) {
        uint8_t shifted = _buffer[row] >> 1;
        if (colData & (1 << row)) {
            shifted |= 0x80;
        }
        _buffer[row] = shifted;
    }
}

// --- Bitmap ---

void HT16K33_Matrix_8::drawBitmap4x4(uint8_t startRow, uint8_t startCol, const uint8_t data[4]) {
    for (uint8_t r = 0; r < 4; r++) {
        uint8_t row = startRow + r;
        if (row >= HT16K33_MATRIX_ROWS) break;
        for (uint8_t c = 0; c < 4; c++) {
            uint8_t col = startCol + c;
            if (col >= HT16K33_MATRIX_COLS) break;
            if (data[r] & (1 << c)) {
                _buffer[row] |= (1 << col);
            } else {
                _buffer[row] &= ~(1 << col);
            }
        }
    }
}

void HT16K33_Matrix_8::drawColumns(uint8_t startCol, const uint8_t *data, uint8_t count) {
    for (uint8_t c = 0; c < count; c++) {
        uint8_t col = startCol + c;
        if (col >= HT16K33_MATRIX_COLS) break;
        setColumn(col, data[c]);
    }
}

// --- Hardware write ---

bool HT16K33_Matrix_8::write() {
    if (!_wire) return false;

    _wire->beginTransmission(_addr);
    _wire->write(HT16K33_CMD_RAM); // Start at display RAM address 0x00

    // The HT16K33 display RAM is 16 bytes (8 COM rows x 2 bytes each).
    // RAM layout per COM: [ROW0-ROW7 byte] [ROW8-ROW15 byte]
    //
    // Many 8x8 LED matrix modules have a 1-column offset in how the
    // LED columns map to HT16K33 ROW outputs. We rotate each byte
    // right by 1 bit to correct this, so buffer bit 0 (column 0)
    // aligns with the leftmost physical LED column.
    for (uint8_t row = 0; row < HT16K33_MATRIX_ROWS; row++) {
        uint8_t b = _buffer[row];
        uint8_t rotated = (b >> 1) | ((b & 1) << 7);
        _wire->write(rotated);     // ROW0-ROW7
        _wire->write((uint8_t)0);  // ROW8-ROW15 unused
    }

    return (_wire->endTransmission() == 0);
}

// --- Private ---

bool HT16K33_Matrix_8::sendCommand(uint8_t cmd) {
    if (!_wire) return false;
    _wire->beginTransmission(_addr);
    _wire->write(cmd);
    return (_wire->endTransmission() == 0);
}
