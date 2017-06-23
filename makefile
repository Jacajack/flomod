all: src/chsb.c src/flomod.c
	gcc -o flomod -Wall $^
