CC=g++ -g

all: build/main.o build/groundGen.o
	$(CC) build/groundGen.o build/main.o

build/main.o: src/main.cpp headers/groundGen.h
	$(CC) -c src/main.cpp -o build/main.o -Iheaders

build/groundGen.o: src/groundGen.cpp headers/groundGen.h
	$(CC) -c src/groundGen.cpp -o build/groundGen.o -Iheaders