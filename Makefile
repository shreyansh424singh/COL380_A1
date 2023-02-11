all:
	export OMP_NUM_THREADS=4
	g++ -fopenmp -O2 -c main.cpp -o main.o
	g++ -fopenmp main.o library1.so -o a.out -Wl,-rpath=.
	./a.out 