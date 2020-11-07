// 30x10 main_menu
// 40x15 gameMenu

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h> //Con esta librería ya no da el 'warning' en la compilacion
#include <stdbool.h>
//******************************************************************************
#include <stdarg.h>
#include "conway.h"
// Check for correct memory allocation and clear memory in case of error.
#define CHECK_ALLOC(...) \
    if (!check) { freeVector(__VA_ARGS__); return -1; } \
    else (void)0
//*****************************************************************************

int width;
int height;
char symbol = '*';
float refreshing_time = 1.0;

char *choices_menu[] = {
  "PLAY",
  "OPTIONS",
  "EXIT",
};

char *choices_game[] = {
	"Test 1: R-pentomino",
	"Test 2: Diehard",
	"Test 3: Acorn",
	"User input using keyboard",
	"Back",
};

char *choices_options[] = {
	"Width: ",
	"Heigth: ",
	"Cell Alive Symbol: ",
	"Refreshing time: ",
	"Back",
};

enum states {main_menu, play_menu, options_menu, game_on};
enum states current_state = main_menu;

void printMenu(WINDOW *win, int highlight, int n_choices);
void printGameMenu(WINDOW *win, int highlight, int n_choices);
void printOptionsMenu(WINDOW *win, int highlight, int n_choices, int *width, int *height, char *symbol, float *refreshing_time);
void keyPressed(WINDOW *win, int *highlight, int n_choices, int *choice);
void setWindowsSize(int width_size,int height_size);
void optionsGetch(char *buffer, WINDOW *win);
//*****************************************************************************
int GetUserSim(WINDOW *win, vector *v, int *width, int *height, char *symbol);
int initMode(vector *v, int *width, int *x, int *y, char size);
void PrintWndw(WINDOW *win, int *width, int *height, vector *v, char *symbol);
//*****************************************************************************

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
//*****************************************************************************
	int mid_x = (width+2)/2-1;
	int mid_y = (height+2)/2-1;
