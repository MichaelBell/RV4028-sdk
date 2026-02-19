#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

extern char __StackLimit; /* Set by linker.  */
extern char __HeapStart; /* Set by linker.  */

static uint32_t test_val = 0x13457aef;

int ptr32_test(int max_bytes) {
    uint32_t* ptr = (uint32_t*)&__HeapStart;
    uint32_t* end = (uint32_t*)&__StackLimit;
    if (end - ptr > max_bytes / 4) end = ptr + max_bytes / 4;

    printf("Testing from %p to %p\n", ptr, end);

    uint32_t val = test_val;
    
    while (ptr < end) {
        *ptr++ = val;
        val = (val << 1) | (val >> 31);
    }

    ptr = (uint32_t*)&__HeapStart;
    val = test_val;

    int fails = 0;
    while (ptr < end) {
        uint32_t read_val = *ptr++;
        if (read_val != val) {
            if (!fails) {
                printf("32-bit test fail at %p: wrote %08x, read %08x\n", ptr, val, read_val);
            }
            ++fails;
        }
        val = (val << 1) | (val >> 31);
    }

    return fails;
}

int ptr8_test(int max_bytes) {
    uint8_t* start = (uint8_t*)&__HeapStart;
    uint8_t* end = (uint8_t*)&__StackLimit;
    if (end - start > max_bytes) end = start + max_bytes;

    printf("Testing from %p to %p\n", start, end);

    int fails = 0;

    for (int i = 0; i < 7 && fails == 0; ++i) {
        uint32_t val = (test_val << i) | (test_val >> (32 - i));
        uint8_t* ptr = start + i;

        while (ptr < end) {
            *ptr = val;
            ptr += 7;
            val = (val << 1) | (val >> 31);
        }

        ptr = start + i;
        val = (test_val << i) | (test_val >> (32 - i));

        while (ptr < end) {
            uint8_t read_val = *ptr;
            if (read_val != (val & 0xFF)) {
                if (!fails) {
                    printf("8-bit test fail at %p: wrote %02x, read %02x\n", ptr, val, read_val);
                }
                ++fails;
            }
            ptr += 7;
            val = (val << 1) | (val >> 31);
        }

        if (i > 0) {
            int j = i-1;
            ptr = start + j;
            val = (test_val << j) | (test_val >> (32 - j));

            int fails = 0;
            while (ptr < end) {
                uint8_t read_val = *ptr;
                if (read_val != (val & 0xFF)) {
                    if (!fails) {
                        printf("8-bit test fail at %p: wrote %02x, read %02x\n", ptr, val, read_val);
                    }
                    ++fails;
                }
                ptr += 7;
                val = (val << 1) | (val >> 31);
            }
        }
    }

    return fails;
}

int main(void) {
    printf("Memory test:\n\n");

    int fails = ptr32_test(1024);
    if (fails != 0) {
        printf("Total failures in 256 32-bit words: %d\n", fails);
        return 1;
    }
    else {
        printf("Short test OK\n");
    }
    
    while (true) {
        fails = ptr32_test(8 * 1024 * 1024);
        if (fails != 0) {
            printf("Total failures in 32-bit test: %d\n", fails);
            return 1;
        }
        else {
            printf("Full test OK\n");
        }

        fails = ptr8_test(8 * 1024 * 1024);
        if (fails != 0) {
            printf("Total failures in 8-bit test: %d\n", fails);
            return 1;
        }
        else {
            printf("8-bit test OK\n");
        }

        test_val += 0x11111111;
    }

    return 0;
}
