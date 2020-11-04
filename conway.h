#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>

typedef struct cell {
	int id; // linear index encoding location
	unsigned char live_neighbours; // number of live neighbours
} cell;

typedef struct vector {
    cell *array;
    unsigned short size;
    unsigned short capacity;
} vector;

// vector functions
bool initVector(vector *v, const unsigned short init_size);
void freeVector(vector *v);
bool pushBack(vector *v, const cell *c);
bool popBack(vector *v);
int compareCells(const void *a, const void *b);
void sortVector(vector *v);

// conway functions
int xy2id(const unsigned short x, const unsigned short y, size_t columns);
int isInArray(const vector *v, const int id);
void get8nn(int *array, const int id, size_t columns);

#endif
