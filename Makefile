GL_PKGS=glfw3 glew
CFLAGS=-Wall -Wextra -Ofast -fopenmp
FILES=game.cpp renderer/setup.c

cli: cli.o
	./cli.o

cli.o: cli.cpp parser.h unit.h units.csv
	g++ $(CFLAGS) cli.cpp -o cli.o

game: game.o
	./game.o

game.o: game.cpp parser.h unit.h
	g++ $(CFLAGS) `pkg-config --cflags $(GL_PKGS)` $(FILES) -o game.o `pkg-config --libs $(GL_PKGS)`
