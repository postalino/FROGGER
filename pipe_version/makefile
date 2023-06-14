Frogger: main.o Collisioni.o Fiume.o NPC.o Stampa.o Strada.o
	gcc main.o Collisioni.o Fiume.o NPC.o Stampa.o Strada.o -o FROGGER -lncurses -lpthread

main.o: main.c Frogger.h
	gcc -c main.c 

collisioni.o: Collisioni.c Collisioni.h Frogger.h
	gcc -c Collisioni.c

fiume.o: Fiume.c Fiume.h Frogger.h
	gcc -c Fiume.c

NPC.o: NPC.c NPC.h Frogger.h
	gcc -c NPC.c 

stampa.o: Stampa.c Stampa.h Frogger.h
	gcc -c Stampa.c 

strada.o: Strada.c Strada.h Frogger.h
	gcc -c Strada.c

clean: 
	rm *.o
	rm FROGGER
	echo "pulizia effettuata.."
