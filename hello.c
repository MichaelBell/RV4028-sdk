#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Hello, world!\n");

    while (true) {
        // Using getc(stdin) doesn't seem to work, I'm not sure why
        char c;
        int len = read(0, &c, 1);
        if (len) {
            putc(c, stdout);
            if (c == '\r') putc('\n', stdout);
            else fflush(stdout);
        }
    }
}
