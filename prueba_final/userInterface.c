// 30x10 mainMenu
// 40x15 gameMenu

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include "conway.h"

int width;
int height;
char symbol = '*';
float refreshingTime = 1.0;

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
	"Back",
};

char *choicesOptions[] = {
	"Width: ",
	"Heigth: ",
	"Cell Alive Symbol: ",
	"Refreshing time: ",
	"Back",
};

enum states {mainMenu, playMenu, optionsMenu, gameOn};
enum states currentState = mainMenu;

void print_menu(WINDOW *win, int highlight, int n_choices);
void print_gameMenu(WINDOW *win, int highlight, int n_choices);
void print_optionsMenu(WINDOW *win, int highlight, int n_choices, int *width, int *height, char *symbol, float *refresingTime);
void key_pressed(WINDOW *win, int *highlight, int n_choices, int *choice);
void setWindowsSize(int widthSize,int heightSize);
void optionsGetch(char *buffer, WINDOW *win);
//*****************************************************************************
int GetUserSim(WINDOW *win, vector *v, int *width, int *height, char *symbol);
int initMode(vector *v, int *width, int *x, int *y, char size);
void Print_Wndw(WINDOW *win, int *width, int *height, vector *v, char *symbol);
//*****************************************************************************

int main(int argc, char *argv[])
{

	initscr();
  curs_set(0); //Makes the cursor invisible, otherwise it will keep blinking after the '*'
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	clear();

	bool escape;
  vector state; //Check if defined previously

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
				startxMenu = (int)(COLS/2-15-1);
				startyMenu = (int)(LINES/2-5-1);
				menu_win = newwin(10, 30,startyMenu,startxMenu);

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
					else if(highlight==2){
						currentState = optionsMenu;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						highlight = 1;
					}
					else{escape = true;}
					choice = 0;
				}	/* User did a choice come out of the infinite loop */
			break;

			case playMenu: ;

				n_choices = sizeof(choicesGame) / sizeof(char *);
				WINDOW *gameMenu_win;
				startxMenu = (int)(COLS/2-20-1);
				startyMenu = (int)(LINES/2-7-1);
				gameMenu_win = newwin(15, 40,startyMenu,startxMenu);

				keypad(gameMenu_win, TRUE);
				print_gameMenu(gameMenu_win, highlight,n_choices);
				key_pressed(gameMenu_win, &highlight, n_choices, &choice);
				print_gameMenu(gameMenu_win, highlight,n_choices);

				if(choice != 0){
					//*****************************************************************************
					if(highlight==5){
						currentState = mainMenu;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						highlight = 1;
					} else {
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						bool check = false; //Check if memory allocation was successful -> Check if defined previously
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
	 							currentState = gameOn;
	 					} else if (check_int==1){ //Go back to choice menu
								//freeVector(1,&state); //Pensaba que con esto limpiaba el vector pero parece que no
								state.size = 0;
								currentState = playMenu;
	 					}
						Print_Wndw(game_win, &width, &height, &state, &symbol);
						}
					}
					//*****************************************************************************
					choice = 0;
			break;

			case optionsMenu: ;

				n_choices = sizeof(choicesOptions) / sizeof(char *);
				WINDOW *optionsMenu_win;
				startxMenu = (int)(COLS/2-20-1);
				startyMenu = (int)(LINES/2-7-1);
				optionsMenu_win = newwin(15, 40,startyMenu,startxMenu);

				keypad(optionsMenu_win, TRUE);
				print_optionsMenu(optionsMenu_win, highlight, n_choices, &width, &height, &symbol, &refreshingTime);
				key_pressed(optionsMenu_win, &highlight, n_choices, &choice);
				print_optionsMenu(optionsMenu_win, highlight, n_choices, &width, &height, &symbol, &refreshingTime);

				if(choice != 0){
					if(highlight<5)	{
						mvwprintw(optionsMenu_win, 10, 2, "Introduce new %s", choicesOptions[highlight-1]);
						wrefresh(optionsMenu_win);
					}
					char newValue[5];
					if(highlight==1){
						optionsGetch(newValue,optionsMenu_win);
						erase();
						refresh();
						setWindowsSize(atoi(newValue),height);
						startx = (int)COLS/2-(int)width/2-1;
						starty = (int)LINES/2-(int)height/2-1;
						game_win = newwin(height+2, width+2, starty, startx);
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					else if(highlight==2){
						optionsGetch(newValue,optionsMenu_win);
						erase();
						refresh();
						setWindowsSize(width,atoi(newValue));
						erase();
						startx = (int)COLS/2-(int)width/2-1;
						starty = (int)LINES/2-(int)height/2-1;
						game_win = newwin(height+2, width+2, starty, startx);
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					else if(highlight==3){
						symbol = wgetch(optionsMenu_win);
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					else if(highlight==4){
						optionsGetch(newValue,optionsMenu_win);
						refreshingTime=atof(newValue);
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					else{
						currentState = mainMenu;
						highlight=1;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					choice = 0;
				}	/* User did a choice come out of the infinite loop */

			break;
//*****************************************************************************
			case gameOn: ;
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
  freeVector(1,&state);
	curs_set(1);
	nocbreak();
	echo();
	erase();
	refresh();
	endwin();
	return 0;
}

void setWindowsSize(int widthSize,int heightSize){

  if(widthSize<COLS && heightSize<LINES && widthSize>42 && heightSize>17){
      width = widthSize;
      height = heightSize;
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

void print_optionsMenu(WINDOW *win, int highlight, int n_choices, int *width, int *height, char *symbol, float *refresingTime)
{
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for(i = 0; i < n_choices-1; ++i)
	{
		mvwprintw(win, y, x, "%s", choicesOptions[i]);
		if(highlight == i + 1) /* High light the present choice */
		  wattron(win, A_REVERSE);

		if(i==0)
			wprintw(win,"%d",*width);
		else if(i==1)
			wprintw(win,"%d",*height);
		else if(i==2)
			wprintw(win,"%c",*symbol);
		else if(i==3)
			wprintw(win,"%.02f",*refresingTime);

		if(highlight == i + 1)
			wattroff(win, A_REVERSE);
		++y;
	}
	if(highlight == n_choices){
		wattron(win, A_REVERSE);
		mvwprintw(win, y, x, "%s", choicesOptions[n_choices-1]);
		wattroff(win, A_REVERSE);
	}else{
		mvwprintw(win, y, x, "%s", choicesOptions[n_choices-1]);
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

	int x_Loc = 1;
	int y_Loc = 1;
	int x_Max = *width;
	int y_Max = *height;

	int movement;
	int check = 0;
	do{
		wmove(win, y_Loc, x_Loc);
		wrefresh(win);

		movement = wgetch(win);

		switch (movement){
			case KEY_UP:
			case 'w':
			case 'W':
				if (y_Loc>1){y_Loc--;}
				break;
			case KEY_DOWN:
			case 's':
			case 'S':
				if (y_Loc<y_Max){y_Loc++;}
				break;
			case KEY_LEFT:
			case 'a':
			case 'A':
				if (x_Loc>1){x_Loc--;}
				break;
			case KEY_RIGHT:
			case 'd':
			case 'D':
				if (x_Loc<x_Max){x_Loc++;}
				break;
			case 32:
				mvwprintw(win,y_Loc,x_Loc, "%c",*symbol);
				check = initMode(v,width,&x_Loc,&y_Loc,1);
				if (check){return -1;} //Memory allocation failed, terminate
				break;
			default:
				break;
		}
		wmove(win, y_Loc, x_Loc);
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

void Print_Wndw(WINDOW *win, int *width, int *height, vector *v, char *symbol){
	unsigned short coordinates[2] = {0};
	for (int i=0; i<(v->size); i++){
		id2xy(coordinates, v->array[i].id, *width);
		mvwprintw(win,coordinates[1], coordinates[0], "%c",*symbol);
	}
	wrefresh(win);
}
