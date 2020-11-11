#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ncurses.h>
#include <conway.h>

////////////////////////////
//                        //
//    display functions   //
//                        //
////////////////////////////

// Coder 2 /////////////////////////////////////////////////////////////////////
void printMenu(WINDOW *win, int highlight, int n_choices, char *choices_menu[]) {
	// Prints Main menu
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for (i = 0; i < n_choices; ++i) {
		if (highlight == i + 1) { // Highlight the present choice
			wattron(win, A_REVERSE);
			mvwprintw(win, y, x, "%s", choices_menu[i]);
			wattroff(win, A_REVERSE);
		} else {
			mvwprintw(win, y, x, "%s", choices_menu[i]);
		}
		++y;
	}
	wrefresh(win);
}

void printGameMenu(WINDOW *win, int highlight, int n_choices, char *choices_game[]) {
	// Prints Play/Game menu
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for (i = 0; i < n_choices; ++i) {
		if (highlight == i + 1) { // Highlight the present choice
			wattron(win, A_REVERSE);
			mvwprintw(win, y, x, "%s", choices_game[i]);
			wattroff(win, A_REVERSE);
		} else {
			mvwprintw(win, y, x, "%s", choices_game[i]);
		}
		++y;
	}
	wrefresh(win);
}

void printOptionsMenu(WINDOW *win, int highlight, int n_choices, int width, int height, char symbol, float refreshing_time, char *choices_options[]) {
	// Prints Options menu
	clear();
	int x, y, i;

	x = 2;
	y = 2;
	box(win, 0, 0);
	for (i = 0; i < n_choices-1; ++i) {
		// In this case we only highlight the parameter
		mvwprintw(win, y, x, "%s", choices_options[i]);
		if(highlight == i + 1) { // Highlight the parameter
		  wattron(win, A_REVERSE);
		}
		if (i == 0) {
			wprintw(win, "%d", width);
		} else if (i == 1) {
			wprintw(win, "%d", height);
		} else if (i == 2) {
			wprintw(win, "%c", symbol);
		} else if (i == 3) {
			wprintw(win,"%.02f", refreshing_time);
		}
		if(highlight == i + 1) {
			wattroff(win, A_REVERSE);
		}
		++y;
	}
	// But if we are on the back option, it is highlighted
	if (highlight == n_choices) {
		wattron(win, A_REVERSE);
		mvwprintw(win, y, x, "%s", choices_options[n_choices-1]);
		wattroff(win, A_REVERSE);
	} else {
		mvwprintw(win, y, x, "%s", choices_options[n_choices-1]);
	}
	wrefresh(win);
}

void keyPressedMenu(WINDOW *win, int *highlight, int n_choices, int *choice) {
	// Reads arrows, updates cursor's position and reads selected choice when clicking enter
	int c = wgetch(win);
	switch(c) {
		case KEY_UP:
			if (*highlight == 1) {
				*highlight = n_choices;
			} else {
				--(*highlight);
			}
			break;
		case KEY_DOWN:
			if (*highlight == n_choices) {
				*highlight = 1;
			} else {
				++(*highlight);
			}
			break;
		case 10: // ENTER
			*choice = *highlight;
			break;
		default:
			wrefresh(win);
			break;
	}
}

void setWindowsSize(int width_size,int height_size, int *width, int *height) {
	// Updates window width and height variables if some restrictions are acomplished
 	if (width_size < COLS && height_size < LINES && width_size > 42 && height_size > 17) {
		*width = width_size;
		*height = height_size;
	}
}

// Coder 3 /////////////////////////////////////////////////////////////////////
void optionsGetch(char *buffer, WINDOW *win) {
	// Reads keyboard input in options menu for modifying height and width and refreshing time (all of them requires more than one number)
	curs_set(1);
	nocbreak();
	echo();
	wgetstr(win, buffer);
	curs_set(0);
	noecho();
	cbreak();
	keypad(win, TRUE);
}

int initMode(vector *v, int width, int *x, int *y, char size) {
	// Receives x&y coord. transforms and inputs them into the cells vector
	bool check = true;
	cell c;
	c.live_neighbours = 0;
	for (int i=0; i<size; i++){
		c.id = xy2id(x[i], y[i], width);
		check = pushBack(v, &c);
		if (!check) {
			freeVector(1, v);
			return -1;
		}
	}
	return 0;
}

int getUserSim(WINDOW *win, vector *v, int start_x, int start_y, int width, int height, char symbol) {
	// Get Users' Simulation. Reads user's input and stores the positions
	curs_set(1);
	keypad(win, TRUE);
	mvprintw (start_y+height+2, start_x, "Use arrows or WASD to move arround");
	mvprintw (start_y+height+3, start_x, "spacebar: print live cell");
	mvprintw (start_y+height+4, start_x, "enter: Start game");
	mvprintw (start_y+height+2, start_x+width-17, "esc: exit game");
	mvprintw (start_y+height+3, start_x+width-17, "r: reset");
	box(win, 0, 0);
	refresh();

	int x_loc = 0;
	int y_loc = 0;
	int x_max = width-1;
	int y_max = height-1;

	int movement;
	int check = 0;

    wmove(win, y_loc+1, x_loc+1);
    wrefresh(win);
	// Users' tracking loop
	do {
		// Tracks users' movements
		movement = wgetch(win);
		switch (movement){
			case KEY_UP:
			case 'w':
			case 'W':
				if (y_loc > 0) {
					y_loc--;
				}
				break;
			case KEY_DOWN:
			case 's':
			case 'S':
				if (y_loc < y_max) {
					y_loc++;
				}
				break;
			case KEY_LEFT:
			case 'a':
			case 'A':
				if (x_loc > 0) {
					x_loc--;
				}
				break;
			case KEY_RIGHT:
			case 'd':
			case 'D':
				if (x_loc < x_max) {
					x_loc++;
				}
				break;
			case 32: // Spacebar. Prints live cell.
				mvwprintw(win, y_loc+1, x_loc+1, "%c", symbol);
				check = initMode(v, width, &x_loc, &y_loc, 1);
				if (check == -1) {
					freeVector(1, v);
					return -1;
				}
				break;
			case 'r':
				v->size = 0; // Reset state
				wclear(win);
				box(win, 0, 0);
				wrefresh(win);
			default:
				break;
		}
		// Updates cursor
		wmove(win, y_loc+1, x_loc+1);
		wrefresh(win);
	} while (movement != 10 && movement != 27);
	// ESC or ENTER have been pressed.
	clear();
	refresh();
	curs_set(0);
	keypad(win, FALSE);
	if (movement == 27) { //ESC. Return back to Game menu
		wclear(win);
		box(win, 0, 0);
		wrefresh(win);
		return 1;
	} else { //ENTER. Start game
		return 0;
	}
}

void printWndw(WINDOW *win, int width, vector *v, char symbol) {
	// Prints active cells
	unsigned short coordinates[2] = {0};
	for (unsigned short i = 0; i < (v->size); i++) {
		id2xy(coordinates, v->array[i].id, width);
		coordinates[0]++;
		coordinates[1]++;
		mvwprintw(win, coordinates[1], coordinates[0], "%c", symbol);
	}
	wrefresh(win);
}
