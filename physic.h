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
    float offset_y;
    struct entity* next;
} Entity;

extern bool overlaps(AABB, AABB);
extern void updateControl(int, Control* ctrl);
extern void updatePhysic(Entity* entity);