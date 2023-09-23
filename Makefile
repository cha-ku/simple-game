SRC_FILES = ./src/*.cpp \
	src/ECS/*.cpp \
	src/GameState/*.cpp \
	src/System/* \
	src/Logger/*.cpp

build:
	g++ -g -Wall -Wfatal-errors --pedantic -std=c++20 $(SRC_FILES) -lSDL2 -lSDL2_image -o stabby2d

build-asan:
	g++ -g -Wall -Wfatal-errors --pedantic -fsanitize=address -std=c++20 $(SRC_FILES) -lSDL2 -lSDL2_image -o stabby2d

run: build
	./stabby2d

clean:
	rm stabby2d
