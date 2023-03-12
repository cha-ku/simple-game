build:
	g++ -Wall -Werror -std=c++20 src/*.cpp src/GameState/*.cpp src/Logger/*.cpp -lSDL2 -lSDL2_image -o stabby2d

run:
	./stabby2d

clean:
	rm stabby2d
