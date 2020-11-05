// 30x10 mainMenu
// 40x15 gameMenu

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h> //Con esta librería ya no da el 'warning' en la compilacion
#include <stdbool.h>

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
	"User input using mouse(?)",
	"Back",
};

char *choicesOptions[] = {
	"Width: ",
	"Heigth: ",
	"Cell Alive Symbol: ",
	"Refreshing time: ",
	"Back",
};

//******************************************************************************
typedef struct Structures {
	unsigned int alive; //Int or char?
	unsigned int pos;
} Structure_t;

enum states {mainMenu, playMenu, optionsMenu, gameOn};
enum states currentState = mainMenu;

void setWindowsSize(int widthSize,int heightSize);
void optionsGetch(char *buffer, WINDOW *win);
void R_pentomino(WINDOW *win, int *width, int *height);
void R_Diehard(WINDOW *win, int *width, int *height);
void R_Acorn(WINDOW *win, int *width, int *height);

int main(int argc, char *argv[])
{

	initscr();
  curs_set(0); //Makes the cursor invisible, otherwise it will keep blinking after the '*'
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	clear();

	bool escape;
//Nota: Esto es para meterle las dimensiones de la ventana desde el terminal al ejecutar el ejecutable (ex: ./userInterface 60 60)
  if(argc == 3){ //Nota: Si ve que le has metido otros dos argumentos a parte del ejecutable asume que son coordenadas
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

	R_Acorn(game_win, &width, &height);

	while(escape == false){
	}

}

void setWindowsSize(int widthSize,int heightSize){

  if(widthSize<COLS && heightSize<LINES && widthSize>42 && heightSize>17){
      width = widthSize;
      height = heightSize;
  }
}


void optionsGetch(char *buffer, WINDOW *win){
	curs_set(1);
	nocbreak();
	echo();
	wgetstr(win,buffer);
	curs_set(0);
	noecho();
	cbreak();
	keypad(win, TRUE);
}

//******************************************************************************
void R_pentomino(WINDOW *win, int *width, int *height){
	int strt_x = (*width+2)/2-1;
	int strt_y = (*height+2)/2-1;
	char R_pen[9] = {0,1,1,1,1,0,0,1,0};
	for (int i=0; i<9; i++){
		if (R_pen[i]==1){
			mvwprintw(win,strt_y+(i/3), strt_x+(i%3), "X");
		}
	}
	wrefresh(win);
}

void R_Diehard(WINDOW *win, int *width, int *height){
	int strt_x = (*width+2)/2-3;
	int strt_y = (*height+2)/2-1;
	char R_Die[24] = {0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1};
	for (int i=0; i<24; i++){
		if (R_Die[i]==1){
			mvwprintw(win,strt_y+(i/8), strt_x+(i%8), "X");
		}
	}
	wrefresh(win);
}

void R_Acorn(WINDOW *win, int *width, int *height){
	int strt_x = (*width+2)/2-3;
	int strt_y = (*height+2)/2-1;
	char R_Die[21] = {0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,1,1,1};
	for (int i=0; i<21; i++){
		if (R_Die[i]==1){
			mvwprintw(win,strt_y+(i/7), strt_x+(i%7), "X");
		}
	}
	wrefresh(win);
}
