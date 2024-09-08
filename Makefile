CXX?=g++
SDL2TTFFLGS=$(shell pkg-config SDL2_ttf --cflags --libs)
CXXFLAGS?=-std=c++11 -Wall -pedantic -Wshadow

.PHONY: clean run main debug

# all: build

main: CXXFLAGS+=-O2

debug: CXXFLAGS+=-g

main debug: build

build: main.o Vector2.o
	${CXX} main.o Vector2.o -o output ${CXXFLAGS} ${SDL2TTFFLGS}

main.o: main.cpp 
	${CXX} -c main.cpp ${CXXFLAGS}

Vector2.o: Vector2.cpp Vector2.h
	${CXX} -c Vector2.cpp ${CXXFLAGS}

clean: 
	rm *.o output

run:
	./output