// Amoh chavaleh que estoh es toh facil

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h> //Con esta librería ya no da el 'warning' en la compilacion

#define WIDTH 62
#define HEIGHT 22


void print_menu(WINDOW *menu_win, int highlight);
int c;

int main()
{

  WINDOW *menu_win;
	initscr();
  curs_set(0); //Makes the cursor invisible, otherwise it will keep blinking after the '*'

	int startx = (WIDTH/2) -WIDTH/4;
	int starty = (HEIGHT/2) -HEIGHT/4;


	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
  box(menu_win, 0, 0);
  do{
    clear();
    refresh();
    box(menu_win, 0, 0);
    wrefresh(menu_win);
    mvprintw(0, 0, "Write X coordinates (max x is %d): ",WIDTH-2);
    refresh();
    char xCoord[5];
    getstr(xCoord);
    mvprintw(1, 0, "Write Y coordinates (max y is %d): ",HEIGHT-2);
    refresh();
    char yCoord[5];
    getstr(yCoord);
    mvwprintw(menu_win,atoi(yCoord)+1, atoi(xCoord)+1, "*");
    wrefresh(menu_win);
    c = getch();
  }while(c!=27);



	endwin();
	return 0;
}
