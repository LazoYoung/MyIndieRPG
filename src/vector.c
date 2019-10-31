#include <stdlib.h>
#include <math.h>
#include "header/vector.h"

/* Add two vectors: a + b */
void add(Vector a, Vector b, Vector *result) {
    (*result)[0] = a[0] + b[0];
    (*result)[1] = a[1] + b[1];
}

/**
 * Add two vectors: a + b
 * Vector b is given by 'b_x' and 'b_y'
 **/
void add_m(Vector a, float b_x, float b_y, Vector *result) {
    (*result)[0] = a[0] + b_x;
    (*result)[1] = a[1] + b_y;
}

/* Subtract two vectors: a - b */
void sub(Vector a, Vector b, Vector *result) {
    (*result)[0] = a[0] - b[0];
    (*result)[1] = a[1] - b[1];
}

/* Scale a vector by 'scalar' */
void scale(Vector a, float scalar, Vector *result) {
    (*result)[0] = scalar * a[0];
    (*result)[1] = scalar * a[1];
}

/* Measure the distance between two location: p1 and p2*/
float distance(Vector p1, Vector p2) {
    double a = pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2);
    return sqrt(a);
}