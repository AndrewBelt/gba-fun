
PATH := ../devkitARM/bin:$(PATH)

ARCH := -mthumb-interwork -mthumb
SPECS := -specs=gba.specs

# Flags

CFLAGS  := $(ARCH) -O2 -fno-strict-aliasing -std=c99 \
	-Wall -Wno-pointer-to-int-cast \
	-I../libgba/include

LDFLAGS := $(ARCH) $(SPECS) \
	-lgba \
	-L../libgba/lib

# Custom files

TARGET = fun.gba
OBJCOPY_FLAGS := -O elf32-littlearm -B arm
GRIT_FLAGS := -pT4 -gB4 -Mw4 -Mh4 -ftb -fh!
VBAM_FLAGS := --bios=../bios/gba.bin

ASSETS := assets/fun.txt pokemon.pal.bin pokemon.img.bin
SRCS := src/fun.c
OBJECTS := $(SRCS:=.o)

# Targets

build: assets/pokemon.gif.grit $(TARGET)

%.elf: $(OBJECTS) assets.bin.o
	arm-none-eabi-gcc $^ $(LDFLAGS) -o $@

assets.bin.o: $(ASSETS)
	arm-none-eabi-ld -r -b binary -o $@ $^
	arm-none-eabi-objcopy --rename-section .data=.rodata $@ $@

%.gba: %.elf
	arm-none-eabi-objcopy -O binary $^ $@
	gbafix $@

%.grit: %
	grit $^ $(GRIT_FLAGS)

%.c.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $^ -o $@


# Auxiliary targets

clean:
	@rm -fv *.gba *.elf *.bin *.o **/*.o

run: $(TARGET)
	vbam $(VBAM_FLAGS) $^