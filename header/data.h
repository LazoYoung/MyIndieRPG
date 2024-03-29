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

typedef enum MonsterType {
    GOLEM, DWARF, GOBLIN, GARGOYLE, GHOST, LUCIFER, SUCCUBUS, DULLAHAN, SKELETON, TITAN,
    MT_SIZE
} MonsterType;
typedef enum MonsterAttribute {
    M_MAX_HEALTH, M_AGI, M_ABSORB, M_DAMAGE, M_COLOR, M_VOLUME,
    MA_SIZE
} MonsterAttribute;

typedef enum ItemCategory {
    WEAPON, ARMORY, POTION,
    IC_SIZE
} ItemCategory;
typedef enum ItemType {
    SMALL_SWORD, BRONZE_SWORD, STEEL_BLADE, ANNEAL_BLADE, TYRANT_DRAGON, ELUCIDATOR, HOOD_CAPE, COAT_OF_MIDNIGHT, HEAL_CRYSTAL, MANA_CRYSTAL,
    IT_SIZE
} ItemType;
typedef enum ItemAttribute {
    I_CATEGORY, I_VALUE, I_ASSET, I_EQUIP,
    IA_SIZE
} ItemAttribute;

typedef enum DungeonType {
    DUNGEON_1, DUNGEON_2, DUNGEON_3, DUNGEON_4, DUNGEON_5,
    DT_SIZE
} DungeonType;

/**
 * D_MONSTER_X is MonsterType
 * D_REWARD_X is ItemType
 * These attributes can represent None by negative integer value.
 **/
typedef enum DungeonAttribute {
    D_MONSTER_1, D_MONSTER_2, D_MONSTER_3, D_REWARD_1, D_REWARD_2, D_REWARD_3, D_EXP,
    DA_SIZE
} DungeonAttribute;

extern PlayerType playerType;
extern int playerData[PT_SIZE][PA_SIZE]; // PlayerType-PlayerAttribute
extern int monsterData[MT_SIZE][MA_SIZE]; // MonsterType-MonsterAttribute
extern int itemData[IT_SIZE][IA_SIZE]; // ItemType-ItemAttribute
extern int dungeonData[DT_SIZE][DA_SIZE]; // DungeonType-DungeonAttribute
extern const char* dataFileName;

void initData();
int loadData();
int saveData();
char* getPlayerName(int type);
char* getMonsterName(int type);
char* getItemName(int type);
char* getDungeonName(int type);
char* getItemCategoryName(int type);
char* intToString(int i);
void loadInventory();

#endif