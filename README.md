## Conway's game of life

### **Introduction**
Contains source code for Conway's Game of Life.
Created for the ERTS course of the Master's Degree in Automatic Control and Robotics
of the UPC. In order to play the game, running the makefile is first required.

### **Interface**
A full integrated and adaptative interface has been design for the game. It has
different windows and menus for each of the modes during the game. the different
menus can be explored using up and down arrows and clicking enter button.
- Main menu: We can choose  whether to play the game, enter the options menu or exit
- Options menu: We can modify some parameters like height and width for the game
window, the alive cell symbol and refreshing time for the different iterations.
- Game menu: Once we enter in the game menu, we can choose between different test
structures and create our own starting configuration by user input.

### **Installation**
After cloning the repository, run `$ make all clean`.

### **Game**
To play the game, run `$ ./conwayMake`. During the game, the iterations are computed using _iterateConway_ function as follows:

1. An empty array of dead cells is initialised
2. Iterate over every live cell in the current state:
	- Iterate over each of its eight nearest neighbours:
		* If it is alive, add one to the number of live neighbours of the live cell.
		* Otherwise, check if the neighbour is in the array of dead cells. If it is, add one to the number of live neighbours of the dead cell; if it is not, add the neighbour to the array of dead cells and set its number of live neighbours to one.
3. At this point, we have two arrays containing the location and the number of live neighbours of each live cell and of each dead cell that might evolve in the next iteration, respectively. We can then apply game logic to these two arrays:
	- Iterate over every live cell. If it does not have two or three live neighbours, remove it from the array of live cells.
	- Iterate over every dead cell. If it has three live neighbours, add it to the array of live cells.


### **.c and .h**
Some useful libraries using c files and header files has been implemented.
1. It implements the structures required for the iterations during the game and to make memory management less cumbersome. We are implementing our game using two structures:
   - Cell structure: it has an ID to locate the different cells on the grid and the number of live neighbours of that cell.
   - Vector structure: it has an array of cells, the current size of the vector, which keeps track of the number of elements added/removed, and its capacity, which is doubled whenever one tries to add an element to a full vector (i.e. when size = capacity).

2. userInterface: It includes some useful functions for displaying and key input purposes.


### **Structure for the main.c**
Firstly, ncurses initialization is done and then the game window and some global variables used in the game are initialized. The the code enters in a while loop in which a switch case is implemented for the different states of the game.
At each case, we create its respective window, and depending on the user actions, the state changes.


### **Contributors**
- Guillermo Legarda Herranz
- Antonio Miranda Moreno
- Lucía Teruel Saldaña
