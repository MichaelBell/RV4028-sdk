#include <stdint.h>
#include <stdbool.h>

volatile uint8_t* uart0_dat = (volatile uint8_t*)0x8FFFFF00;

int main(void) {
    const char* str = "Hello, world!\r\n";

    const char* ptr = str;
    while (*ptr != 0) {
        *uart0_dat = *ptr++;
    }

    while (true) {
        uint8_t c = *uart0_dat;
        if (c) {
            *uart0_dat = c;
            if (c == '\r') *uart0_dat = '\n';
        }
    }
}
