all: Ex1-A Ex2
Ex1-A: Ex1-A.o gestionFichiers.o
		gcc -lpthread Ex1-A.o gestionFichiers.o -o Ex1
Ex2: Ex2.o gestionFichiers.o
		gcc -lpthread Ex2.o gestionFichiers.o -o Ex2
gestionFichiers.o: gestionFichiers.c gestionFichiers.h
		gcc -c gestionFichiers.c
Ex1-A.o: Ex1-A.c gestionFichiers.h
		gcc -c Ex1-A.c
Ex2.o: Ex2.c gestionFichiers.h
		gcc -c Ex2.c