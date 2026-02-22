#include <stdint.h>
#include <stdbool.h>

#define RV4028_MAGIC (0x52563238)
#define RV4028_MAGIC2 (0x40284028)

int printf(const char* fmt, ...);
void flush_uart();

static void read_flash(uint32_t addr, uint32_t len, uint8_t* buf) {
    volatile uint8_t* flash_ptr = (volatile uint8_t*)0x8FFFFF90;
    flash_ptr[2] = addr & 0xff;
    flash_ptr[4] = (addr >> 8) & 0xff;
    flash_ptr[6] = (addr >> 16) & 0xff;

    for (uint32_t i = 0; i < len; ++i) {
        buf[i] = *flash_ptr;
    }
}

static void reset_leds() {
    // Reset a string of up to 6 LEDs to off
    volatile uint8_t* ws2812_ctrl = (volatile uint8_t*)0x8FFFFF80;
    *ws2812_ctrl = 254;
    for (uint8_t i = 0; i < 6; ++i) {
        ws2812_ctrl[0] = i;
        ws2812_ctrl[2] = 0;
        ws2812_ctrl[4] = 0;
        ws2812_ctrl[6] = 0;
        (void)*ws2812_ctrl;
    }
    *ws2812_ctrl = 255;
}

int main(void) {
    printf("==  RV4028 Bootloader: Loading program ...                ==\n");

    reset_leds();

    uint32_t magic, magic2;
    read_flash(0, 4, (uint8_t*)&magic);
    read_flash(12, 4, (uint8_t*)&magic2);
    if (magic != RV4028_MAGIC || magic2 != RV4028_MAGIC2) {
        printf("No RV4028 program found.  Magic value %08x:%08x != %08x:%08x\n", magic, magic2, RV4028_MAGIC, RV4028_MAGIC2);
        return 1;
    }

    uint32_t len;
    read_flash(4, 4, (uint8_t*)&len);
    if (len < 4 || len > 0x100000) {
        printf("RV4028 program length out of range.  Got length %d (0x%x)\n", len, len);
        return 1;
    }

    uint32_t check;
    read_flash(8, 4, (uint8_t*)&check);

    uint8_t* prog_load_addr = (uint8_t*)0;
    read_flash(16, len, prog_load_addr);

    uint32_t* prog = (uint32_t*)0;
    for (int i = 0; i < (len + 3) / 4; ++i) {
        check ^= prog[i];
    }

    if (check != 0) {
        printf("RV4028 program bad checksum.  %08x != 0\n", check);
        return 1;
    }

    printf("=   RV4028 Bootloader: Load complete.  Starting program.   =\n");
    flush_uart();

    return 0;
}
