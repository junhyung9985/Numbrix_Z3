numbrixsolve : numbrix.o
	gcc -o numbrixsolve numbrix.o

numbrix.o : main.c
	gcc -c -o numbrix.o main.c
clean :
	rm *.o quiz
