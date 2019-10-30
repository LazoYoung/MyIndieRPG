#ifndef FILE_LEVEL_INCLUDED
#define FILE_LEVEL_INCLUDED
#define MAX_ENTITY 3

#include <stdbool.h>
#include "game-struct.h"

extern int level_width, level_height;

extern void spawnEntity(Entity*);
extern bool despawnEntity(const char*);
extern Entity* getEntity(const char*);
extern Entity* getEntityByID(int);
extern Location getTopLocation(int);
extern void assignPortal(Portal);
extern Portal* getPortal(Tile);
extern void setStage(Stage);
extern void destructLevel();
extern Stage getStage();
extern const char* getStageName(Stage);
extern Tile getTileAt(int, int);

// lobby.c
extern void generateLobby(Tile**);

// dungeon.c
extern void generateDungeon(Tile**);
extern void destructDungeon();

#endif