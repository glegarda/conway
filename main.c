#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "conway.h"

// **************************
// ***** DO NOT COMPILE *****
// **************************

// This is only a schematic file. Most declarations and initialisations missing.
// unsigned short types have been used because they only occupy 2 bytes and
// should have enough range for the purpose of this program

// Formatting considerations:
//   - Try to stick to standard 80-character width (default terminal)
//   - Name local variables using lowercase and underscores: my_variable
//   - For global variables, capitalise the first letter: My_global_variable
//   - Constants (#define) in capital letters: VARIABLE
//   - Name functions with lowercase and capitals: myFunction

int main() {

	// NCURSES implementation here. Define window size and obtain COLS and LINES
	// variables, which are then used to obtain linear indices of active cells.

	// *size* is the number of active cells in the initial configuration.
	unsigned short size;

	// *state* is the malloc'ed array containing linear indices of active cells.
	// *state* should be sorted in ascending/descending order, which will make
	// cheking neighbour status more efficient. Use qsort() from stdlib.h for
	// this purpose.
	unsigned short *state = (unsigned short *) malloc(size * sizeof(unsigned short));
	if (state == NULL) {
		fprintf(stderr, "malloc failed\n");
		return -1;
	}
	// *state* must be filled here

	// Main loop. Apply game logic to update *state* at every iteration. 
	while (true) {
		size = sizeof(state) / sizeof(state[0]);
		for (unsigned short i = 0; i < size; i++) {
			
		}
	}

	// Clear memory
	free(state);

	return 0;
}
