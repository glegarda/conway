#include <ncurses.h>


#define WIDTH 80
#define HEIGHT 20

int main()
{
  WINDOW *menu_win;

	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */


  int startx = (WIDTH/2) -WIDTH/4;
	int starty = (HEIGHT/2) -HEIGHT/4;

	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
  box(menu_win, 0, 0);

  mvprintw(0, 0, "This is a message printed in coordinates x=0, y=0 of the src window");
  mvwprintw(menu_win, 1, 1, "This is a message printed in coordinates x=1, y=1 of the menu window");
  refresh();
  wrefresh(menu_win);

  getch();			/* Wait for user input */
	endwin();			    /* End curses mode		  */

	return 0;
}
