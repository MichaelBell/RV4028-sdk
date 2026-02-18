PROJECT_NAME ?= bootload

PROJECT_SOURCES ?= bootload.c print.c

RISCV_TOOLCHAIN ?= /opt/rv4028

CC = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-gcc
AS = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-as
AR = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-ar
LD = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-ld
OBJCOPY = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-objcopy

all: $(PROJECT_NAME).bin $(PROJECT_NAME).hex

clean:
	cd $(dir $(PROJECT_NAME)) && rm *.o *.elf *.bin *.hex

%.o: %.c
	$(CC) -Os -march=rv32i -nostdlib -nostartfiles -ffreestanding -ffunction-sections -fdata-sections -Wall -Werror -lc -c $< -o $@

%.o: %.s
	$(AS) -march=rv32i $< -o $@

$(PROJECT_NAME).elf: $(PROJECT_SOURCES:.c=.o) start.o runtime_init.o
	$(LD) $^ $(RISCV_TOOLCHAIN)/lib/gcc/riscv32-unknown-elf/*/libgcc.a -T memmap --gc-sections -o $@

$(PROJECT_NAME).bin: $(PROJECT_NAME).elf
	$(OBJCOPY) $< -O binary $@

$(PROJECT_NAME).hex: $(PROJECT_NAME).bin
	od -An -t x4 -w4 -v $< > $@
