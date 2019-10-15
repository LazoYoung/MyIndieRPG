#include "vector.h"

Vector addVector(Vector a, Vector b) {
    Vector v = {a.x + b.x, a.y + b.y};
    return v;
}

Vector subVector(Vector a, Vector b) {
    Vector v = {a.x - b.x, a.y - b.y};
    return v;
}