#include <stdio.h>
#include <ncurses.h>

#include "conway.h"

int main()
{
  initall();
  printw("Hello World !!!");	/* Print Hello World		  */
  refresh();			/* Print it on to the real screen */
  mvaddch(10,10,'X');
    refresh();			/* Print it on to the real screen */

  getch();			/* Wait for user input */
  enditall();
 
}
