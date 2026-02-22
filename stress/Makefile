PROJECT_NAME ?= memtest

PROJECT_SOURCES ?= memtest.c

RISCV_TOOLCHAIN ?= /opt/rv4028

CC = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-gcc
AS = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-as
AR = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-ar
LD = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-ld
OBJCOPY = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-objcopy

RV4028_SDK = ..

all: $(PROJECT_NAME).uf2

clean:
	cd $(dir $(PROJECT_NAME)) && rm *.o *.elf *.bin *.hex

%.o: %.c
	$(CC) -O2 -march=rv32i -I$(RV4028_SDK) -nostdlib -nostartfiles -ffreestanding -ffunction-sections -fdata-sections -Wall -Werror -Wno-format -lc -c $< -o $@

%.o: %.s
	$(AS) -march=rv32i_zicsr $< -o $@

$(PROJECT_NAME).elf: $(PROJECT_SOURCES:.c=.o) 
	$(LD) $^ $(RV4028_SDK)/start.o $(RV4028_SDK)/rv4028.a $(RISCV_TOOLCHAIN)/riscv32-unknown-elf/lib/libc.a $(RISCV_TOOLCHAIN)/riscv32-unknown-elf/lib/libm.a $(RISCV_TOOLCHAIN)/lib/gcc/riscv32-unknown-elf/*/libgcc.a -T memmap --gc-sections -o $@

$(PROJECT_NAME).bin: $(PROJECT_NAME).elf
	$(OBJCOPY) $< -O binary $@

$(PROJECT_NAME).uf2: $(PROJECT_NAME).bin
	$(RV4028_SDK)/mkuf2.py $< $@
