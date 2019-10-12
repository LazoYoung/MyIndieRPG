#include <stdbool.h>
#include "physic.h"

#define TILE_AIR 0
#define TILE_BLOCK 1

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
extern char** tiles;
extern int level_width, level_height;

extern void spawnEntity(const char*, Location);
extern int despawnEntity(const char*);
extern Entity getEntity(const char*);
extern Location getSpawnLocation(int x_pos);
extern void updateEntities();
extern void generateLevel();
extern void destructLevel();