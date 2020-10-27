// Declare the functions that should be implemented in a separate conway.c file.

// Return number of live neighbours of a given cell
unsigned char nLiveNeighbours(const unsigned short state,
							  const unsigned short cell);

// Check if any (exactly) three live cells share a common dead cell, and return
// the location of that cell
unsigned short evolvingCell(const unsigned short state);

// Check if a value is in an array
bool isInArray(const unsigned short cell);
