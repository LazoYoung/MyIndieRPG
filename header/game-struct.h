#ifndef GAME_STRUCT_INCLUDED
#define GAME_STRUCT_INCLUDED
#define INVENTORY_CAP 10

#include <stdbool.h>

typedef float Vector[2];

typedef enum Tile { AIR, BLOCK, TRAIL, PORTAL_1, PORTAL_2, PORTAL_3, PORTAL_4, PORTAL_5 } Tile;
typedef enum Color { DEFAULT, RED, GREEN, BLUE, WHITE, YELLOW, MAGENTA, BLACK, CYAN } Color;
typedef enum Stage { VOID, LOBBY, SHOP, DUNGEON_1, DUNGEON_2, DUNGEON_3 } Stage;
typedef enum ItemCategory { WEAPON, ARMORY, POTION } ItemCategory;
typedef enum ItemType { SMALL_SWORD, BRONZE_SWORD, STEEL_BLADE, HOOD_CAPE, ITEMTYPE_SIZE } ItemType;
typedef enum EntityType { PLAYER, MONSTER } EntityType;
typedef struct GItem {
    ItemCategory category;
    ItemType type;
    bool equip;
    int value;
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
    bool up; // Jump trigger
    bool facing; // Facing right == true
    int leftSpan;
    int rightSpan;
    int attackCooldown;
} Bias;

typedef struct entity {
    const char* name;
    EntityType type;
    Location loc;
    struct entity* target; // Monster's target
    char agility; // Extra speed ratio (%)
    char strength; // Extra damage ratio (%)
    char absorb; // Damage absorbtion
    float health;
    float max_health;
    char mp; // Mana point
    char damage; // Amount of damage dealt without a weapon
    Texture skin;
    AABB hitbox;
    Vector offset;
    Bias bias; // Movement bias
    void (* deathEvent)(struct entity*);
} Entity;

typedef struct {
    char max_health;
    char max_mp;
    char agility; // Extra speed ratio (%)
    char strength; // Extra damage ratio (%)
    unsigned int level;
    unsigned long long exp; // Level-up cost: sqrt(level) * 100
    const char* name; // Character name
} PlayerProperty;

typedef struct {
    GItem *items[INVENTORY_CAP];
    GItem *equipment[3]; // Indexed upon ItemCategory
    long long skills; // Achieved skills are recorded as codes in bit-manner.
    int coin;
} Inventory;

#endif