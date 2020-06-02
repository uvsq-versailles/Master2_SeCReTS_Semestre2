exec: compil1 compil2
	./main

compil1:
	gcc -c moyenne.s -o moyenne.o
	gcc -c moyenneopti.s -o moyenneopti.o 
	gcc -c main.c

compil2:
	gcc -o main main.o moyenne.o #moyenneopti.o -fPIE

