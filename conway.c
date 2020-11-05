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

int isInVector(const vector *v, const int id) {
	// Check if cell is in vector and return its index
	for (unsigned short i = 0; i < v->size; i++) {
		if (v->array[i].id == id) {
			return i;
		}
	}
	return -1;
}

int compareCellsDescending(const void *a, const void *b) {
	// Compares cells based on number of live neighbours for sorting purposes
	const cell cell_1 = *(const cell *) a;
	const cell cell_2 = *(const cell *) b;
	const int ln1 = cell_1.live_neighbours;
	const int ln2 = cell_2.live_neighbours;
	return (ln1 < ln2) - (ln1 > ln2);
}

void sortVectorDescending(vector *v) {
	// Sorts vector of cells in descending order of number of live neighbours
	qsort(v->array, v->size, sizeof(cell), compareCellsDescending);
}

int compareCellsAscending(const void *a, const void *b) {
	// Compares cells based on number of live neighbours for sorting purposes
	const cell cell_1 = *(const cell *) a;
	const cell cell_2 = *(const cell *) b;
	const int ln1 = cell_1.live_neighbours;
	const int ln2 = cell_2.live_neighbours;
	return (ln1 > ln2) - (ln1 < ln2);
}

void sortVectorAscending(vector *v) {
	// Sorts vector of cells in ascending order of number of live neighbours
	qsort(v->array, v->size, sizeof(cell), compareCellsAscending);
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

void get8nn(int *array, const int id, size_t columns, size_t lines) {
	// Return linear indices of 8 nearest neighbours of cell at id
	// Assign default values
	array[0] = id - columns - 1;
	array[1] = id - columns;
	array[2] = id - columns + 1;
	array[3] = id - 1;
	array[4] = id + 1;
	array[5] = id + columns - 1;
	array[6] = id + columns;
	array[7] = id + columns + 1;

	// Reassing accounting for end-of-grid by wrapping the environment
	if (id < columns) { // top row
		array[0] = columns * (lines - 1) + id - 1;
		array[1] = columns * (lines - 1) + id;
		array[2] = columns * (lines - 1) + id + 1;
		if (id == 0) { // top-left corner
			array[0] = columns * lines - 1;
			array[3] = id + columns - 1;
			array[5] = id + 2 * columns - 1;
		} else if (id == columns - 1) { // top-right corner
			array[2] = columns * (lines - 1);
			array[4] = id - columns + 1;
			array[7] = id + 1;
		}
	} else if (id >= columns * (lines - 1)) { // bottom row
		array[5] = (id - 1) % columns;
		array[6] = id % columns;
		array[7] = (id + 1) % columns;
		if (id == columns * (lines - 1)) { // bottom-left corner
			array[0] = id - 1;
			array[3] = id + columns - 1;
			array[5] = columns - 1;
		} else if (id == columns * lines - 1) { // bottom-right corner
			array[2] = id - 2 * columns + 1;
			array[4] = id - columns + 1;
			array[7] = 0;
		}
	} else if (id % columns == 0) { // left column
		array[0] = id - 1;
		array[3] = id + columns - 1;
		array[5] = id + 2 * columns - 1;
	} else if (id % columns == columns - 1) { // right column
		array[2] = id - 2 * columns + 1;
		array[4] = id - columns + 1;
		array[7] = id + 1;
	}
}

void resetNeighbours(vector *v) {
	// Set number of live neighbours of every cell in vector to zero
	for (unsigned short i = 0; i < v->size; i++) {
		v->array[i].live_neighbours = 0;
	}
}
