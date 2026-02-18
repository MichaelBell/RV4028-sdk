#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0
#define NANOPRINTF_SNPRINTF_SAFE_TRIM_STRING_ON_OVERFLOW 1

// Compile nanoprintf in this translation unit.
#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

volatile uint8_t* const uart0_dat = (volatile uint8_t*)0x8FFFFF00;
volatile uint8_t* const uart0_flags = (volatile uint8_t*)0x8FFFFF02;

static void uart_putc(int c, void*) {
    while (*uart0_flags & 0x20);
    if (c == '\n') {
        *uart0_dat = '\r';
        while (*uart0_flags & 0x20);
    }
    *uart0_dat = c;
}

int printf(const char* fmt, ...) {
    va_list val;
    va_start(val, fmt);
    int const rv = npf_vpprintf(&uart_putc, NULL, fmt, val);
    va_end(val);
    return rv;
}

void flush_uart() {
    while ((*uart0_flags & 0x80) != 0x80);
}