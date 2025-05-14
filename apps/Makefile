.PHONY: all
all: hello.elf shell.elf

hello.elf:
	nasm -f elf64 hello/hello.asm -o hello.o
	ld -o $@ hello.o -nostdlib -Ttext=0x400000

shell.elf:
	nasm -f elf64 shell/shell.asm -o shell.o
	ld -o $@ shell.o -nostdlib -Ttext=0x400000

.PHONY: clean
clean:
	rm -rf *.o *.elf