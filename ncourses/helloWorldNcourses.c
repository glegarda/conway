// https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/

#include <ncurses.h>

int main(){
  initscr(); // Start courses mode
  printw("HELLO WORLD !!!"); //Prints using ncourses
  refresh(); //Prints whatever printw on the real screen. stdscr at the current
             //(y,x) co-ordinates. Since our present co-ordinates are at 0,0 the
             //string is printed at the left hand corner of the window.
  getch();   // waits for user input
  endwin();  // End courses mode

  return 0;
}
