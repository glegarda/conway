
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h> //Con esta librería ya no da el 'warning' en la compilacion
#include <stdbool.h>

int width;
int height;

char *choicesMenu[] = {
		"PLAY",
		"OPTIONS",
		"EXIT",
		};

enum states {mainMenu, Play, Options};
enum states currentState = mainMenu;

void print_menu(WINDOW *menu_win, int highlight, int n_choices);
void setWindowsSize(int arg1,int arg2);

int main(int argc, char *argv[])
{

	initscr();
  curs_set(0); //Makes the cursor invisible, otherwise it will keep blinking after the '*'
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	clear();

	bool escape;

  if(argc == 3){
    int arg1 = atoi(argv[1]);
    int arg2 = atoi(argv[2]);
    setWindowsSize(arg1,arg2);

  }else{
    setWindowsSize((int)(3.0/4*COLS),(int)(3.0/4*LINES));
  }

  WINDOW *game_win;

  int highlight = 1;
  int choice = 0;
  int c;

	int startx = (int)COLS/2-(int)width/2-1;
	int starty = (int)LINES/2-(int)height/2-1;
	game_win = newwin(height+2, width+2, starty, startx);


  refresh();
  box(game_win, 0, 0);
  wrefresh(game_win);

	while(escape == false){
		switch (currentState) {
			case mainMenu: ;

				int n_choices = sizeof(choicesMenu) / sizeof(char *);

				WINDOW *menu_win;
				int startxMenu = (int)COLS/2-(int)((float)width/4);
				int startyMenu = (int)LINES/2-(int)((float)height/2.5);
				menu_win = newwin((int)(0.75*height), (int)(0.5*width),startyMenu,startxMenu);
				keypad(menu_win, TRUE);
				box(menu_win, 0, 0);
				print_menu(menu_win, highlight,n_choices);
				c = wgetch(menu_win);
				switch(c)
				{	case KEY_UP:
						if(highlight == 1)
							highlight = n_choices;
						else
							--highlight;
						break;
					case KEY_DOWN:
						if(highlight == n_choices)
							highlight = 1;
						else
							++highlight;
						break;
					case 10:
						choice = highlight;
						break;
					default:
						refresh();
						break;
				}
				print_menu(menu_win, highlight,n_choices);
				if(choice != 0){
					if(highlight==3)
						escape = true;
					choice = 0;
				}	/* User did a choice come out of the infinite loop */
			break;
		}

	}
	endwin();
	return 0;


}

void setWindowsSize(int arg1,int arg2){
  if(arg1>arg2){
    if(arg1<COLS && arg2<LINES){
      width = arg1;
      height = arg2;
    }
  }else{
    if(arg2<COLS && arg1<LINES){
      width = arg2;
      height = arg1;
    }
  }
}

void print_menu(WINDOW *menu_win, int highlight, int n_choices)
{
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choicesMenu[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choicesMenu[i]);
		++y;
	}
	wrefresh(menu_win);
}
