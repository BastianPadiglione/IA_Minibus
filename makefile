EXEC=IA
CFLAGS=-Wall -Wextra -std=c99
LDFLAGS=


all:: $(EXEC) clean

IA: main.o recuperationDeDonnees.o gestionListeChaine.o verifications.o 
	gcc -o ../Minibus/ligueIA/IA main.o recuperationDeDonnees.o gestionListeChaine.o verifications.o $(LDFLAGS)

main.o: main.c main.h
	gcc -o main.o -c main.c $(CFLAGS)

recuperationDeDonnees.o: recuperationDeDonnees.c recuperationDeDonnees.h
	gcc -o recuperationDeDonnees.o -c recuperationDeDonnees.c $(CFLAGS)

gestionListeChaine.o: gestionListeChaine.c gestionListeChaine.h
	gcc -o gestionListeChaine.o -c gestionListeChaine.c $(CFLAGS)

verifications.o: verifications.c verifications.h
	gcc -o verifications.o -c verifications.c $(CFLAGS)

clean::
	rm -rf *.o

mrproper:: clean
	rm -rf $(EXEC)