SRC_FILES = ./src/*.cpp \
	src/ECS/*.cpp \
	src/GameState/*.cpp \
	src/Logger/*.cpp

build:
	g++ -g -fsanitize=address -Wall -Wfatal-errors -std=c++20 $(SRC_FILES) -lSDL2 -lSDL2_image -o stabby2d

run:
	./stabby2d

clean:
	rm stabby2d
