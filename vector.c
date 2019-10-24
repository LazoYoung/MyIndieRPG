#include <stdlib.h>
#include "vector.h"

void addVector(Vector a, Vector b, Vector* result) {
    result = calloc(2, sizeof(float));
    *result[0] = a[0] + b[0];
    *result[1] = a[1] + b[1];
}

void subVector(Vector a, Vector b, Vector* result) {
    result = calloc(2, sizeof(float));
    *result[0] = a[0] - b[0];
    *result[1] = a[1] - b[1];
}