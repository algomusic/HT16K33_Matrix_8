/*
 * TinyTalkQuadrants.ino
 *
 * Displays four characters on an 8x8 LED matrix using the Tiny Talk 4x4 font:
 *
 *   +------+------+
 *   | char | char |   top: two lowercase letters
 *   +------+------+
 *   | dig  | dig  |   bottom: two digits (0-9)
 *   +------+------+
 *
 * Cycles through the alphabet showing sequential letter pairs
 * (ab, cd, ef ...) with a two-digit counter below (00-99).
 *
 * The Tiny Talk font is lowercase-only, so uppercase input is
 * automatically mapped to lowercase by the lookup function.
 *
 * Hardware:
 *   - 8x8 LED matrix with HT16K33 backpack
 *   - SDA -> GPIO 18
 *   - SCL -> GPIO 21
 *   - I2C address 0x70 (A0=A1=A2 = GND)
 */

#include <HT16K33_Matrix_8.h>
#include <HT16K33_FontTinyTalk.h>

// I2C pin configuration - change to match your wiring
static const int PIN_SDA = 18;
static const int PIN_SCL = 21;

HT16K33_Matrix_8 matrix;

// Display four characters in the four quadrants and push to hardware
void showQuadrants(char topLeft, char topRight, char botLeft, char botRight) {
    matrix.clear();
    tinyTalk_drawChar(matrix, topLeft,  0, 0);
    tinyTalk_drawChar(matrix, topRight, 0, 4);
    tinyTalk_drawChar(matrix, botLeft,  4, 0);
    tinyTalk_drawChar(matrix, botRight, 4, 4);
    matrix.write();
}

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!matrix.begin(HT16K33_DEFAULT_ADDR, PIN_SDA, PIN_SCL)) {
        Serial.println("HT16K33 not found! Check wiring.");
        while (true) { delay(1000); }
    }

    Serial.println("Tiny Talk four-quadrant demo ready.");
    matrix.setBrightness(8);
}

void loop() {
    static uint8_t counter = 0;

    // Show sequential letter pairs: ab, cd, ef, ...
    for (uint8_t i = 0; i < 26; i += 2) {
        char left  = 'a' + i;
        char right = (i + 1 < 26) ? 'a' + i + 1 : 'a';

        char tens = '0' + (counter / 10);
        char ones = '0' + (counter % 10);

        showQuadrants(left, right, tens, ones);

        Serial.printf("%c%c %c%c\n", left, right, tens, ones);

        counter = (counter + 1) % 100;
        delay(800);
    }
}
