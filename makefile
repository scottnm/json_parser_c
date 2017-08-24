
SRC := $(wildcard *.cpp)

WARNINGS := -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused

all:
	g++ $(WARNINGS) -std=c++14 $(SRC) -o main

.PHONY: test
test: all
	./test/success_tests.sh
	python3.5 test/test_runner.py

clean:
	rm ./main
