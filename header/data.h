#ifndef DATA_STRUCT_INCLUDED
#define DATA_STRUCT_INCLUDED

typedef enum PlayerType {
    KIRITO, ASUNA, AGIL, KLEIN,
    PT_SIZE
} PlayerType;
typedef enum PlayerAttribute {
    P_MAX_HEALTH, P_MAX_MP, P_AGI, P_STR, P_LEVEL, P_EXP,
    PA_SIZE
} PlayerAttribute;

typedef enum ItemCategory {
    WEAPON, ARMORY, POTION,
    IC_SIZE
} ItemCategory;
typedef enum ItemType {
    SMALL_SWORD, BRONZE_SWORD, STEEL_BLADE, HOOD_CAPE,
    IT_SIZE
} ItemType;
typedef enum ItemAttribute {
    I_CATEGORY, I_VALUE,
    IA_SIZE
} ItemAttribute;

typedef enum DungeonType {
    DUNGEON_1, DUNGEON_2, DUNGEON_3,
    DUN_SIZE
} DungeonType;

typedef enum MonsterType {
    GOLEM,
    MT_SIZE
} MonsterType;
typedef enum MonsterAttribute {
    M_MAX_HEALTH, M_AGI, M_ABSORB, M_DAMAGE,
    MA_SIZE
} MonsterAttribute;

extern PlayerType playerType;
extern int playerAttr[PT_SIZE][PA_SIZE]; // PlayerType-PlayerAttribute
extern int monsterAttr[MT_SIZE][MA_SIZE]; // MonsterType-MonsterAttribute
extern int itemAttr[IT_SIZE][IA_SIZE]; // ItemType-ItemAttribute

extern void initData();
extern char* getPlayerName(PlayerType type);
extern char* getMonsterName(MonsterType type);
extern char* getItemName(ItemType type);
extern char* intToString(int i);

#endif