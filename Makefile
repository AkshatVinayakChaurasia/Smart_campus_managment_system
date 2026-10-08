CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude
SRC      := $(filter-out src/main.cpp, $(wildcard src/*.cpp))
OBJ      := $(patsubst src/%.cpp, build/%.o, $(SRC))

all: smartcampus

smartcampus: $(OBJ) build/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(OBJ) tests/test_modules.cpp
	$(CXX) $(CXXFLAGS) -o build/run_tests $^
	./build/run_tests

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build smartcampus

.PHONY: all test clean
