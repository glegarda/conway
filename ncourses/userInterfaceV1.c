
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

char *choicesGame[] = {
	"Test 1: R-pentomino",
	"Test 2: Diehard",
	"Test 3: Acorn",
	"User input using keyboard",
	"User input using mouse(?)",
	"Back"
};

enum states {mainMenu, playMenu, OptionsMenu, gameOn};
enum states currentState = mainMenu;

void print_menu(WINDOW *win, int highlight, int n_choices);
void print_gameMenu(WINDOW *win, int highlight, int n_choices);
void key_pressed(WINDOW *win, int *highlight, int n_choices, int *choice);
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

		int n_choices;
		int startxMenu, startyMenu;

		switch (currentState) {
			case mainMenu: ;

				n_choices = sizeof(choicesMenu) / sizeof(char *);
				WINDOW *menu_win;
				startxMenu = (int)COLS/2-(int)((float)width/4);
				startyMenu = (int)LINES/2-(int)((float)height/2.5);
				menu_win = newwin((int)(0.75*height), (int)(0.5*width),startyMenu,startxMenu);

				keypad(menu_win, TRUE);
				print_menu(menu_win, highlight,n_choices);
				key_pressed(menu_win, &highlight, n_choices, &choice);
				print_menu(menu_win, highlight,n_choices);

				if(choice != 0){
					if(highlight==1){
						currentState = playMenu;
						clear();
						box(game_win, 0, 0);
					  wrefresh(game_win);
					}
					else if(highlight==2);
						//Options menu
					else
						escape = true;
					choice = 0;
				}	/* User did a choice come out of the infinite loop */
			break;

			case playMenu: ;

				n_choices = sizeof(choicesGame) / sizeof(char *);

				WINDOW *gameMenu_win;
				startxMenu = (int)COLS/2-(int)((float)width/2.5);
				startyMenu = (int)LINES/2-(int)((float)height/2.5);
				gameMenu_win = newwin((int)(0.75*height), (int)(0.75*width),startyMenu,startxMenu);

				keypad(gameMenu_win, TRUE);
				print_gameMenu(gameMenu_win, highlight,n_choices);
				key_pressed(gameMenu_win, &highlight, n_choices, &choice);
				print_gameMenu(gameMenu_win, highlight,n_choices);

				if(choice != 0){
					if(highlight==1);
						//initialize gamemode1
					else if(highlight==2);
						//initialize gamemode2
					else if(highlight==3);
						//initialize gamemode3
					else if(highlight==6){
						currentState = mainMenu;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						highlight = 1;
					}
					choice = 0;
				}
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

void print_menu(WINDOW *win, int highlight, int n_choices)
{
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(win, A_REVERSE);
			mvwprintw(win, y, x, "%s", choicesMenu[i]);
			wattroff(win, A_REVERSE);
		}
		else
			mvwprintw(win, y, x, "%s", choicesMenu[i]);
		++y;
	}
	wrefresh(win);
}

void print_gameMenu(WINDOW *win, int highlight, int n_choices)
{
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(win, A_REVERSE);
			mvwprintw(win, y, x, "%s", choicesGame[i]);
			wattroff(win, A_REVERSE);
		}
		else
			mvwprintw(win, y, x, "%s", choicesGame[i]);
		++y;
	}
	wrefresh(win);
}

void key_pressed(WINDOW *win, int *highlight, int n_choices, int *choice)
{
	int c = wgetch(win);
	switch(c)
	{	case KEY_UP:
			if(*highlight == 1)
				*highlight = n_choices;
			else
				--*highlight;
			break;
		case KEY_DOWN:
			if(*highlight == n_choices)
				*highlight = 1;
			else
				++*highlight;
			break;
		case 10:
			*choice = *highlight;
			break;
		default:
			refresh();
			break;
	}
}
