## Created by Jacob Strieb
## July 2021

CFLAGS = -s -static

elf: elf.c

print: print.s
	$(CC) $(CFLAGS) -nostdlib "$<" -o "$@"
