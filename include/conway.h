// Coder 1 /////////////////////////////////////////////////////////////////////

#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>

// Check for correct memory allocation and clear memory in case of error.
#define CHECK_ALLOC(...) \
	if (!check) { freeVector(__VA_ARGS__); return false; } \
	else (void)0

typedef struct cell {
	int id; // linear index encoding location
	unsigned char live_neighbours; // number of live neighbours
} cell;

typedef struct vector {
	cell *array;
	unsigned short size; // occupied elements
	unsigned short capacity; // number of elements available in memory
} vector;

//////////////////////
//                  //
// vector functions //
//                  //
//////////////////////

bool initVector(vector *v, const unsigned short init_size);
bool copyVector(vector *v_c, const vector *v_o);
void freeVector(const unsigned int n, ...);
bool pushBack(vector *v, const cell *c);
bool popBack(vector *v);
int isInVector(const vector *v, const int id);
int compareCellsDescending(const void *a, const void *b);
void sortVectorDescending(vector *v);
int compareCellsAscending(const void *a, const void *b);
void sortVectorAscending(vector *v);

//////////////////////
//                  //
// conway functions //
//                  //
//////////////////////

bool iterateConway(vector *v, const int columns, const int lines);
int xy2id(const unsigned short x, const unsigned short y, const int columns);
void id2xy(unsigned short *array, const int id, const int columns);
void get8nn(int *array, const int id, const int columns, const int lines);
void resetNeighbours(vector *v);

#endif
