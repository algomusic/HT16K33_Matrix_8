/*
 * BasicPixelControl.ino
 *
 * Demonstrates the HT16K33_Matrix_8 library on an ESP32-S3.
 * Cycles through several display patterns:
 *   1. Individual pixel walk
 *   2. Row sweep
 *   3. Column sweep
 *   4. Checkerboard toggle
 *   5. Brightness fade
 *   6. Querying pixel state via getPixel()
 *
 * Hardware:
 *   - 8x8 LED matrix with HT16K33 backpack
 *   - SDA -> GPIO 18
 *   - SCL -> GPIO 21
 *   - I2C address 0x70 (A0=A1=A2 = GND)
 */

#include <HT16K33_Matrix_8.h>

// I2C pin configuration - change these to match your wiring
static const int PIN_SDA = 18;
static const int PIN_SCL = 21;

HT16K33_Matrix_8 matrix;

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!matrix.begin(HT16K33_DEFAULT_ADDR, PIN_SDA, PIN_SCL)) {
        Serial.println("HT16K33 not found! Check wiring.");
        while (true) { delay(1000); }
    }

    Serial.println("HT16K33 8x8 matrix ready.");
    matrix.setBrightness(8);
}

void loop() {
    pixelWalk();
    rowSweep();
    columnSweep();
    checkerboard();
    brightnessFade();
    pixelQueryDemo();
}

// Light each LED one at a time across the matrix
void pixelWalk() {
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            matrix.clear();
            matrix.setPixel(row, col, true);
            matrix.write();
            delay(40);
        }
    }
}

// Fill one row at a time from top to bottom, then clear
void rowSweep() {
    matrix.clear();
    for (uint8_t row = 0; row < 8; row++) {
        matrix.setRow(row, 0xFF);
        matrix.write();
        delay(100);
    }
    delay(300);
    for (uint8_t row = 0; row < 8; row++) {
        matrix.setRow(row, 0x00);
        matrix.write();
        delay(100);
    }
}

// Fill one column at a time from left to right, then clear
void columnSweep() {
    matrix.clear();
    for (uint8_t col = 0; col < 8; col++) {
        matrix.setColumn(col, 0xFF);
        matrix.write();
        delay(100);
    }
    delay(300);
    for (uint8_t col = 0; col < 8; col++) {
        matrix.setColumn(col, 0x00);
        matrix.write();
        delay(100);
    }
}

// Alternate between two checkerboard patterns
void checkerboard() {
    for (int i = 0; i < 6; i++) {
        for (uint8_t row = 0; row < 8; row++) {
            matrix.setRow(row, (row + i) % 2 == 0 ? 0xAA : 0x55);
        }
        matrix.write();
        delay(400);
    }
}

// Fade brightness up and down
void brightnessFade() {
    matrix.fill(true);
    matrix.write();
    for (uint8_t b = 0; b <= 15; b++) {
        matrix.setBrightness(b);
        delay(80);
    }
    for (int b = 15; b >= 0; b--) {
        matrix.setBrightness((uint8_t)b);
        delay(80);
    }
    matrix.setBrightness(8);
    matrix.clear();
    matrix.write();
}

// Show how getPixel() can query the buffer state
void pixelQueryDemo() {
    matrix.clear();

    // Draw a diagonal line
    for (uint8_t i = 0; i < 8; i++) {
        matrix.setPixel(i, i, true);
    }
    matrix.write();
    delay(500);

    // Toggle every pixel: diagonal goes off, everything else turns on
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            matrix.togglePixel(row, col);
        }
    }
    matrix.write();
    delay(500);

    // Print the buffer state to Serial to demonstrate getPixel()
    Serial.println("Pixel state after toggle (inverted diagonal):");
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            Serial.print(matrix.getPixel(row, col) ? '#' : '.');
        }
        Serial.println();
    }
    Serial.println();
    delay(1000);
}
