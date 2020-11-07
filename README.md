# conway
Contains source code for Conway's Game of Life.
Created for the ERTS course of the Master's Degree in Automatic Control and Robotics of the UPC.

Edit with the following info from the pseudocode:
The state of the game is defined by an array of cell objects representing all live cells.
At each iteration:
    1. Initialise an array (malloc) of cell (let's call it dead_cells) 
    2. Iterate over every cell in state:
        2.1. Iterate over each of its 8-nearest neighbours:
            2.1.1. If neighbour is alive, cell.live_neighbours += 1; but if neighbour is dead, add neighbour to dead_cells (if it is not there already) and neighbour.live_cells += 1
    // By now, we have two arrays containing the location and the number of live neighbours of each live cell and of each dead cell that might evolve in the next iteration, respectively. We can then apply game logic to these two arrays.
    3. Iterate over every cell in state: if it does not have 2/3 live neighbours, remove from state.
    4. Iterate over every cell in dead_cells: if it has 3 live neighbours, add to state.

