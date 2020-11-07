#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>

// Check for correct memory allocation and clear memory in case of error.
#define CHECK_ALLOC(...) \
	if (!check) { freeVector(__VA_ARGS__); return -1; } \
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

// vector functions
bool initVector(vector *v, const unsigned short init_size);
void freeVector(const unsigned int n, ...);
bool pushBack(vector *v, const cell *c);
bool popBack(vector *v);
int isInVector(const vector *v, const int id);
int compareCellsDescending(const void *a, const void *b);
void sortVectorDescending(vector *v);
int compareCellsAscending(const void *a, const void *b);
void sortVectorAscending(vector *v);

// conway functions
int iterateConway(vector *v, size_t columns, size_t lines);
int xy2id(const unsigned short x, const unsigned short y, size_t columns);
void id2xy(unsigned short *array, const int id, size_t columns);
void get8nn(int *array, const int id, size_t columns, size_t lines);
void resetNeighbours(vector *v);

#endif
