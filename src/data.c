#include <stdio.h>
#include <stdlib.h>
#include "header/data.h"
#include "header/game-struct.h"

PlayerType playerType;
int playerData[PT_SIZE][PA_SIZE];
int monsterData[MT_SIZE][MA_SIZE];
int itemData[IT_SIZE][IA_SIZE];
int dungeonData[DT_SIZE][DA_SIZE];

void initData() {
    playerData[KIRITO][P_MAX_HEALTH] = 100;
    playerData[KIRITO][P_MAX_MP] = 100;
    playerData[KIRITO][P_AGI] = 30;
    playerData[KIRITO][P_STR] = 30;
    playerData[KIRITO][P_LEVEL] = 1;
    playerData[KIRITO][P_EXP] = 0;
    playerData[ASUNA][P_MAX_HEALTH] = 100;
    playerData[ASUNA][P_MAX_MP] = 100;
    playerData[ASUNA][P_AGI] = 50;
    playerData[ASUNA][P_STR] = 20;
    playerData[ASUNA][P_LEVEL] = 1;
    playerData[ASUNA][P_EXP] = 0;
    playerData[KLEIN][P_MAX_HEALTH] = 100;
    playerData[KLEIN][P_MAX_MP] = 100;
    playerData[KLEIN][P_AGI] = 30;
    playerData[KLEIN][P_STR] = 30;
    playerData[KLEIN][P_LEVEL] = 1;
    playerData[KLEIN][P_EXP] = 0;
    playerData[AGIL][P_MAX_HEALTH] = 100;
    playerData[AGIL][P_MAX_MP] = 100;
    playerData[AGIL][P_AGI] = 20;
    playerData[AGIL][P_STR] = 50;
    playerData[AGIL][P_LEVEL] = 1;
    playerData[AGIL][P_EXP] = 0;
    monsterData[GOLEM][M_MAX_HEALTH] = 50;
    monsterData[GOLEM][M_AGI] = -50;
    monsterData[GOLEM][M_ABSORB] = 0;
    monsterData[GOLEM][M_DAMAGE] = 5;
    monsterData[GOLEM][M_COLOR] = GREEN;
    monsterData[GOLEM][M_VOLUME] = 3;
    monsterData[DWARF][M_MAX_HEALTH] = 10;
    monsterData[DWARF][M_AGI] = 0;
    monsterData[DWARF][M_ABSORB] = 2;
    monsterData[DWARF][M_DAMAGE] = 2;
    monsterData[DWARF][M_COLOR] = BLUE;
    monsterData[DWARF][M_VOLUME] = 1;
    monsterData[GOBLIN][M_MAX_HEALTH] = 100;
    monsterData[GOBLIN][M_AGI] = -20;
    monsterData[GOBLIN][M_ABSORB] = 0;
    monsterData[GOBLIN][M_DAMAGE] = 10;
    monsterData[GOBLIN][M_COLOR] = MAGENTA;
    monsterData[GOBLIN][M_VOLUME] = 2;
    monsterData[GARGOYLE][M_MAX_HEALTH] = 200;
    monsterData[GARGOYLE][M_AGI] = 10;
    monsterData[GARGOYLE][M_ABSORB] = 5;
    monsterData[GARGOYLE][M_DAMAGE] = 20;
    monsterData[GARGOYLE][M_COLOR] = YELLOW;
    monsterData[GARGOYLE][M_VOLUME] = 2;
    monsterData[GHOST][M_MAX_HEALTH] = 50;
    monsterData[GHOST][M_AGI] = 80;
    monsterData[GHOST][M_ABSORB] = 10;
    monsterData[GHOST][M_DAMAGE] = 1;
    monsterData[GHOST][M_COLOR] = WHITE;
    monsterData[GHOST][M_VOLUME] = 1;
    monsterData[LUCIFER][M_MAX_HEALTH] = 300;
    monsterData[LUCIFER][M_AGI] = 20;
    monsterData[LUCIFER][M_ABSORB] = 5;
    monsterData[LUCIFER][M_DAMAGE] = 30;
    monsterData[LUCIFER][M_COLOR] = BLUE;
    monsterData[LUCIFER][M_VOLUME] = 1;
    monsterData[SUCCUBUS][M_MAX_HEALTH] = 350;
    monsterData[SUCCUBUS][M_AGI] = -20;
    monsterData[SUCCUBUS][M_ABSORB] = 0;
    monsterData[SUCCUBUS][M_DAMAGE] = 50;
    monsterData[SUCCUBUS][M_COLOR] = RED;
    monsterData[SUCCUBUS][M_VOLUME] = 2;
    monsterData[DULLAHAN][M_MAX_HEALTH] = 400;
    monsterData[DULLAHAN][M_AGI] = 0;
    monsterData[DULLAHAN][M_ABSORB] = 5;
    monsterData[DULLAHAN][M_DAMAGE] = 50;
    monsterData[DULLAHAN][M_COLOR] = RED;
    monsterData[DULLAHAN][M_VOLUME] = 1;
    monsterData[SKELETON][M_MAX_HEALTH] = 500;
    monsterData[SKELETON][M_AGI] = -50;
    monsterData[SKELETON][M_ABSORB] = 0;
    monsterData[SKELETON][M_DAMAGE] = 5;
    monsterData[SKELETON][M_COLOR] = WHITE;
    monsterData[SKELETON][M_VOLUME] = 2;
    monsterData[TITAN][M_MAX_HEALTH] = 1000;
    monsterData[TITAN][M_AGI] = 60;
    monsterData[TITAN][M_ABSORB] = 10;
    monsterData[TITAN][M_DAMAGE] = 90;
    monsterData[TITAN][M_COLOR] = YELLOW;
    monsterData[TITAN][M_VOLUME] = 3;
    itemData[SMALL_SWORD][I_CATEGORY] = WEAPON;
    itemData[SMALL_SWORD][I_VALUE] = 5;
    itemData[SMALL_SWORD][I_ASSET] = 1;
    itemData[BRONZE_SWORD][I_CATEGORY] = WEAPON;
    itemData[BRONZE_SWORD][I_VALUE] = 10;
    itemData[BRONZE_SWORD][I_ASSET] = 0;
    itemData[STEEL_BLADE][I_CATEGORY] = WEAPON;
    itemData[STEEL_BLADE][I_VALUE] = 15;
    itemData[STEEL_BLADE][I_ASSET] = 0;
    itemData[ANNEAL_BLADE][I_CATEGORY] = WEAPON;
    itemData[ANNEAL_BLADE][I_VALUE] = 20;
    itemData[ANNEAL_BLADE][I_ASSET] = 0;
    itemData[TYRANT_DRAGON][I_CATEGORY] = WEAPON;
    itemData[TYRANT_DRAGON][I_VALUE] = 30;
    itemData[TYRANT_DRAGON][I_ASSET] = 0;
    itemData[ELUCIDATOR][I_CATEGORY] = WEAPON;
    itemData[ELUCIDATOR][I_VALUE] = 50;
    itemData[ELUCIDATOR][I_ASSET] = 0;
    itemData[HOOD_CAPE][I_CATEGORY] = ARMORY;
    itemData[HOOD_CAPE][I_VALUE] = 1;
    itemData[HOOD_CAPE][I_ASSET] = 1;
    itemData[COAT_OF_MIDNIGHT][I_CATEGORY] = ARMORY;
    itemData[COAT_OF_MIDNIGHT][I_VALUE] = 5;
    itemData[COAT_OF_MIDNIGHT][I_ASSET] = 0;
    itemData[HEAL_CRYSTAL][I_CATEGORY] = POTION;
    itemData[HEAL_CRYSTAL][I_VALUE] = 50;
    itemData[HEAL_CRYSTAL][I_ASSET] = 0;
    itemData[MANA_CRYSTAL][I_CATEGORY] = POTION;
    itemData[MANA_CRYSTAL][I_VALUE] = 50;
    itemData[MANA_CRYSTAL][I_ASSET] = 0;
    dungeonData[DUNGEON_1][D_MONSTER_1] = GOLEM;
    dungeonData[DUNGEON_1][D_MONSTER_2] = DWARF;
    dungeonData[DUNGEON_1][D_MONSTER_3] = DWARF;
    dungeonData[DUNGEON_1][D_REWARD_1] = BRONZE_SWORD;
    dungeonData[DUNGEON_1][D_REWARD_2] = COAT_OF_MIDNIGHT;
    dungeonData[DUNGEON_1][D_REWARD_3] = -1;
    dungeonData[DUNGEON_1][D_EXP] = 200;
    dungeonData[DUNGEON_2][D_MONSTER_1] = GOBLIN;
    dungeonData[DUNGEON_2][D_MONSTER_2] = GOLEM;
    dungeonData[DUNGEON_2][D_MONSTER_3] = -1;
    dungeonData[DUNGEON_2][D_REWARD_1] = STEEL_BLADE;
    dungeonData[DUNGEON_2][D_REWARD_2] = HEAL_CRYSTAL;
    dungeonData[DUNGEON_2][D_REWARD_3] = MANA_CRYSTAL;
    dungeonData[DUNGEON_2][D_EXP] = 250;
    dungeonData[DUNGEON_3][D_MONSTER_1] = GARGOYLE;
    dungeonData[DUNGEON_3][D_MONSTER_2] = GHOST;
    dungeonData[DUNGEON_3][D_MONSTER_3] = GOBLIN;
    dungeonData[DUNGEON_3][D_REWARD_1] = ANNEAL_BLADE;
    dungeonData[DUNGEON_3][D_REWARD_2] = HEAL_CRYSTAL;
    dungeonData[DUNGEON_3][D_REWARD_3] = MANA_CRYSTAL;
    dungeonData[DUNGEON_3][D_EXP] = 300;
    dungeonData[DUNGEON_4][D_MONSTER_1] = LUCIFER;
    dungeonData[DUNGEON_4][D_MONSTER_2] = SUCCUBUS;
    dungeonData[DUNGEON_4][D_MONSTER_3] = DULLAHAN;
    dungeonData[DUNGEON_4][D_REWARD_1] = TYRANT_DRAGON;
    dungeonData[DUNGEON_4][D_REWARD_2] = HEAL_CRYSTAL;
    dungeonData[DUNGEON_4][D_REWARD_3] = MANA_CRYSTAL;
    dungeonData[DUNGEON_4][D_EXP] = 350;
    dungeonData[DUNGEON_5][D_MONSTER_1] = SKELETON;
    dungeonData[DUNGEON_5][D_MONSTER_2] = SKELETON;
    dungeonData[DUNGEON_5][D_MONSTER_3] = TITAN;
    dungeonData[DUNGEON_5][D_REWARD_1] = ELUCIDATOR;
    dungeonData[DUNGEON_5][D_REWARD_2] = HEAL_CRYSTAL;
    dungeonData[DUNGEON_5][D_REWARD_3] = MANA_CRYSTAL;
    dungeonData[DUNGEON_5][D_EXP] = 400;
}

