/*
 * WordBlocks.ino
 *
 * Displays 4-character words across the four quadrants of an 8x8
 * LED matrix using the 3x3 font. Each word is held for 1 second
 * before being replaced by a randomly chosen new word.
 *
 *   +------+------+
 *   | ch 0 | ch 1 |
 *   +------+------+
 *   | ch 2 | ch 3 |
 *   +------+------+
 *
 * Words shorter than 4 characters are padded with spaces (blank quadrants).
 *
 * Word list: "filt", "res ", "osc1", "sqr", "synt"
 *
 * Hardware:
 *   - 8x8 LED matrix with HT16K33 backpack
 *   - SDA -> GPIO 18
 *   - SCL -> GPIO 21
 *   - I2C address 0x70 (A0=A1=A2 = GND)
 */

#include <HT16K33_Matrix_8.h>
#include <HT16K33_Font3x3.h>

static const int PIN_SDA = 18;
static const int PIN_SCL = 21;

HT16K33_Matrix_8 matrix;

const char *words[] = {"filt", "res ", "osc1", "sqr", "synt"};
const uint8_t wordCount = 5;

void showWord(const char *word) {
    uint8_t len = strlen(word);
    matrix.clear();
    if (len > 0) font3x3_drawChar(matrix, word[0], 0, 0);
    if (len > 1) font3x3_drawChar(matrix, word[1], 0, 4);
    if (len > 2) font3x3_drawChar(matrix, word[2], 4, 0);
    if (len > 3) font3x3_drawChar(matrix, word[3], 4, 4);
    matrix.write();
}

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!matrix.begin(HT16K33_DEFAULT_ADDR, PIN_SDA, PIN_SCL)) {
        Serial.println("HT16K33 not found! Check wiring.");
        while (true) { delay(1000); }
    }

    Serial.println("Word blocks demo ready.");
    matrix.setBrightness(8);
    randomSeed(analogRead(0));
}

void loop() {
    uint8_t idx = random(wordCount);
    showWord(words[idx]);
    Serial.printf("Showing: %s\n", words[idx]);
    delay(1000);
}
