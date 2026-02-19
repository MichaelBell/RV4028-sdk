#include <stdint.h>
#include <stddef.h>

volatile uint8_t* const uart0_dat = (volatile uint8_t*)0x8FFFFF00;
volatile uint8_t* const uart0_flags = (volatile uint8_t*)0x8FFFFF02;

void uart_putc(int c) {
    while (*uart0_flags & 0x20);
    *uart0_dat = c;
}

void uart_puts(const char* c) {
    while (*c) {
        uart_putc(*c++);
    }
    uart_putc('\r');
    uart_putc('\n');
}

void uart_put_buffer(const char* c, int len) {
    while (len-- > 0) {
        if (*c == '\n') uart_putc('\r');
        uart_putc(*c++);
    }
}

int uart_is_char_available(void) {
    return (*uart0_flags & 0x10) == 0;
}

int uart_getc(void) {
    return *uart0_dat;
}
