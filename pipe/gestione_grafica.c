#include "gestione_grafica.h"

int abilita_movimento_confini_tane(oggetto_rana npc, int direzione)
{
    if((npc.y <9 && !((npc.x/9)%2)) && direzione ==KEY_UP)
        return 0;
    if((npc.y == 4 && ((npc.x/9)%2)) && direzione != KEY_DOWN)
        return 0;
    else
        return 1;
 
}

int calcola_background(int x , int y)
{
    if(y >= MAX_PRATO && y <= MAX_FIUME)
        return COLOR_BLUE;
    if(y > MAX_FIUME && y < MIN_STRADA)
        return COLOR_MAGENTA;
    if(y >= MIN_STRADA && y <= MAX_STRADA)
        return COLOR_BLACK;
    if(y>MAX_STRADA)
        return COLOR_MAGENTA;
}

void print_sprite(int x,int y,const char *sprite[], int size)
{
    for (int i = 0; i < size; i++)
    {
        mvprintw(y+i, x, sprite[i]);
    }
}

void mappa_gioco()
{
    //colori scenari
    init_pair(1,COLOR_GREEN,COLOR_GREEN); //prato verde
    init_pair(2,COLOR_BLUE,COLOR_BLUE); //mare
    init_pair(3,COLOR_MAGENTA,COLOR_MAGENTA); //prato viola
    init_pair(4,COLOR_BLACK,COLOR_BLACK); //strada

    for(unsigned i=0; i < H_FROGGER; i++) //generazione prato
	{
		attron(COLOR_PAIR(1));
		for (size_t i = 0; i < MAXX; i++)
		{
			printw(" ");
		}
		printw("\n");
		
	}
	
	for(unsigned i=0; i < H_FROGGER; i++) //generazione tane
	{
		for (size_t i = 0; i < N_TANE ; i++)
		{
			printw("         ");
			attron(COLOR_PAIR(2));
			printw("         ");
			attron(COLOR_PAIR(1));
		}
		
		printw("         \n");
	}

	for(unsigned i=0; i < H_FROGGER * N_CORSIE_FIUME; i++) //generazione fiume
	{
		attron(COLOR_PAIR(2));
		for (size_t i = 0; i < MAXX; i++)
		{
			printw(" ");
		}
		printw("\n");
	}

	for(unsigned i=0; i < H_FROGGER; i++) //generazione marciapiede
	{
		attron(COLOR_PAIR(3));
		for (size_t i = 0; i < MAXX; i++)
		{
			printw(" ");
		}
		printw("\n");
	}
	
	for(unsigned i=0; i < H_FROGGER * N_CORSIE_STRADA; i++) //generazione strada
	{
		attron(COLOR_PAIR(4));
		for (size_t i = 0; i < MAXX; i++)
		{
			printw(" ");
		}
		printw("\n");
	}
	
	for(unsigned i=0; i < H_FROGGER; i++) //generazione marciapiede
	{
		attron(COLOR_PAIR(3));
		for (size_t i = 0; i < MAXX; i++)
		{
			printw(" ");
		}
		printw("\n");
	}

}

	