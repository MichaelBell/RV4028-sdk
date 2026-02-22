#pragma once

#include <stdint.h>

void leds_set_pixel(int i, uint8_t r, uint8_t g, uint8_t b);
void leds_update();
void leds_reset(int num_leds);
