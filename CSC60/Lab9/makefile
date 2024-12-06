# Jeremy Vuong. lab9.

lab9: lab9.o ParseCmd.o
	gcc -o lab9 lab9.o ParseCmd.o -Wall
lab9.o: lab9.c lab9_10.h
	gcc -c lab9.c -Wall
ParseCmd.o: ParseCmd.c lab9_10.h
	gcc -c ParseCmd.c -Wall

