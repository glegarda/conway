#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ncurses.h>
#include "conway.h"
#include "userInterface.h"

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

int main (int argc, char *argv[])
{

	initscr();
	curs_set(0); //Makes the cursor invisible, otherwise it will keep blinking after the '*'
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
 	clear();

 	bool escape;

	vector state; //Check if defined previously

	if (argc == 3) {
		int arg1 = atoi(argv[1]);
		int arg2 = atoi(argv[2]);
		setWindowsSize(arg1,arg2,&width,&height);
  	} else { setWindowsSize((int)(3.0/4*COLS),(int)(3.0/4*LINES),&width,&height); }

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

  	while (escape == false) {

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
				printMenu(menu_win, highlight, n_choices, choices_menu);
				keyPressedMenu(menu_win, &highlight, n_choices, &choice);
				printMenu(menu_win, highlight, n_choices, choices_menu);

				if (choice != 0) {
		  			if (highlight==1) {
						current_state = play_menu;
						clear();
						box(game_win, 0, 0);
		  				wrefresh(game_win);
					} else if (highlight==2) {
		  				current_state = options_menu;
		  				clear();
		  				box(game_win, 0, 0);
		  				wrefresh(game_win);
		  				highlight = 1;
		  			} else{escape = true;}
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
				printGameMenu(gameMenu_win, highlight,n_choices, choices_game);
				keyPressedMenu(gameMenu_win, &highlight, n_choices, &choice);
				printGameMenu(gameMenu_win, highlight,n_choices, choices_game);

				if(choice != 0){

		  			int mid_x = (width+2)/2-1;
		  			int mid_y = (height+2)/2-1;

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
		  				check = initVector(&state,5); //We estimate that the min. number is going to be 5
		  				CHECK_ALLOC(0);
		  				int check_int = 0; //check_int = 1 go back to menu; check_int = -1 memory allocation failed, teminate.
		  				if (highlight==1) { //Game mode 1: R-pentomino
		  					int x1[5] = {mid_x  ,mid_x+1,mid_x-1,mid_x,mid_x  };
		  					int y1[5] = {mid_y-1,mid_y-1,mid_y  ,mid_y,mid_y+1};
		  					check_int = initMode(&state,&width,x1,y1,5);
		  				} else if (highlight==2) { //Game mode 2: Diehard
		  					int x2[7] = {mid_x+3,mid_x-3,mid_x-2,mid_x-2,mid_x+2,mid_x+3,mid_x+4};
		  					int y2[7] = {mid_y-1,mid_y  ,mid_y  ,mid_y+1,mid_y+1,mid_y+1,mid_y+1};
		  					check_int = initMode(&state,&width,x2,y2,7);
		  				} else if (highlight==3) { //Game mode 3: Acorn
		  					int x3[7] = {mid_x-2,mid_x  ,mid_x-3,mid_x-2,mid_x+1,mid_x+2,mid_x+3};
		  					int y3[7] = {mid_y-1,mid_y  ,mid_y+1,mid_y+1,mid_y+1,mid_y+1,mid_y+1};
		  					check_int = initMode(&state,&width,x3,y3,7);
		  				} else { //Game mode 4: User inputs simulation
							check_int = GetUserSim(game_win, &state, &width, &height, &symbol);
		  				}
		  				//Check if memory allocation was successful
		  				if (check_int==-1){
		  					return -1;
		  				} else if (check_int==0) { //Start game
	  						current_state = game_on;
		  				} else if (check_int==1){ //Go back to choice menu
							state.size = 0;
							current_state = play_menu;
		  				}

		  			}
				}

				choice = 0;

	  		break;

			case options_menu: ;

				n_choices = sizeof(choices_options) / sizeof(char *);
				WINDOW *options_menu_win;
				start_x_menu = (int)(COLS/2-20-1);
				start_y_menu = (int)(LINES/2-7-1);
				options_menu_win = newwin(15, 40,start_y_menu,start_x_menu);

				keypad(options_menu_win, TRUE);
				printOptionsMenu(options_menu_win, highlight, n_choices, &width, &height, &symbol, &refreshing_time, choices_options);
				keyPressedMenu(options_menu_win, &highlight, n_choices, &choice);
				printOptionsMenu(options_menu_win, highlight, n_choices, &width, &height, &symbol, &refreshing_time, choices_options);

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
						setWindowsSize(atoi(newValue),height,&width,&height);
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
						setWindowsSize(width,atoi(newValue),&width,&height);
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

			case game_on: ;
				PrintWndw(game_win, &width, &height, &state, &symbol);
				float scaled_speed = 1.0;
				bool game_paused = false;
				mvprintw (start_y+height+2,start_x,"r: reset");
				mvprintw (start_y+height+3,start_x,"space bar: pause");
				mvprintw (start_y+height+4,start_x,"esc: exit game");
				mvprintw (start_y+height+5,start_x,"-> arrow: increment speed");
				mvprintw (start_y+height+6,start_x,"<- arrow: decrement speed");
				refresh();
				box(game_win, 0, 0);
				wrefresh(game_win);
				wtimeout(game_win,(int)(refreshing_time/scaled_speed*1000));
				keypad(game_win, TRUE);
				int dummy_var = 1;
				int movement;
				do{
					mvprintw (start_y+height+2,start_x+width-17,"speed factor: x%.02f",scaled_speed);
					refresh();
					mvwprintw(game_win,1, dummy_var, "%c",symbol);
					movement = wgetch(game_win);
					bool refresh = true;
					switch (movement){
						case KEY_RIGHT:
							if(scaled_speed < 4){
								scaled_speed *= 2.0;
								wtimeout(game_win,(int)(refreshing_time/scaled_speed*1000));
							}
						break;

						case KEY_LEFT:
							if(scaled_speed > 0.25){
								scaled_speed /= 2;
								wtimeout(game_win,(int)(refreshing_time/scaled_speed*1000));
							}
						break;

						case 32:
							if(!game_paused){
								game_paused=true;
								wtimeout(game_win,-1);
							}else{
								game_paused=false;
								wtimeout(game_win,(int)(refreshing_time/scaled_speed*1000));
							}
						break;

						case 114:
							wclear(game_win);
							box(game_win, 0, 0);
							wrefresh(game_win);
							dummy_var = 0;
							PrintWndw(game_win, &width, &height, &state, &symbol);

						break;

						case 27:
							current_state = play_menu;
							clear();
							refresh();
							wrefresh(game_win);
						break;

						case ERR:
						break;

						default:
							refresh = false;
						break;
					}
					if (refresh){
						dummy_var ++;
						mvwprintw(game_win,1, dummy_var, "%c",symbol);
						wrefresh(game_win);
					}



				}while(movement!=27);
				wclear(game_win);
				box(game_win, 0, 0);
				wrefresh(game_win);
			break;

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
