
SRC := $(wildcard *.c)

all:
	gcc -std=gnu11 $(SRC) -o main

.PHONY: test
test: all
	python3.5 test/test_runner.py

clean:
	rm ./main
