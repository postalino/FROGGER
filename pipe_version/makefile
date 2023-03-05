Frogger: main.o collisioni.o fiume.o NPC.o stampa.o strada.o
	gcc main.o collisioni.o fiume.o NPC.o stampa.o strada.o -lncurses

main.o: main.c Frogger.h
	gcc -c main.c 

collisioni.o: collisioni.c collisioni.h Frogger.h
	gcc -c collisioni.c

fiume.o: fiume.c fiume.h Frogger.h
	gcc -c fiume.c

NPC.o: NPC.c NPC.h Frogger.h
	gcc -c NPC.c 

stampa.o: stampa.c stampa.h Frogger.h
	gcc -c stampa.c 

strada.o: strada.c strada.h Frogger.h
	gcc -c strada.c

clean: 
	rm *.o
	rm *.out
	echo "pulizia effettuata.."
