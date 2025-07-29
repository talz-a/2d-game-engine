CXX = g++-15
LANG_STD = -std=c++23
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATHS = -I./libs
SRC_FILES = ./src/*.cpp \
			./src/Game/*.cpp \
			./src/Logger/*.cpp \
			./src/ECS/*.cpp \
			./src/AssetStore/*.cpp
SDL_CFLAGS = `sdl2-config --cflags`
SDL_LIBS = `sdl2-config --libs`
LINKER_FLAGS = -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua
OBJ_NAME = gameengine

build:
	$(CXX) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SRC_FILES) $(SDL_CFLAGS) $(SDL_LIBS) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