//*****************************************************************************
  WINDOW *game_win;

  int highlight = 1;
  int choice = 0;
	int c;

	int start_x = (int)COLS/2-(int)width/2-1;
	int start_y = (int)LINES/2-(int)height/2-1;
	game_win = newwin(height+2, width+2, start_y, start_x);


  refresh();
  box(game_win, 0, 0);
  wrefresh(game_win);

	while(escape == false){

		int n_choices;
		int start_x_menu, start_y_menu;

		switch (current_state) {
			case main_menu: ;

				n_choices = sizeof(choices_menu) / sizeof(char *);
				WINDOW *menu_win;
				start_x_menu = (int)(COLS/2-15-1);
				start_y_menu = (int)(LINES/2-5-1);
				menu_win = newwin(10, 30,start_y_menu,start_x_menu);

				keypad(menu_win, TRUE);
				printMenu(menu_win, highlight,n_choices);
				keyPressed(menu_win, &highlight, n_choices, &choice);
				printMenu(menu_win, highlight,n_choices);

				if(choice != 0){
					if(highlight==1){
						current_state = play_menu;
						clear();
						box(game_win, 0, 0);
					  wrefresh(game_win);
					}
					else if(highlight==2){
						current_state = options_menu;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						highlight = 1;
					}
					else{escape = true;}
					choice = 0;
				}	/* User did a choice come out of the infinite loop */
			break;

			case play_menu: ;

				n_choices = sizeof(choices_game) / sizeof(char *);
				WINDOW *gameMenu_win;
				start_x_menu = (int)(COLS/2-20-1);
				start_y_menu = (int)(LINES/2-7-1);
				gameMenu_win = newwin(15, 40,start_y_menu,start_x_menu);

				keypad(gameMenu_win, TRUE);
				printGameMenu(gameMenu_win, highlight,n_choices);
				keyPressed(gameMenu_win, &highlight, n_choices, &choice);
				printGameMenu(gameMenu_win, highlight,n_choices);

				if(choice != 0){
					//*****************************************************************************
					if(highlight==5){
						current_state = main_menu;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						highlight = 1;
					} else {
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						bool check = false; //Check if memory allocation was successful -> Check if defined previously
						vector state; //Check if defined previously
						check = initVector(&state,5); //We estimate that the min. number is going to be 5
						CHECK_ALLOC(0);
						int check_int = 0; //check_int = 1 go back to menu; check_int = -1 memory allocation failed, teminate.
						if(highlight==1){ //Game mode 1: R-pentomino
							int x1[5] = {mid_x  ,mid_x+1,mid_x-1,mid_x,mid_x  };
							int y1[5] = {mid_y-1,mid_y-1,mid_y  ,mid_y,mid_y+1};
							check_int = initMode(&state,&width,x1,y1,5);
						}	else if(highlight==2){ //Game mode 2: Diehard
							int x2[7] = {mid_x+3,mid_x-3,mid_x-2,mid_x-2,mid_x+2,mid_x+3,mid_x+4};
							int y2[7] = {mid_y-1,mid_y  ,mid_y  ,mid_y+1,mid_y+1,mid_y+1,mid_y+1};
							check_int = initMode(&state,&width,x2,y2,7);
						} else if(highlight==3){ //Game mode 3: Acorn
							int x3[7] = {mid_x-2,mid_x  ,mid_x-3,mid_x-2,mid_x+1,mid_x+2,mid_x+3};
							int y3[7] = {mid_y-1,mid_y  ,mid_y+1,mid_y+1,mid_y+1,mid_y+1,mid_y+1};
							check_int = initMode(&state,&width,x3,y3,7);
						} else if(highlight==4) { //Game mode 4: User inputs simulation
	 						check_int = GetUserSim(game_win, &state, &width, &height, &symbol);
						}
						//Check if memory allocation was successful
						if (check_int==-1){
							return -1;
						} else if (check_int==0) { //Start game
	 							current_state = game_on;
	 					} else if (check_int==1){ //Go back to choice menu
								//freeVector(1,&state); //Pensaba que con esto limpiaba el vector pero parece que no
								state.size = 0;
								current_state = play_menu;
	 					}
						PrintWndw(game_win, &width, &height, &state, &symbol);
						}
					}
					//*****************************************************************************
					choice = 0;
			break;

			case options_menu: ;

				n_choices = sizeof(choices_options) / sizeof(char *);
				WINDOW *options_menu_win;
				start_x_menu = (int)(COLS/2-20-1);
				start_y_menu = (int)(LINES/2-7-1);
				options_menu_win = newwin(15, 40,start_y_menu,start_x_menu);

				keypad(options_menu_win, TRUE);
				printOptionsMenu(options_menu_win, highlight, n_choices, &width, &height, &symbol, &refreshing_time);
				keyPressed(options_menu_win, &highlight, n_choices, &choice);
				printOptionsMenu(options_menu_win, highlight, n_choices, &width, &height, &symbol, &refreshing_time);

				if(choice != 0){
					if(highlight<5)	{
						mvwprintw(options_menu_win, 10, 2, "Introduce new %s", choices_options[highlight-1]);
						wrefresh(options_menu_win);
					}
					char newValue[5];
					if(highlight==1){
						optionsGetch(newValue,options_menu_win);
						erase();
						refresh();
						setWindowsSize(atoi(newValue),height);
						start_x = (int)COLS/2-(int)width/2-1;
						start_y = (int)LINES/2-(int)height/2-1;
						game_win = newwin(height+2, width+2, start_y, start_x);
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					else if(highlight==2){
						optionsGetch(newValue,options_menu_win);
						erase();
						refresh();
						setWindowsSize(width,atoi(newValue));
						erase();
						start_x = (int)COLS/2-(int)width/2-1;
						start_y = (int)LINES/2-(int)height/2-1;
						game_win = newwin(height+2, width+2, start_y, start_x);
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					else if(highlight==3){
						symbol = wgetch(options_menu_win);
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					else if(highlight==4){
						optionsGetch(newValue,options_menu_win);
						refreshing_time=atof(newValue);
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					else{
						current_state = main_menu;
						highlight=1;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					choice = 0;
				}	/* User did a choice come out of the infinite loop */

			break;
//*****************************************************************************
			case game_on: ;
				mvwprintw(game_win, 1, 1, "Lets begin...");
				mvwprintw(game_win, 2, 1, "Press ESC to finalize");
				wrefresh(game_win);

				keypad(game_win, TRUE);
				int movement;
				do{
					movement = wgetch(game_win);
					switch (movement){
						case 27:

							break;

						default:
							break;
					}
				}while(movement!=27);
				keypad(game_win, FALSE);
				wclear(game_win);
				box(game_win, 0, 0);
				wrefresh(game_win);
				escape = true;
				choice = 0;
			break;
//*****************************************************************************
		}

	}
  // Mac Problem
	curs_set(1);
	nocbreak();
	echo();
	erase();
	refresh();
  // Mac Problem
	endwin();
	return 0;
}

void setWindowsSize(int width_size,int height_size){

  if(width_size<COLS && height_size<LINES && width_size>42 && height_size>17){
      width = width_size;
      height = height_size;
  }
}

void printMenu(WINDOW *win, int highlight, int n_choices)
{
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(win, A_REVERSE);
			mvwprintw(win, y, x, "%s", choices_menu[i]);
			wattroff(win, A_REVERSE);
		}
		else
			mvwprintw(win, y, x, "%s", choices_menu[i]);
		++y;
	}
	wrefresh(win);
}

void printGameMenu(WINDOW *win, int highlight, int n_choices)
{
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(win, A_REVERSE);
			mvwprintw(win, y, x, "%s", choices_game[i]);
			wattroff(win, A_REVERSE);
		}
		else
			mvwprintw(win, y, x, "%s", choices_game[i]);
		++y;
	}
	wrefresh(win);
}

