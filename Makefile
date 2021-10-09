GL_PKGS=glfw3 glew
CFLAGS=-Wall -Wextra -Ofast -fopenmp
FILES=main.cpp renderer/setup.c

run: main.o
	./main.o

main.o: main.cpp parser.h unit.h
	g++ $(CFLAGS) `pkg-config --cflags $(GL_PKGS)` $(FILES) -o main.o `pkg-config --libs $(GL_PKGS)`
