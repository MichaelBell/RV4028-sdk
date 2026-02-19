#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
    printf("Hello, world!\n");

    while (true) {
        int c = getc(stdin);
        if (c) {
            putc(c, stdout);
            if (c == '\r') putc('\n', stdout);
        }
    }
}
