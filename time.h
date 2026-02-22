#pragma once

#include <stdint.h>

inline static uint32_t read_cycle() {
    int r;
    asm volatile ("rdcycle %0" :
         [r] "=r" (r) 
         );
    return r;
}

inline static void sleep_us(uint32_t time_us) {
    // Assume 15MHz clock
    uint32_t delay_cycles = time_us * 15;

    // Rough guess of the overheads.
    if (delay_cycles <= 50) return;
    delay_cycles -= 50;

    uint32_t start_cycle = read_cycle();
    while ((read_cycle() - start_cycle) < delay_cycles);
}
