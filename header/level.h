#ifndef FILE_LEVEL_INCLUDED
#define FILE_LEVEL_INCLUDED

#include <stdbool.h>
#include "game-struct.h"

extern int level_width, level_height;

extern void spawnEntity(Entity*);
extern int despawnEntity(const char*);
extern Entity* getEntity(const char*);
extern Location getSpawnLocation();
extern void assignPortal(Portal);
extern Portal* getPortal(Tile);
extern void generateLevel();
extern void destructLevel();
extern void setStage(Stage);
extern Stage getStage();
extern const char* getStageName(Stage);
extern Tile getTileAt(int, int);

// lobby.c
extern void generateLobby(Tile**);

#endif