#ifndef GAME_STRUCT_INCLUDED
#define GAME_STRUCT_INCLUDED

#define SLOT_CAP 10

#include <stdbool.h>
#include "data.h"

typedef float Vector[2];

typedef enum Tile { AIR, BLOCK, TRAIL, PORTAL_1, PORTAL_2, PORTAL_3, PORTAL_4, PORTAL_5 } Tile;
typedef enum Color { DEFAULT, RED, GREEN, BLUE, WHITE, YELLOW, MAGENTA, BLACK, CYAN } Color;
typedef enum Stage { VOID, LOBBY, SHOP, DUNGEON } Stage;
typedef enum EntityType { PLAYER, MONSTER } EntityType;

typedef struct portal_str {
    Tile tile;
    Color color;
    Stage dest;
    DungeonType dungeon;
    struct portal_str *prev, *next;
} Portal;

typedef struct {
    bool map[9][9];
    int color;
} Texture;

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

typedef struct { // Present bias of entity movement
    bool up; // Jump trigger
    bool facing; // Facing right == true
    int leftSpan;
    int rightSpan;
    int attackCooldown;
} Bias;

typedef struct entity_str {
    const char* name;
    int type[2]; // EntityType-Type (i.e. PLAYER-ASUNA)
    int id; // Unique ID that is automatically assigned
    Location loc;
    struct entity_str* target; // Monster's target
    float health;
    int mp; // Player's mana point
    int damage; // Amount of damage dealt without a weapon
    Texture skin;
    AABB hitbox;
    Vector offset;
    Bias bias; // Movement bias
    void (* deathEvent)(struct entity_str*);
    struct entity_str *prev, *next;
} Entity;

typedef struct {
    int items[IC_SIZE][SLOT_CAP]; // ItemType storage. ItemCategory-Slot
    int equipment[IC_SIZE]; // ItemCategory-ItemType
    long long skills; // Achieved skills are recorded as codes in bit-manner.
    int coin;
} Inventory;

#endif