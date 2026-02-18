// Much of this borrowed from the Pico SDK
// Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
// Copyright (c) 2023-2026 Michael Bell

#include <stdint.h>

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __data_lma;

void __runtime_init(void) {
    for (uint32_t* ptr = &__bss_start__; ptr < &__bss_end__; ) *ptr++ = 0;
    
    uint32_t* load_ptr = &__data_lma;
    for (uint32_t* ptr = &__data_start__; ptr < &__data_end__; ) *ptr++ = *load_ptr++;
}
