#include <stdio.h>
#include <ncurses.h>



int initall(void)
{
	initscr();			/* Start curses mode 		  */
	return 0;
};

int enditall(void)
{
	endwin();			/* Start curses mode 		  */
	return 0;
};
