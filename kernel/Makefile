CC := clang
LD := ld

SOURCES := $(wildcard ./src/**.c)
OBJECTS := $(SOURCES:.c=.o)

OUTPUT := kernel.elf

.PHONY: all
all: $(OUTPUT)

CFLAGS := \
	-Wall \
	-Wextra \
	-Werror \
	-std=gnu11 \
	-mno-red-zone \
	-fno-pic \
	-ffreestanding \
	-fno-stack-protector \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

LDFLAGS := \
	-nostdlib \
	-z max-page-size=0x1000 \
	-T linker.ld

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT): $(OBJECTS)
	nasm -f elf64 src/gdt.asm -o src/a_gdt.o
	nasm -f elf64 src/idt.asm -o src/a_idt.o
	$(LD) $(LDFLAGS) $(OBJECTS) src/a_gdt.o src/a_idt.o -o $@

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(OUTPUT) src/*.o