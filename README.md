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

### **Game**
Once the game is played, the iterations are computed using _iterateConway_ function as follows:

    1. For the current state an array (malloc) of cell (let's call it dead_cells) is initialised
    2. Iterate over every cell in state:
	   * 8 nearest neighbours are computed
       * Iterate over each of its 8-nearest neighbours:
         * If neighbour is alive, cell.live_neighbours += 1; but if neighbour is dead, add neighbour to dead_cells (if it is not there already) and neighbour.live_cells += 1
    By now, we have two arrays containing the location and the number of live neighbours of each live cell and of each dead cell that might evolve in the next iteration, respectively. We can then apply game logic to these two arrays.
    3. Iterate over every cell in state: if it does not have 2/3 live neighbours, remove from state.
    4. Iterate over every cell in dead_cells: if it has 3 live neighbours, add to state.

### **.c and .h**
Some useful libraries using c files and header files has been implemented.
1. conway: It basically implements the structures required for the iterations during the game,
some vector treatment functions and memory allocation checking. We are implementing our game using two structures:
   - Cell structure: That has an id to locate the different cells on the map and the number of live neighbours for that cell.
   - Vector structure: it has an array of cells, the current size of the vector and the capacity.
   These two parameters are used for augmenting and reducing the size of the vector if necessary.

2. userInterface: It includes some useful functions for displaying and key input purposes.


### **Structure for the main.c**
Firstly, ncurses initialization is done and then the game window and some global variables used in the game are initialized. The the code enters in a while loop in which a switch case is implemented for the different states of the game.
At each case, we create its respective window, and depending on the user actions, the state changes.


### **Contributors**
- Guillermo Legarda Herranz
- Antonio Miranda Moreno
- Lucía Teruel Saldaña
