#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <ncurses.h>
#include "conway.h"

// Formatting considerations:
//   - Try to stick to standard 80-character width (default terminal)
//   - Name local variables using lowercase and underscores: my_variable
//   - For global variables, capitalise the first letter: My_global_variable
//   - Constants (#define) in capital letters: VARIABLE
//   - Name functions with lowercase and capitals: myFunction

int main() {

	// NCURSES implementation here. Define window size and obtain COLS and LINES
	// variables, which are then used to obtain linear indices of active cells.
	// For testing purposes, set values.
	size_t COLS = 80;
	size_t LINES = 24;

	// Create cells for testing purposes
	const unsigned short test_size = 4;
	cell c1 = {.id = xy2id(12, 40, COLS), .live_neighbours = 0};
	cell c2 = {.id = xy2id(14, 40, COLS), .live_neighbours = 0};
	cell c3 = {.id = xy2id(13, 41, COLS), .live_neighbours = 0};
	cell c4 = {.id = xy2id(13, 42, COLS), .live_neighbours = 0};

	// *state* is the malloc'ed array containing linear indices of active cells.
	// *state* should be sorted in ascending/descending order, which will make
	// cheking neighbour status more efficient. Use qsort() from stdlib.h for
	// this purpose.
	bool check = false; // keeps track of correct allocations
	vector state;
	check = initVector(&state, test_size);
	if (!check) {
		return -1;
	}
	// *state* must be filled here
	check = pushBack(&state, &c1);
	if (!check) {
		freeVector(&state);
		return -1;
	}
	check = pushBack(&state, &c2);
	if (!check) {
		freeVector(&state);
		return -1;
	}
	check = pushBack(&state, &c3);
	if (!check) {
		freeVector(&state);
		return -1;
	}
	check = pushBack(&state, &c4);
	if (!check) {
		freeVector(&state);
		return -1;
	}

	bool end_game = false;
	int eight_nn[8] = {0}; // indices of 8 nearest neighbours
	unsigned short coordinates[2] = {0}; // (x,y) coordinates of a cell
	// Main loop. Apply game logic to update *state* at every iteration. 
	//while (!end_game) {
	for (int k = 0; k < 3; k++) {
		// Show current state
		printf("Iteration %d:\n", k);
		for (unsigned short n = 0; n < state.size; n++) {
			id2xy(coordinates, state.array[n].id, COLS);
			printf("%hu   %hu\n", coordinates[0], coordinates[1]);
		}
		printf("=============\n");

		// Dynamic array that keeps track of dead cells encountered
		vector dead_cells;
		check = initVector(&dead_cells, state.size);
		if (!check) {
			freeVector(&state);
			return -1;
		}

		// For every live cell...
		for (unsigned int i = 0; i < state.size; i++) {
			get8nn(eight_nn, state.array[i].id, COLS, LINES);
			int in_live_array = -1;
			int in_dead_array = -1;
			// ... iterate over its 8 nearest neighbours
			for (unsigned char j = 0; j < 8; j++) {
				in_live_array = isInVector(&state, eight_nn[j]);
				if (in_live_array >= 0) {
					state.array[i].live_neighbours++;
				} else {
					in_dead_array = isInVector(&dead_cells, eight_nn[j]);
					if (in_dead_array >= 0) {
						dead_cells.array[in_dead_array].live_neighbours++;
					} else {
						cell c = {.id = eight_nn[j], .live_neighbours = 1};
						check = pushBack(&dead_cells, &c);
						if (!check) {
							freeVector(&dead_cells);
							freeVector(&state);
							return -1;
						}
					}
				}
			}
		}

		// Remove underpopulated live cells
		sortVectorDescending(&state);
		while (state.array[state.size - 1].live_neighbours < 2) {
			check = popBack(&state);
			if (!check) {
				freeVector(&dead_cells);
				freeVector(&state);
				return -1;
			}
		}
		
		// Remove overpopulated live cells
		sortVectorAscending(&state);
		while (state.array[state.size - 1].live_neighbours > 3) {
			check = popBack(&state);
			if (!check) {
				freeVector(&dead_cells);
				freeVector(&state);
				return -1;
			}
		}
		
		// Evolve dead cells
		sortVectorAscending(&dead_cells);
		for (unsigned short i = 0; i < dead_cells.size; i++) {
			if (dead_cells.array[i].live_neighbours == 3) {
				check = pushBack(&state, &(dead_cells.array[i]));
				if (!check) {
					freeVector(&dead_cells);
					freeVector(&state);
					return -1;
				}
			} else if (dead_cells.array[i].live_neighbours > 3) {
				break;
			}
		}

		// Reset number of live neighbours of live cells
		resetNeighbours(&state);

		// Clear memory
		freeVector(&dead_cells);

		// Trigger *end_game* from NCURSES here
	}

	// Clear memory
	freeVector(&state);

	return 0;
}
