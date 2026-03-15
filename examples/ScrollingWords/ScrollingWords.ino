/*
 * ScrollingWords.ino
 *
 * Smooth pixel-by-pixel scrolling of words across the top half of an
 * 8x8 LED matrix using the 3x3 font, while showing a two-digit
 * counter in the bottom half.
 *
 * The word scrolls left one column at a time through the top 4 rows.
 * Each 3x3 font character is 4 columns wide (3 content + 1 spacing),
 * so letters flow smoothly across the display.
 *
 * Words are randomly chosen from: "filter", "oscillator", "envelope", "delay"
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

const char *words[] = {"filter", "oscillator", "envelope", "delay"};
const uint8_t wordCount = 4;

uint8_t counter = 0;

// Draw the bottom-half counter digits
void drawCounter() {
    font3x3_drawChar(matrix, '0' + (counter / 10), 4, 0);
    font3x3_drawChar(matrix, '0' + (counter % 10), 4, 4);
}

// Smooth-scroll a word across the top half (rows 0-3), one column per frame.
// The word enters from the right and exits to the left.
void scrollWord(const char *word) {
    uint8_t len = strlen(word);
    uint8_t wordWidth = len * 4;       // total pixel columns in the word
    uint8_t totalFrames = wordWidth + 8; // +8 so it fully scrolls off both sides

    for (uint8_t frame = 0; frame < totalFrames; frame++) {
        // Clear the top 4 rows
        for (uint8_t r = 0; r < 4; r++) {
            matrix.setRow(r, 0);
        }

        // For each visible display column, work out which virtual column
        // of the word it corresponds to and set the pixel if needed.
        for (uint8_t dispCol = 0; dispCol < 8; dispCol++) {
            // Virtual column in the word bitmap.
            // frame=0 → word starts just off-screen to the right;
            // subtracting 7 so the first column appears at dispCol 7.
            int virtCol = (int)frame + (int)dispCol - 7;

            if (virtCol < 0 || virtCol >= (int)wordWidth) continue;

            uint8_t charIdx   = virtCol / 4;
            uint8_t colInChar = virtCol % 4;

            uint8_t glyph[4];
            if (!font3x3_getGlyph(word[charIdx], glyph)) continue;

            for (uint8_t r = 0; r < 4; r++) {
                if (glyph[r] & (1 << colInChar)) {
                    matrix.setPixel(r, dispCol, true);
                }
            }
        }

        drawCounter();
        matrix.write();
        delay(250);
    }

    counter = (counter + 1) % 100;
}

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!matrix.begin(HT16K33_DEFAULT_ADDR, PIN_SDA, PIN_SCL)) {
        Serial.println("HT16K33 not found! Check wiring.");
        while (true) { delay(1000); }
    }

    Serial.println("Scrolling words demo ready.");
    matrix.setBrightness(8);
    randomSeed(analogRead(0));
}

void loop() {
    uint8_t idx = random(wordCount);
    const char *word = words[idx];
    Serial.printf("Scrolling: %s\n", word);
    scrollWord(word);
    delay(300);
}
