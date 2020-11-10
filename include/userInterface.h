// Coder 2 & Coder 3

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "conway.h"

////////////////////////////
//                        //
//    display functions   //
//                        //
////////////////////////////

// display functions
void printMenu (WINDOW *win, int highlight, int n_choices, char *choices_menu[]);
void printGameMenu(WINDOW *win, int highlight, int n_choices, char *choices_game[]);
void printOptionsMenu(WINDOW *win, int highlight, int n_choices, int width, int height, char symbol, float refreshing_time, char *choices_options[]);
void keyPressedMenu (WINDOW *win, int *highlight, int n_choices, int *choice);
void setWindowsSize(int width_size,int height_size, int *width, int *height);
void optionsGetch (char *buffer, WINDOW *win);
int initMode (vector *v, int width, int *x, int *y, char size);
int getUserSim(WINDOW *win, vector *v, int start_x, int start_y, int width, int height, char symbol);
void printWndw (WINDOW *win, int width, vector *v, char symbol);

#endif
