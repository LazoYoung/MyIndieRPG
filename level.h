#include <stdbool.h>
#include "physic.h"

#define TILE_AIR 0
#define TILE_BLOCK 1

extern char** tiles;
extern int level_width, level_height;

extern void spawnEntity(const char*, Location);
extern int despawnEntity(const char*);
extern Entity getEntity(const char*);
extern Location getSpawnLocation(int x_pos);
extern void updateEntities();
extern void generateLevel();
extern void destructLevel();