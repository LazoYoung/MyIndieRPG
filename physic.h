#include "vector.h"

typedef struct { // Axis Aligned Bounding Box
    Vector centre;
    Vector radius;
} AABB;

typedef struct {
    Vector last_pos;
    Vector last_spd;
    Vector pos;
    Vector spd;
    bool onGround;
    bool wasOnGround;
} Location;

typedef struct { // Present inputs and attributes for moving an entity
    bool left; // Go left
    bool right; // Go right
    bool up; // Toggle jump or fly
    int times; // How many times does the present control carry on
} Control;

typedef struct entity {
    bool valid;
    const char* name;
    Location loc;
    Control ctrl;
    AABB hitbox;
    Vector offset;
    struct entity* next;
} Entity;

extern bool overlaps(AABB, AABB);
extern void updateControl(int, Control* ctrl);
extern void updatePhysic(Entity* entity);