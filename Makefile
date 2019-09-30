numbrixsolve : numbrix.o
	gcc -o numbrixsolve main.o

numbrix.o : main.c
	gcc -c -o main.o main.c
clean :
	rm *.o numbrixsolve
