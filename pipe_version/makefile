esegubile: main.o npc.o fiume.o strada.o collisioni.o gestione_grafica.o
	gcc main.o npc.o fiume.o strada.o collisioni.o gestione_grafica.o -lncurses 

main.o: main.c
	gcc -c -g main.c -lncurses

npc.o: npc.c npc.h
	gcc -c -g npc.c -lncurses

fiume.o: fiume.c fiume.h
	gcc -c -g fiume.c -lncurses

strada.o: strada.c strada.h
	gcc -c strada.c -lncurses

collisioni.o: collisioni.c collisioni.h
	gcc -c -g collisioni.c -lncurses

gestione_grafica.o: gestione_grafica.c gestione_grafica.h
	gcc -c -g gestione_grafica.c -lncurses

clean: 
	rm *.o
	rm *.out
	echo "pulizia effettuata.."

