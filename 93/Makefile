main: main.o
	gcc -o main main.o -no-pie

main.o:
	nasm -f elf64 -o main.o main.asm
