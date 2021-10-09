run: main.o
	./main.o

main.o: main.cpp parser.h unit.h
	g++ -Ofast -fopenmp main.cpp -o main.o
