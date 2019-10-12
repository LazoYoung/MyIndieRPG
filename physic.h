typedef struct { // Axis Aligned Bounding Box
    float centre_x;
    float centre_y;
    float radius_x;
    float radius_y;
} AABB;

typedef struct {
    float last_pos_x;
    float last_pos_y;
    float last_spd_x;
    float last_spd_y;
    float pos_x;
    float pos_y;
    float spd_x;
    float spd_y;
    bool onGround;
    bool wasOnGround;
} Location;

typedef struct entity {
    bool valid;
    const char* name;
    Location loc;
    AABB hitbox;
    float offset_y;
    struct entity* next;
} Entity;

extern float deltaTime;

extern bool overlaps(AABB, AABB);
extern void updatePhysic(Entity* entity);