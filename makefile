all: src/chsb.c src/flomod.c src/args.c
	gcc -o flomod -Wall -g $^
