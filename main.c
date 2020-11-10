#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ncurses.h>
#include "conway.h"
#include "userInterface.h"

int Width; // Game window width
int Height; // Game window height
char Symbol = '*'; // Deafult game symbol
float Refreshing_time = 1.0; // Default screen refreshing time
bool Check = true; // Memory allocation success check variable
int Check_int;

// Game menus
char *Choices_menu[] = {
    // Main menu
    "PLAY",
    "OPTIONS",
    "EXIT",
};
char *Choices_game[] = {
    // PLAY menu
    "Test 1: R-pentomino",
    "Test 2: Diehard",
    "Test 3: Acorn",
    "User input using keyboard",
    "Back",
};
char *Choices_options[] = {
    // OPTIONS menu
    "Width: ",
    "Heigth: ",
    "Cell Alive Symbol: ",
    "Refreshing time: ",
    "Back",
};

enum states {main_menu, play_menu, options_menu, game_on}; // Different screens
enum states current_state = main_menu; // Current screen

int main (int argc, char *argv[]) {
	// NCURSES initialization
	initscr();
	curs_set(0); // Makes cursor invisible
	noecho();
	cbreak(); // Line buffering disabled
 	clear();

	// Create Exit-game variable
 	bool escape;
	// Create vector of initial live cells
	vector original_conway_state;

	// Allows user to input window height and width from terminal
	if (argc == 3) {
		int arg1 = atoi(argv[1]);
		int arg2 = atoi(argv[2]);
		setWindowsSize(arg1,arg2,&Width,&Height);
	} else { // By default the window is 3/4 of the terminals' dimensions
		setWindowsSize((int)(3.0/4*COLS),(int)(3.0/4*LINES), &Width, &Height);
	}

	// Menu variables
	int highlight = 1;
	int choice = 0;
	int c;

	// Initialises game window
	WINDOW *game_win;
	int start_x = (int)COLS/2-(int)Width/2-1;
	int start_y = (int)LINES/2-(int)Height/2-1;
	game_win = newwin(Height+2, Width+2, start_y, start_x);
	refresh();
	box(game_win, 0, 0); // Creates game box
	wrefresh(game_win);

	// Program loop. Run until escape is triggered.
	while (escape == false) {
		int n_choices;
		int start_x_menu, start_y_menu;

		// Check users' current window
		switch (current_state) {
			case main_menu: ;
				// Initialise Main menu
				n_choices = sizeof(Choices_menu) / sizeof(char *);
				WINDOW *menu_win;
				start_x_menu = (int)(COLS/2-15-1);
				start_y_menu = (int)(LINES/2-5-1);
				menu_win = newwin(10, 30,start_y_menu,start_x_menu);

				// Tracking users' movemenet through menu
				keypad(menu_win, TRUE);
				printMenu(menu_win, highlight, n_choices, Choices_menu);
				keyPressedMenu(menu_win, &highlight, n_choices, &choice);
				printMenu(menu_win, highlight, n_choices, Choices_menu);

				// Users' selected choice
				if (choice != 0) {
					if (highlight == 1) {
						// PLAY. Updates current state & window
						current_state = play_menu;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					} else if (highlight == 2) {
						//OPTIONS. Updates current state & window
						current_state = options_menu;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						highlight = 1;
					} else {
						// EXIT. Triggers game exit
						escape = true;
					}
					choice = 0; // A choice has been made, get out of the loop
				}
				break;

			case play_menu: ;
				// Initialise Play menu
				n_choices = sizeof(Choices_game) / sizeof(char *);
				WINDOW *gameMenu_win;
				start_x_menu = (int)(COLS/2-20-1);
				start_y_menu = (int)(LINES/2-7-1);
				gameMenu_win = newwin(15, 40,start_y_menu,start_x_menu);

				// Tracking users' movemenet through menu
				keypad(gameMenu_win, TRUE);
				printGameMenu(gameMenu_win, highlight,n_choices, Choices_game);
				keyPressedMenu(gameMenu_win, &highlight, n_choices, &choice);
				printGameMenu(gameMenu_win, highlight,n_choices, Choices_game);

				// Users' selected choice
				if (choice != 0) {
					int mid_x = (Width+2)/2-1; // Screen center point (x,y) coordinates
					int mid_y = (Height+2)/2-1;

					if (highlight==5) {
						// Back. Goes back to Main menu.
						current_state = main_menu;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						highlight = 1;
					} else {
						// Any other chosen option.
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
						Check = initVector(&original_conway_state, 10); // Set initial arbitrary size
						if (!Check) {
							escape = true;
							break;
						}
						// Load selected game mode:
						if (highlight == 1) {
							//Game mode 1: R-pentomino
							int x1[5] = {mid_x  , mid_x+1, mid_x-1, mid_x, mid_x  };
							int y1[5] = {mid_y-1, mid_y-1, mid_y  , mid_y, mid_y+1};
							Check_int = initMode(&original_conway_state, &Width, x1, y1, 5);
						} else if (highlight == 2) {
							//Game mode 2: Diehard
							int x2[7] = {mid_x+3, mid_x-3, mid_x-2, mid_x-2, mid_x+2, mid_x+3, mid_x+4};
							int y2[7] = {mid_y-1, mid_y  , mid_y  , mid_y+1, mid_y+1, mid_y+1, mid_y+1};
							Check_int = initMode(&original_conway_state, &Width, x2, y2, 7);
						} else if (highlight == 3) {
							//Game mode 3: Acorn
							int x3[7] = {mid_x-2,mid_x  , mid_x-3, mid_x-2, mid_x+1, mid_x+2, mid_x+3};
							int y3[7] = {mid_y-1,mid_y  , mid_y+1, mid_y+1, mid_y+1, mid_y+1, mid_y+1};
							Check_int = initMode(&original_conway_state, &Width, x3, y3, 7);
						} else {
							//Game mode 4: User inputs simulation
							Check_int = GetUserSim(game_win, &original_conway_state, &Width, &Height, &Symbol);
						}

						if (Check_int == -1) { //Check if memory allocation was successful
							escape = true;
							break;
						} else if (Check_int == 0) { //Start game
							current_state = game_on;
						} else if (Check_int == 1) {
							//Go back to Main menu
							original_conway_state.size = 0; // Reset state
							current_state = play_menu;
						}
					}
				}
				choice = 0; // A choice has been made, get out of the loop
				break;

			case options_menu: ;
				// Initialise Options menu
				n_choices = sizeof(Choices_options) / sizeof(char *);
				WINDOW *options_menu_win;
				start_x_menu = (int)(COLS/2-20-1);
				start_y_menu = (int)(LINES/2-7-1);
				options_menu_win = newwin(15, 40, start_y_menu, start_x_menu);

				// Tracking users' movemenet through menu
				keypad(options_menu_win, TRUE);
				printOptionsMenu(options_menu_win, highlight, n_choices, &Width, &Height, &Symbol, &Refreshing_time, Choices_options);
				keyPressedMenu(options_menu_win, &highlight, n_choices, &choice);
				printOptionsMenu(options_menu_win, highlight, n_choices, &Width, &Height, &Symbol, &Refreshing_time, Choices_options);

				// Users' selected choice
				if (choice != 0) {
					if (highlight < 5) {
						// New value is going to be introduced, display message
						mvwprintw(options_menu_win, 10, 2, "Introduce new %s", Choices_options[highlight-1]);
						wrefresh(options_menu_win);
					}
					char newValue[5];
					if (highlight == 1) {
						// New Width. Read new value, update window size & screen.
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
						// New Height. Read new value, update window size & screen.
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
						// New Symbol. Read new symbol & update.
						Symbol = wgetch(options_menu_win);
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					} else if (highlight == 4) {
						// New Refreshing time. Read new value & update.
						optionsGetch(newValue,options_menu_win);
						Refreshing_time = atof(newValue);
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					} else{
						// Back. Goes back to Main menu.
						current_state = main_menu;
						highlight = 1;
						clear();
						box(game_win, 0, 0);
						wrefresh(game_win);
					}
					choice = 0; // A choice has been made, get out of the loop
				}
				break;

			case game_on: ;
				// GAME
				// Initialise game-state vector
				vector conway_state;
				Check = copyVector(&conway_state, &original_conway_state);
				if (!Check) {
					escape = true;
					break;
				}
				// Print game starting point
				PrintWndw(game_win, &Width, &Height, &original_conway_state, &Symbol);
				// Print game-menu on the lower part of the screen
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
				wtimeout(game_win, (int)(Refreshing_time/scaled_speed*1000)); // Set screen refreshing time
				keypad(game_win, TRUE);

				// Game loop. Run until ESC is pressed.
				int movement;
				do {
					// Update screen
					mvprintw (start_y+Height+2, start_x+Width-17, "speed factor: x%.02f", scaled_speed);
					refresh();
					movement = wgetch(game_win);
					bool refresh = true;
					// Check for new input screen refresh speed
					switch (movement) {
						case KEY_RIGHT:
							// Increase factor = decrease refreshing time
							if (scaled_speed < 4) {
								scaled_speed *= 2.0;
								wtimeout(game_win, (int)(Refreshing_time/scaled_speed*1000));
							}
							break;

						case KEY_LEFT:
							// Decrease factor = increase refreshing time
							if (scaled_speed > 0.25) {
								scaled_speed /= 2;
								wtimeout(game_win, (int)(Refreshing_time/scaled_speed*1000));
							}
							break;

						case 32: // Spacebar
							// Pause/Play game
							if (!game_paused) { // Pause game
								game_paused = true;
                                refresh = false;
								wtimeout(game_win, -1);
							} else { // Continue game
								game_paused = false;
								wtimeout(game_win, (int)(Refreshing_time/scaled_speed*1000));
							}
							break;

						case 114: // 'r'
							// Reset game
							// Resets state vector to initial/original state
							freeVector(1, &conway_state);
							Check = copyVector(&conway_state, &original_conway_state);
							if (!Check) { // Check for unsuccessful memory allocation
								movement = 27;
								escape = true;
								break;
							}
							wclear(game_win);
							box(game_win, 0, 0);
							wrefresh(game_win);
							PrintWndw(game_win, &Width, &Height, &original_conway_state, &Symbol);
							break;

						case 27: // ESC
							// Exit the game. Back to Play menu.
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
					// Game iteration
					// Apply game logic to update the state at every iteration.
					if (refresh) {
						Check = iterateConway(&conway_state, Width, Height);
						if (!Check) {
							freeVector(1, &conway_state);
							escape = true;
							break;
						}
						// Print new iteration
						wclear(game_win);
						box(game_win, 0, 0);
						PrintWndw(game_win, &Width, &Height, &conway_state, &Symbol);
						wrefresh(game_win);
					}
					// Game ends. No more living cells. Go back to Play menu.
					if (!conway_state.size) {
						current_state = play_menu;
						break;
					}
				} while (movement != 27);
				// ESC has been pressed or memory allocation was unsuccessful
				// Game ends. State memory is cleared. Back to Play menu.
				if (!Check || Check_int == -1) {
					break;
				}
				wclear(game_win);
				box(game_win, 0, 0);
				wrefresh(game_win);
				if (conway_state.array) {
					freeVector(1, &conway_state);
				}
				break;
		}
	}

	// Clear memory
	if (original_conway_state.array) {
		freeVector(1, &original_conway_state);
	}

	// Mac Problem
	// These lines solve a Mac-related problem with the program termination and
	// the terminal, as it would otherwise get stuck.
	curs_set(1);
	nocbreak();
	echo();
	erase();
	refresh();
	endwin();

	return 0;
}
