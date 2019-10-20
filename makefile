all: main

main: src/main.cpp src/server.cpp src/player.cpp
	g++ -o $@ $^
