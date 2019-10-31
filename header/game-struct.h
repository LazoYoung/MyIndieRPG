#ifndef GAME_STRUCT_INCLUDED
#define GAME_STRUCT_INCLUDED

#include <stdbool.h>

// TODO Introduce Union as vector storage. Returning a pointer involves headache
typedef float Vector[2];

typedef enum Tile { AIR, BLOCK, PORTAL_1, PORTAL_2, PORTAL_3, PORTAL_4, PORTAL_5 } Tile;
typedef enum Color { DEFAULT, RED, GREEN, BLUE, WHITE, YELLOW, MAGENTA, BLACK, CYAN } Color;
typedef enum Stage { VOID, LOBBY, SHOP, DUNGEON_TEST } Stage;
typedef enum ItemCategory { WEAPON, ARMORY, POTION } ItemCategory;
typedef enum ItemType {EMPTY, SMALL_SWORD, BRONZE_SWORD, STEEL_BLADE, HOOD_CAPE} ItemType;
typedef struct GItem {
    bool valid;
    ItemCategory category;
    ItemType type;
    bool equip;
    int value;
    char* id_str;
} GItem;

typedef struct {
    bool valid;
    Tile tile;
    Stage dest;
    Color color;
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
    bool left; // Going left
    bool right; // Going right
    bool up; // Toggling jump or fly
    bool attack; // Attacking others
    int times; // How many times does the present control carry on
} Bias;

typedef struct entity {
    bool valid;
    const char* name;
    Location loc;
    char health; // 0 ~ 100
    char damage; // Amount of damage dealt without a weapon
    Bias bias;
    Texture skin;
    AABB hitbox;
    Vector offset;
    //struct entity* next;
} Entity;

typedef struct {
    char agility; // 1 ~ 100
    char strength; // 1 ~ 100
    char mp; // 1 ~ 100
    unsigned int level;
    unsigned long long exp; // Level-up cost: 200 + (50 * level)
    const char* name; // Character name
} PlayerProperty;

typedef struct {
    GItem items[10];
    GItem *equipment[3]; // Indexed upon ItemCategory
    long long skills; // Achieved skills are recorded as codes in bit-manner.
    int coin;
} Inventory;

#endif