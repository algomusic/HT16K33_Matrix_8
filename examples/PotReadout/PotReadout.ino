/*
 * PotReadout.ino
 *
 * Full-display numerical readout (0-99) driven by a potentiometer.
 * Uses 4-column-wide, 8-pixel-tall digit glyphs so two digits
 * fill the 8x8 matrix exactly (4+4 = 8 columns).
 *
 * Single digits (0-9) are centred on the display.
 * Double digits (10-99) fill the full width.
 *
 * Hardware:
 *   - 8x8 LED matrix with HT16K33 backpack
 *   - SDA -> GPIO 18,  SCL -> GPIO 21
 *   - Potentiometer wiper on GPIO 3 (ADC)
 *   - I2C address 0x70
 */

#include <HT16K33_Matrix_8.h>
#include <HT16K33_FontDigits4x8.h>

static const int PIN_SDA = 18;
static const int PIN_SCL = 21;
static const int PIN_POT = 3;

HT16K33_Matrix_8 matrix;

// Display a value 0-99 on the full 8x8 matrix.
void showValue(uint8_t value) {
    matrix.clear();
    if (value > 99) value = 99;

    if (value < 10) {
        // Single digit: centre on display (cols 2-5)
        fontDigits4x8_drawDigit(matrix, value, 2);
    } else {
        // Double digit: tens at cols 0-3, ones at cols 4-7
        fontDigits4x8_drawDigit(matrix, value / 10, 0);
        fontDigits4x8_drawDigit(matrix, value % 10, 4);
    }
    matrix.write();
}

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!matrix.begin(HT16K33_DEFAULT_ADDR, PIN_SDA, PIN_SCL)) {
        Serial.println("HT16K33 not found! Check wiring.");
        while (true) { delay(1000); }
    }

    Serial.println("Pot readout demo ready.");
    matrix.setBrightness(10);
    analogReadResolution(12);
}

void loop() {
    static int8_t lastValue = -1;

    // Read pot and map 0-4095 → 0-99
    uint16_t raw = analogRead(PIN_POT);
    uint8_t value = (uint8_t)((raw * 99UL) / 4095);

    // Only update display when the value changes
    if (value != lastValue) {
        lastValue = value;
        showValue(value);
        Serial.printf("Pot: %4d  Value: %2d\n", raw, value);
    }

    delay(20);
}
