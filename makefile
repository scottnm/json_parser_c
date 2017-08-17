
SRC := $(wildcard *.c)

all:
	gcc -std=gnu11 $(SRC) -o main

clean:
	rm ./main
