/*
 * ScrollingFullDisplay.ino
 *
 * Scrolls text across the full 8x8 LED matrix using the variable-width
 * 8-pixel-tall font. Text enters from the right and exits to the left,
 * one column at a time.
 *
 * Randomly chooses from: "filter", "oscillator", "envelope", "delay"
 *
 * Hardware:
 *   - 8x8 LED matrix with HT16K33 backpack
 *   - SDA -> GPIO 18
 *   - SCL -> GPIO 21
 *   - I2C address 0x70 (A0=A1=A2 = GND)
 */

#include <HT16K33_Matrix_8.h>
#include <HT16K33_FontFull.h>

static const int PIN_SDA = 18;
static const int PIN_SCL = 21;
static const uint16_t SCROLL_DELAY_MS = 80;

HT16K33_Matrix_8 matrix;

const char *words[] = {"filter", "oscillator", "envelope", "delay"};
const uint8_t wordCount = 4;

// Scroll a complete text string across the display.
// Includes leading and trailing blank columns so the text
// enters from the right and fully exits to the left.
void scrollText(const char *text) {
    // 8 blank columns to scroll text in from the right
    for (uint8_t i = 0; i < 8; i++) {
        matrix.shiftLeft();
        matrix.write();
        delay(SCROLL_DELAY_MS);
    }

    for (const char *p = text; *p; p++) {
        uint8_t cols[FONTFULL_MAX_WIDTH];
        uint8_t width = fontFull_getChar(*p, cols);

        // Push each column of this character
        for (uint8_t c = 0; c < width; c++) {
            matrix.shiftLeft(cols[c]);
            matrix.write();
            delay(SCROLL_DELAY_MS);
        }

        // 1-column gap between characters
        matrix.shiftLeft();
        matrix.write();
        delay(SCROLL_DELAY_MS);
    }

    // 8 blank columns to scroll text off to the left
    for (uint8_t i = 0; i < 8; i++) {
        matrix.shiftLeft();
        matrix.write();
        delay(SCROLL_DELAY_MS);
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!matrix.begin(HT16K33_DEFAULT_ADDR, PIN_SDA, PIN_SCL)) {
        Serial.println("HT16K33 not found! Check wiring.");
        while (true) { delay(1000); }
    }

    Serial.println("Full-display scrolling demo ready.");
    matrix.setBrightness(8);
    randomSeed(analogRead(0));
}

void loop() {
    uint8_t idx = random(wordCount);
    Serial.printf("Scrolling: %s\n", words[idx]);
    scrollText(words[idx]);
    delay(200);
}
