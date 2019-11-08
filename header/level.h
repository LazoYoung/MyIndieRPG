#ifndef FILE_LEVEL_INCLUDED
#define FILE_LEVEL_INCLUDED
#define MAX_ENTITY 4

#include <stdbool.h>
#include "game-struct.h"
#include "data.h"

extern int level_width, level_height;
extern DungeonType dungeon;

void spawnEntity(Entity*);
bool despawnEntity(int id);
Entity* getEntity(const char*);
Entity* getEntityByID(int);
Location getTopLocation(int);
void assignPortal(Portal);
Portal* getPortal(Tile);
void generateLevel(Stage);
void destructLevel();
Stage getStage();
char* getStageName(Stage);
Tile getTileAt(int, int);
void setTileAt(int x, int y, Tile tile);

// lobby.c
void generateLobby();

// dungeon.c
void generateDungeon();
void destructDungeon();

#endif