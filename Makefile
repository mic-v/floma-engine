build:
	g++ -Wall -std=c++17 -I"./libs/" src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -llua5.3 -o engine;

run:
	./engine

clean:
	rm engine