void printOptionsMenu(WINDOW *win, int highlight, int n_choices, int *width, int *height, char *symbol, float *refreshing_time)
{
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for(i = 0; i < n_choices-1; ++i)
	{
		mvwprintw(win, y, x, "%s", choices_options[i]);
		if(highlight == i + 1) /* High light the present choice */
		  wattron(win, A_REVERSE);

		if(i==0)
			wprintw(win,"%d",*width);
		else if(i==1)
			wprintw(win,"%d",*height);
		else if(i==2)
			wprintw(win,"%c",*symbol);
		else if(i==3)
			wprintw(win,"%.02f",*refreshing_time);

		if(highlight == i + 1)
			wattroff(win, A_REVERSE);
		++y;
	}
	if(highlight == n_choices){
		wattron(win, A_REVERSE);
		mvwprintw(win, y, x, "%s", choices_options[n_choices-1]);
		wattroff(win, A_REVERSE);
	}else{
		mvwprintw(win, y, x, "%s", choices_options[n_choices-1]);
	}
	wrefresh(win);
}

void keyPressed(WINDOW *win, int *highlight, int n_choices, int *choice)
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

void optionsGetch(char *buffer, WINDOW *win){
	curs_set(1);
	nocbreak();
	echo();
	curs_set(1);
	nocbreak();
	echo();
	wgetstr(win,buffer);
	curs_set(0);
	noecho();
	cbreak();
	keypad(win, TRUE);
}

//*****************************************************************************
int GetUserSim(WINDOW *win, vector *v, int *width, int *height, char *symbol){
	curs_set(1);
	keypad(win, TRUE);

	int x_loc = 1;
	int y_loc = 1;
	int x_max = *width;
	int y_max = *height;

	int movement;
	int check = 0;
	do{
		wmove(win, y_loc, x_loc);
		wrefresh(win);

		movement = wgetch(win);

		switch (movement){
			case KEY_UP:
			case 'w':
			case 'W':
				if (y_loc>1){y_loc--;}
				break;
			case KEY_DOWN:
			case 's':
			case 'S':
				if (y_loc<y_max){y_loc++;}
				break;
			case KEY_LEFT:
			case 'a':
			case 'A':
				if (x_loc>1){x_loc--;}
				break;
			case KEY_RIGHT:
			case 'd':
			case 'D':
				if (x_loc<x_max){x_loc++;}
				break;
			case 32:
				mvwprintw(win,y_loc,x_loc, "%c",*symbol);
				check = initMode(v,width,&x_loc,&y_loc,1);
				if (check){return -1;} //Memory allocation failed, terminate
				break;
			default:
				break;
		}
		wmove(win, y_loc, x_loc);
		wrefresh(win);
	}while(movement!=10 && movement!=27);
	curs_set(0);
	keypad(win, FALSE);
	if (movement==27){ //ESC return back to menu
		wclear(win);
		box(win, 0, 0);
		wrefresh(win);
		return 1;
	} else { //ENTER starts the game
		return 0;
	}
}

int initMode(vector *v, int *width, int *x, int *y, char size){
	//Le paso un vector de coordenadas y me crea el vector state
	bool check = false;
	cell c;
	c.live_neighbours = 0;
	for (int i=0; i<size; i++){
		c.id = xy2id(x[i],y[i],*width);
		check = pushBack(v, &c);
		CHECK_ALLOC(1, &v);
	}
	return 0;
}

void PrintWndw(WINDOW *win, int *width, int *height, vector *v, char *symbol){
	unsigned short coordinates[2] = {0};
	for (int i=0; i<(v->size); i++){
		id2xy(coordinates, v->array[i].id, *width);
		mvwprintw(win,coordinates[1], coordinates[0], "%c",*symbol);
	}
	wrefresh(win);
}
