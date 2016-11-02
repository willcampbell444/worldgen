CC=g++ -g

all: build/main.o build/groundGen.o build/shaders.o build/visualize.o
	$(CC) build/shaders.o build/groundGen.o build/main.o build/visualize.o -lGL -lglfw -lGLEW -DGLEW_STATIC -Iheaders

build/shaders.o: src/shaders.cpp headers/shaders.h
	$(CC) -c src/shaders.cpp -o build/shaders.o -Iheaders

build/main.o: src/main.cpp headers/groundGen.h
	$(CC) -c src/main.cpp -o build/main.o -Iheaders

build/groundGen.o: src/groundGen.cpp headers/groundGen.h
	$(CC) -c src/groundGen.cpp -o build/groundGen.o -Iheaders

build/visualize.o: src/visualize.cpp headers/visualize.h headers/groundGen.h headers/shaders.h
	$(CC) -c src/visualize.cpp -o build/visualize.o -Iheaders

clean:
	rm build/*