.PHONY: all
all: hello.elf

hello.elf:
	nasm -f elf64 hello/hello.asm -o hello.o
	ld -o $@ hello.o -nostdlib -Ttext=0x400000

.PHONY: clean
clean:
	rm -rf *.o *.elf