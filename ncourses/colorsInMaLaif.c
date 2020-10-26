#include <ncurses.h>
/*
COLOR_BLACK   0
COLOR_RED     1
COLOR_GREEN   2
COLOR_YELLOW  3
COLOR_BLUE    4
COLOR_MAGENTA 5
COLOR_CYAN    6
COLOR_WHITE   7
*/

     //init_color(COLOR_RED, 700, 0, 0); //Modify colors defined by ncurses
    /* param 1     : color name
     * param 2, 3, 4 : rgb content min = 0, max = 1000 */

#define offsetAscii 48

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string);

int main(int argc, char *argv[])
{
  initscr();			/* Start curses mode 		*/
  noecho();
  cbreak();	/* Line buffering disabled. pass on everything */
	start_color();			/* Start color 			*/
	init_pair(1, COLOR_BLACK, COLOR_RED);
  attron(COLOR_PAIR(1));
  mvprintw(0, 0, "Press 0 for a black background ");
  mvprintw(1, 0, "Press 1 for a red background ");
  mvprintw(2, 0, "Press 2 for a green background ");
  mvprintw(3, 0, "Press 3 for a yellow background ");
  attroff(COLOR_PAIR(1));
  wbkgd(COLOR_PAIR(1))
  refresh();
  int c = getch();
  while(c==0+offsetAscii||c==1+offsetAscii||c==2+offsetAscii||c==3+offsetAscii){
    if (c==0+offsetAscii){
      init_pair(1, COLOR_RED, COLOR_BLACK);
    }else if(c==1+offsetAscii){
      init_pair(1, COLOR_BLACK, COLOR_RED);
    }else if(c==2+offsetAscii){
      init_pair(1, COLOR_YELLOW, COLOR_GREEN);
    }else if(c==3+offsetAscii){
      init_pair(1, COLOR_GREEN, COLOR_YELLOW);
    }
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "Press 0 for a black background ");
    mvprintw(1, 0, "Press 1 for a red background ");
    mvprintw(2, 0, "Press 2 for a green background ");
    mvprintw(3, 0, "Press 3 for a yellow background ");
    attroff(COLOR_PAIR(1));
    refresh();
    c=getch();
  }

	endwin();
}








void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	mvwprintw(win, y, x, "%s", string);
	refresh();
}
