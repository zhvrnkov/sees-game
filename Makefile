run: main.o
	./main.o

main.o: main.cpp
	g++ main.cpp -o main.o