char* getPlayerName(int type) {
    switch (type) {
        case KIRITO:
            return "Kirito";
        case ASUNA:
            return "Asuna";
        case KLEIN:
            return "Klein";
        case AGIL:
            return "Agil";
        default:
            return NULL;
    }
}

char* getMonsterName(int type) {
    switch (type) {
        case GOLEM:
            return "Golem";
        case DWARF:
            return "Dwarf";
        case GOBLIN:
            return "Goblin";
        case GARGOYLE:
            return "Gargoyle";
        case GHOST:
            return "Ghost";
        case LUCIFER:
            return "Lucifer";
        case SUCCUBUS:
            return "Succubus";
        case DULLAHAN:
            return "Dullahan";
        case SKELETON:
            return "Skeleton";
        case TITAN:
            return "Titan";
        default:
            return NULL;
    }
}

char* getItemName(int type) {
    switch (type) {
        case SMALL_SWORD:
            return "Small Sword";
        case BRONZE_SWORD:
            return "Bronze Sword";
        case STEEL_BLADE:
            return "Steel Blade";
        case ANNEAL_BLADE:
            return "Anneal Blade";
        case TYRANT_DRAGON:
            return "Tyrant Dragon";
        case ELUCIDATOR:
            return "Elucidator";
        case HOOD_CAPE:
            return "Hood Cape";
        case COAT_OF_MIDNIGHT:
            return "Coat of Midnight";
        case HEAL_CRYSTAL:
            return "Heal Crystal";
        case MANA_CRYSTAL:
            return "Mana Crystal";
        default:
            return NULL;
    }
}

char* getDungeonName(int type) {
    switch (type) {
        case DUNGEON_1:
            return "Dungeon 1";
        case DUNGEON_2:
            return "Dungeon 2";
        case DUNGEON_3:
            return "Dungeon 3";
        case DUNGEON_4:
            return "Dungeon 4";
        case DUNGEON_5:
            return "Dungeon 5";
    }

    return NULL;
}

char* getItemCategoryName(int type) {
    switch (type) {
        case WEAPON:
            return "Weapon";
        case ARMORY:
            return "Armory";
        case POTION:
            return "Potion";
    }

    return NULL;
}

char* intToString(int i) {
    char* str;
    str = malloc(sizeof(int));
    sprintf(str, "%d", i);

    return str;
}