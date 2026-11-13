#include "main.h"
#include "leds.hpp"

// WS2812B LED strip on 3-wire port C
static const char LED_PORT = 'C';
static const int LED_COUNT = 60; // set to your strip length (max 64)
static const int LED_BLOCK = 4; // LEDs per color block
static const uint32_t LED_RED = 0x800000; // 50% brightness
static const uint32_t LED_WHITE = 0x808080;

static pros::adi::Led ledStrip(LED_PORT, LED_COUNT);

// running red and white pattern
static void ledTask() {
    int shift = 0;
    while (true) {
        for (int i = 0; i < LED_COUNT; i++) {
            bool isRed = ((i + shift) / LED_BLOCK) % 2 == 0;
            ledStrip[i] = isRed ? LED_RED : LED_WHITE;
        }
        ledStrip.update();
        shift = (shift + 1) % (LED_BLOCK * 2);
        pros::delay(60); // lower = faster
    }
}

void startLeds() {
    static bool started = false;
    if (started) return; // only ever start one LED task
    started = true;
    pros::Task led_task(ledTask);
}
