#include <stdbool.h>
#include "physic.h"

#define TILE_AIR 0
#define TILE_BLOCK 1

#define STAGE_LOBBY 0
#define STAGE_SHOP 1
#define STAGE_DUNGEON_TEST 2

extern int level_width, level_height;

extern void spawnEntity(Entity*);
extern int despawnEntity(const char*);
extern Entity getEntity(const char*);
extern Location getSpawnLocation(int x_pos);
extern void updateEntities();
extern void generateLevel();
extern void destructLevel();
char getTileAt(int x, int y);