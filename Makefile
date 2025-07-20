build:
	g++ -Wall -std=c++17 \
		`sdl2-config --cflags` \
		-I./libs \
		src/*.cpp \
		`sdl2-config --libs` \
		-lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua \
		-o gameengine
run:
	./gameengine

clean:
	rm gameengine
