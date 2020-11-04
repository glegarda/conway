#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "conway.h"

//////////////////////
//                  //
// vector functions //
//                  //
//////////////////////

bool initVector(vector *v, const unsigned short init_size) {
	// Initialise struct members
    v->array = (cell *) malloc(init_size * sizeof(cell));
    if (v->array == NULL) {
        fprintf(stderr, "malloc failed\n");
        return false;
    }
    v->size = 0;
    v->capacity = init_size;
    return true;
}
void freeVector(vector *v) {
	// Clean up memory
    free(v->array);
    v->array = NULL;
    v->size = 0;
    v->capacity = 0;
}
bool pushBack(vector *v, const cell *c) {
	// Add element at the end
    if (v->size == v->capacity) {
        v->capacity *= 2;
        cell *tmp = (cell *) realloc(v->array, v->capacity * sizeof(cell));
        if (tmp == NULL) {
            fprintf(stderr, "realloc failed\n");
            return false;
        } else {
            v->array = tmp;
        }
    }
    v->array[v->size++] = *c;
    return true;
}
bool popBack(vector *v) {
	// Remove last element
    if (v->size) {
        v->size--;
    } else {
        fprintf(stderr, "cannot popBack empty vector\n");
        return false;
    }
    return true;
}

int compareCells(const void *a, const void *b) {
	// Compares cells based on ID magnitude for sorting purposes
	const cell cell_1 = *(const cell *) a;
	const cell cell_2 = *(const cell *) b;
	const int id1 = cell_1.id;
	const int id2 = cell_2.id;
	return (id1 < id2) - (id1 > id2);
}

void sortVector(vector *v) {
	// Sorts vector of cells in descending order of ID
	qsort(v->array, v->size, sizeof(cell), compareCells);
}

//////////////////////
//                  //
// conway functions //
//                  //
//////////////////////

int xy2id(const unsigned short x, const unsigned short y, size_t columns) {
	// Transform (x,y) coordinates to a linear index (left-right, top-bottom)
	int id = (int) (x * columns + y);
	return id;
}

int isInArray(const vector *v, const int id) {
	// Check if cell is in vector and return its index
	for (unsigned short i = 0; i < v->size; i++) {
		if (v->array[i].id == id) {
			return i;
		}
	}
	return -1;
}

void get8nn(int *array, const int id, size_t columns) {
	// Return linear indices of 8 nearest neighbours of cell at id.
	// *** NOTE: MUST ACCOUNT FOR END OF GRID ***
	array[0] = id - columns - 1;
	array[1] = id - columns;
	array[2] = id - columns + 1;
	array[3] = id - 1;
	array[4] = id + 1;
	array[5] = id + columns - 1;
	array[6] = id + columns;
	array[7] = id + columns + 1;
}
