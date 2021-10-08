run: main.o
	./main.o

main.o: main.cpp
	g++ -Ofast main.cpp -o main.o
