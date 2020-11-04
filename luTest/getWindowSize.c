
// C# program to get the current
// window width and Height

#include <ncurses.h>

int main(void)
{
   int mx=0, my=0;

   initscr();
   getmaxyx(stdscr, mx, my);
   endwin();
   printf("\nWidth = %d \nHeight = %d \n", my, mx);

   return 0;
}
