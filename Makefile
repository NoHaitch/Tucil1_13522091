clean:
	rm -f bin/*

build: create_dir
	g++ src/main.cpp -o bin/main

run:
	bin/main

create_dir:
	mkdir -p bin

all: clean build run