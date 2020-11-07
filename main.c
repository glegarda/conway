#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ncurses.h>
#include "conway.h"
#include "userInterface.h"

int Width;
int Height;
char Symbol = '*';
float Refreshing_time = 1.0;
bool Check = false;
int Check_int;

char *Choices_menu[] = {
  "PLAY",
  "OPTIONS",
  "EXIT",
};

char *Choices_game[] = {
  "Test 1: R-pentomino",
  "Test 2: Diehard",
  "Test 3: Acorn",
  "User input using keyboard",
  "Back",
};

char *Choices_options[] = {
  "Width: ",
  "Heigth: ",
  "Cell Alive Symbol: ",
  "Refreshing time: ",
  "Back",
};

enum states {main_menu, play_menu, options_menu, game_on};
enum states current_state = main_menu;

int main (int argc, char *argv[]) {
	initscr();
	curs_set(0); //Makes the cursor invisible, otherwise it will keep blinking after the '*'
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
 	clear();

 	bool escape;

	vector original_conway_state;

	if (argc == 3) {
		int arg1 = atoi(argv[1]);
		int arg2 = atoi(argv[2]);
		setWindowsSize(arg1,arg2,&Width,&Height);
  	} else {
		setWindowsSize((int)(3.0/4*COLS),(int)(3.0/4*LINES), &Width, &Height);
	}

  	WINDOW *game_win;

  	int highlight = 1;
  	int choice = 0;
  	int c;

  	int start_x = (int)COLS/2-(int)Width/2-1;
  	int start_y = (int)LINES/2-(int)Height/2-1;
  	game_win = newwin(Height+2, Width+2, start_y, start_x);

  	refresh();
  	box(game_win, 0, 0);
  	wrefresh(game_win);

  	while (escape == false) {
		int n_choices;
		int start_x_menu, start_y_menu;

		switch (current_state) {
			case main_menu: ;
				n_choices = sizeof(Choices_menu) / sizeof(char *);
				WINDOW *menu_win;
				start_x_menu = (int)(COLS/2-15-1);
				start_y_menu = (int)(LINES/2-5-1);
				menu_win = newwin(10, 30,start_y_menu,start_x_menu);

				keypad(menu_win, TRUE);
				printMenu(menu_win, highlight, n_choices, Choices_menu);
				keyPressedMenu(menu_win, &highlight, n_choices, &choice);
				printMenu(menu_win, highlight, n_choices, Choices_menu);

				if (choice != 0) {
		  			if (highlight == 1) {
						current_state = play_menu;
						clear();
						box(game_win, 0, 0);
		  				wrefresh(game_win);
					} else if (highlight == 2) {
		  				current_state = options_menu;
		  				clear();
		  				box(game_win, 0, 0);
		  				wrefresh(game_win);
		  				highlight = 1;
		  			} else {
						escape = true;
					}
		  		choice = 0;
				}	/* User did a choice come out of the infinite loop */
			break;

	  		case play_menu: ;
				n_choices = sizeof(Choices_game) / sizeof(char *);
				WINDOW *gameMenu_win;
				start_x_menu = (int)(COLS/2-20-1);
				start_y_menu = (int)(LINES/2-7-1);
				gameMenu_win = newwin(15, 40,start_y_menu,start_x_menu);

				keypad(gameMenu_win, TRUE);
				printGameMenu(gameMenu_win, highlight,n_choices, Choices_game);
				keyPressedMenu(gameMenu_win, &highlight, n_choices, &choice);
				printGameMenu(gameMenu_win, highlight,n_choices, Choices_game);

				if (choice != 0) {
		  			int mid_x = (Width+2)/2-1;
		  			int mid_y = (Height+2)/2-1;

		  			if (highlight==5) {
		  				current_state = main_menu;
		  				clear();
		  				box(game_win, 0, 0);
		  				wrefresh(game_win);
		  				highlight = 1;
		  			} else {
		  				clear();
		  				box(game_win, 0, 0);
		  				wrefresh(game_win);
		  				Check = initVector(&original_conway_state, 10); // Set initial arbitrary size
						if (!Check) {
							return -1;
						}

		  				Check_int = 0; //Check_int = 1 go back to menu; Check_int = -1 memory allocation failed, teminate.
		  				if (highlight == 1) { //Game mode 1: R-pentomino
		  					int x1[5] = {mid_x  , mid_x+1, mid_x-1, mid_x, mid_x  };
		  					int y1[5] = {mid_y-1, mid_y-1, mid_y  , mid_y, mid_y+1};
		  					Check_int = initMode(&original_conway_state, &Width, x1, y1, 5);
		  				} else if (highlight == 2) { //Game mode 2: Diehard
		  					int x2[7] = {mid_x+3, mid_x-3, mid_x-2, mid_x-2, mid_x+2, mid_x+3, mid_x+4};
		  					int y2[7] = {mid_y-1, mid_y  , mid_y  , mid_y+1, mid_y+1, mid_y+1, mid_y+1};
		  					Check_int = initMode(&original_conway_state, &Width, x2, y2, 7);
		  				} else if (highlight == 3) { //Game mode 3: Acorn
		  					int x3[7] = {mid_x-2,mid_x  , mid_x-3, mid_x-2, mid_x+1, mid_x+2, mid_x+3};
		  					int y3[7] = {mid_y-1,mid_y  , mid_y+1, mid_y+1, mid_y+1, mid_y+1, mid_y+1};
		  					Check_int = initMode(&original_conway_state, &Width, x3, y3, 7);
		  				} else { //Game mode 4: User inputs simulation
							Check_int = GetUserSim(game_win, &original_conway_state, &Width, &Height, &Symbol);
		  				}

		  				//Check if memory allocation was successful
		  				if (Check_int == -1) {
		  					return -1;
		  				} else if (Check_int == 0) { //Start game
	  						current_state = game_on;
		  				} else if (Check_int == 1) { //Go back to choice menu
							original_conway_state.size = 0;
							current_state = play_menu;
		  				}
		  			}
				}
				choice = 0;
	  		break;

			case options_menu: ;
				n_choices = sizeof(Choices_options) / sizeof(char *);
				WINDOW *options_menu_win;
				start_x_menu = (int)(COLS/2-20-1);
				start_y_menu = (int)(LINES/2-7-1);
				options_menu_win = newwin(15, 40, start_y_menu, start_x_menu);

				keypad(options_menu_win, TRUE);
				printOptionsMenu(options_menu_win, highlight, n_choices, &Width, &Height, &Symbol, &Refreshing_time, Choices_options);
				keyPressedMenu(options_menu_win, &highlight, n_choices, &choice);
				printOptionsMenu(options_menu_win, highlight, n_choices, &Width, &Height, &Symbol, &Refreshing_time, Choices_options);

				if (choice != 0) {
					if (highlight < 5) {
						mvwprintw(options_menu_win, 10, 2, "Introduce new %s", Choices_options[highlight-1]);
						wrefresh(options_menu_win);
					}
					char newValue[5];
					if (highlight == 1) {
						optionsGetch(newValue, options_menu_win);
						erase();
						refresh();
						setWindowsSize(atoi(newValue), Height, &Width, &Height);
						start_x = (int)COLS/2-(int)Width/2-1;
						start_y = (int)LINES/2-(int)Height/2-1;
						game_win = newwin(Height+2, Width+2, start_y, start_x);
						box(game_win, 0, 0);
						wrefresh(game_win);
					} else if (highlight == 2) {
						optionsGetch(newValue, options_menu_win);
						erase();
						refresh();
						setWindowsSize(Width, atoi(newValue), &Width, &Height);
						erase();
						start_x = (int)COLS/2-(int)Width/2-1;
						start_y = (int)LINES/2-(int)Height/2-1;
						game_win = newwin(Height+2, Width+2, start_y, start_x);
						box(game_win, 0, 0);
						wrefresh(game_win);
					} else if (highlight == 3) {
						Symbol = wgetch(options_menu_win);
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					} else if (highlight == 4) {
						optionsGetch(newValue,options_menu_win);
						Refreshing_time = atof(newValue);
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					} else{
						current_state = main_menu;
						highlight = 1;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					choice = 0;
				} /* User did a choice come out of the infinite loop */
			break;

			case game_on: ;
				vector conway_state;
				Check = copyVector(&conway_state, &original_conway_state);
				if (!Check) {
					freeVector(1, &original_conway_state);
					return -1;
				}
				PrintWndw(game_win, &Width, &Height, &original_conway_state, &Symbol);
				float scaled_speed = 1.0;
				bool game_paused = false;
				mvprintw (start_y+Height+2, start_x, "r: reset");
				mvprintw (start_y+Height+3, start_x, "space bar: pause");
				mvprintw (start_y+Height+4, start_x, "esc: exit game");
				mvprintw (start_y+Height+5, start_x, "-> arrow: increment speed");
				mvprintw (start_y+Height+6, start_x, "<- arrow: decrement speed");
				refresh();
				box(game_win, 0, 0);
				wrefresh(game_win);
				wtimeout(game_win, (int)(Refreshing_time/scaled_speed*1000));
				keypad(game_win, TRUE);
				int movement;
				do {
					mvprintw (start_y+Height+2, start_x+Width-17, "speed factor: x%.02f", scaled_speed);
					refresh();
					movement = wgetch(game_win);
					bool refresh = true;
					switch (movement) {
						case KEY_RIGHT:
							if (scaled_speed < 4) {
								scaled_speed *= 2.0;
								wtimeout(game_win, (int)(Refreshing_time/scaled_speed*1000));
							}
						break;

						case KEY_LEFT:
							if (scaled_speed > 0.25) {
								scaled_speed /= 2;
								wtimeout(game_win, (int)(Refreshing_time/scaled_speed*1000));
							}
						break;

						case 32:
							if (!game_paused) {
								game_paused = true;
								wtimeout(game_win, -1);
							} else {
								game_paused = false;
								wtimeout(game_win, (int)(Refreshing_time/scaled_speed*1000));
							}
						break;

						case 114:
							freeVector(1, &conway_state);
							Check = copyVector(&conway_state, &original_conway_state);
							if (!Check) {
								freeVector(1, &original_conway_state);
								return -1;
							}
							wclear(game_win);
							box(game_win, 0, 0);
							wrefresh(game_win);
							PrintWndw(game_win, &Width, &Height, &original_conway_state, &Symbol);
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
					if (refresh) {
						Check_int = iterateConway(&conway_state, Width, Height);
						if (Check_int == -1) {
							freeVector(2, &original_conway_state, &conway_state);
							return -1;
						}
						wclear(game_win);
						box(game_win, 0, 0);
						PrintWndw(game_win, &Width, &Height, &conway_state, &Symbol);
						wrefresh(game_win);
					}
					if (!conway_state.size) {
						current_state = play_menu;
						break;
					}

				} while (movement != 27);
				wclear(game_win);
				box(game_win, 0, 0);
				wrefresh(game_win);
				freeVector(1, &conway_state);
			break;
		}
	}

	// Clear memory
	freeVector(1, &original_conway_state);

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
