#include <stdbool.h>

#define TILE_AIR 0
#define TILE_BLOCK 1

typedef struct {
    float pos_x;
    float pos_y;
    float vec_x;
    float vec_y;
} Location;
typedef struct entity {
    bool valid;
    const char* name;
    Location loc;
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