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
    free(v->array);
    v->array = NULL;
    v->size = 0;
    v->capacity = 0;
}
bool pushBack(vector *v, const cell *c) {
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
    if (v->size) {
        v->size--;
    } else {
        fprintf(stderr, "cannot popBack empty vector\n");
        return false;
    }
    return true;
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

int compareCells(const cell *c1, const cell *c2) {
	return (c1->id > c2->id);
}

int isInArray(const vector *v, const int id) {
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
