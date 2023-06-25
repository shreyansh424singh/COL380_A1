all:
	g++ -fopenmp -O2 -c main.cpp -o main.o
	g++ -fopenmp main.o library.so -o exec -Wl,-rpath=.