#include <stdint.h>
#include "time.h"

volatile uint8_t* const ws2812_ctrl = (volatile uint8_t*)0x8FFFFF80;

#define MAX_LEDS 250

void leds_set_pixel(int i, uint8_t r, uint8_t g, uint8_t b) {
    if (i >= MAX_LEDS) return;

    ws2812_ctrl[0] = i;
    ws2812_ctrl[2] = r;
    ws2812_ctrl[4] = g;
    ws2812_ctrl[6] = b;
    (void)*ws2812_ctrl;
}


void leds_update() {
    const uint32_t min_cycles_between_updates = 6000;
    static uint32_t last_update_cycle = 0;

    uint32_t cycle = read_cycle();
    while (cycle - last_update_cycle < min_cycles_between_updates) cycle = read_cycle();
    last_update_cycle = cycle;

    ws2812_ctrl[0] = 255;
    (void)*ws2812_ctrl;
}

void leds_reset(int num_leds) {
    ws2812_ctrl[0] = 254;
    for (int i = 0; i < num_leds; ++i) {
        leds_set_pixel(i, 0, 0, 0);
    }
    leds_update();
}
