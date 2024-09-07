CXX?=g++
SDL2FLAGS=$(shell pkg-config sdl2 --cflags --libs)
SDL2TTFFLGS=$(shell pkg-config SDL2_ttf --cflags --libs)
CXXFLAGS?=-std=c++11 -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow

output: main.o Vector2.o
	g++ main.o Vector2.o -o output

main.o: main.cpp
	g++ -c main.cpp

Vector2.o: message.cpp message.h
	g++ -c Vector2.cpp

clean: 