run: main.o
	./main.o

main.o: main.cpp
	g++ -Ofast -fopenmp main.cpp -o main.o
