#include "level.h"

typedef struct { // Axis Aligned Bounding Box
    float centre_x;
    float centre_y;
    float radius_x;
    float radius_y;
} AABB;

extern float deltaTime;

extern bool overlaps(AABB, AABB);
extern void updatePhysic(Entity entity);