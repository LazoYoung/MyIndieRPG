#include <stdlib.h>
#include "header/vector.h"

/* Add two vectors: a + b */
Vector* add(Vector a, Vector b) {
    Vector* result = calloc(2, sizeof(float));
    (*result)[0] = a[0] + b[0];
    (*result)[1] = a[1] + b[1];
    return result;
}

/**
 * Add two vectors: a + b
 * Vector b is given by 'b_x' and 'b_y'
 **/
Vector* add_m(Vector a, float b_x, float b_y) {
    Vector* result = calloc(2, sizeof(float));
    (*result)[0] = a[0] + b_x;
    (*result)[1] = a[1] + b_y;
    return result;
}

/* Subtract two vectors: a - b */
Vector* sub(Vector a, Vector b) {
    Vector* result = calloc(2, sizeof(float));
    (*result)[0] = a[0] - b[0];
    (*result)[1] = a[1] - b[1];
    return result;
}

/* Scale a vector by 'scalar' */
Vector* scale(Vector a, float scalar) {
    Vector* result = calloc(2, sizeof(float));
    (*result)[0] = scalar * a[0];
    (*result)[1] = scalar * a[1];
    return result;
}