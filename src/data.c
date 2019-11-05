#include <stdio.h>
#include <stdlib.h>
#include "header/data.h"
#include "header/game-struct.h"

PlayerType playerType;
int playerAttr[PT_SIZE][PA_SIZE];
int monsterAttr[MT_SIZE][MA_SIZE];
int itemAttr[IT_SIZE][IA_SIZE];
int dungeonAttr[DT_SIZE][DA_SIZE];

void initData() {
    playerAttr[KIRITO][P_MAX_HEALTH] = 100;
    playerAttr[KIRITO][P_MAX_MP] = 100;
    playerAttr[KIRITO][P_AGI] = 30;
    playerAttr[KIRITO][P_STR] = 30;
    playerAttr[KIRITO][P_LEVEL] = 1;
    playerAttr[KIRITO][P_EXP] = 0;
    playerAttr[ASUNA][P_MAX_HEALTH] = 100;
    playerAttr[ASUNA][P_MAX_MP] = 100;
    playerAttr[ASUNA][P_AGI] = 50;
    playerAttr[ASUNA][P_STR] = 20;
    playerAttr[ASUNA][P_LEVEL] = 1;
    playerAttr[ASUNA][P_EXP] = 0;
    playerAttr[KLEIN][P_MAX_HEALTH] = 100;
    playerAttr[KLEIN][P_MAX_MP] = 100;
    playerAttr[KLEIN][P_AGI] = 30;
    playerAttr[KLEIN][P_STR] = 30;
    playerAttr[KLEIN][P_LEVEL] = 1;
    playerAttr[KLEIN][P_EXP] = 0;
    playerAttr[AGIL][P_MAX_HEALTH] = 100;
    playerAttr[AGIL][P_MAX_MP] = 100;
    playerAttr[AGIL][P_AGI] = 20;
    playerAttr[AGIL][P_STR] = 50;
    playerAttr[AGIL][P_LEVEL] = 1;
    playerAttr[AGIL][P_EXP] = 0;
    monsterAttr[GOLEM][M_MAX_HEALTH] = 50;
    monsterAttr[GOLEM][M_AGI] = -50;
    monsterAttr[GOLEM][M_ABSORB] = 0;
    monsterAttr[GOLEM][M_DAMAGE] = 5;
    monsterAttr[GOLEM][M_COLOR] = GREEN;
    monsterAttr[GOLEM][M_VOLUME] = 3;
    monsterAttr[DWARF][M_MAX_HEALTH] = 10;
    monsterAttr[DWARF][M_AGI] = 0;
    monsterAttr[DWARF][M_ABSORB] = 2;
    monsterAttr[DWARF][M_DAMAGE] = 2;
    monsterAttr[DWARF][M_COLOR] = BLUE;
    monsterAttr[DWARF][M_VOLUME] = 1;
    monsterAttr[GOBLIN][M_MAX_HEALTH] = 100;
    monsterAttr[GOBLIN][M_AGI] = -20;
    monsterAttr[GOBLIN][M_ABSORB] = 0;
    monsterAttr[GOBLIN][M_DAMAGE] = 10;
    monsterAttr[GOBLIN][M_COLOR] = MAGENTA;
    monsterAttr[GOBLIN][M_VOLUME] = 2;
    monsterAttr[GARGOYLE][M_MAX_HEALTH] = 200;
    monsterAttr[GARGOYLE][M_AGI] = 10;
    monsterAttr[GARGOYLE][M_ABSORB] = 5;
    monsterAttr[GARGOYLE][M_DAMAGE] = 20;
    monsterAttr[GARGOYLE][M_COLOR] = YELLOW;
    monsterAttr[GARGOYLE][M_VOLUME] = 2;
    monsterAttr[GHOST][M_MAX_HEALTH] = 50;
    monsterAttr[GHOST][M_AGI] = 80;
    monsterAttr[GHOST][M_ABSORB] = 10;
    monsterAttr[GHOST][M_DAMAGE] = 1;
    monsterAttr[GHOST][M_COLOR] = WHITE;
    monsterAttr[GHOST][M_VOLUME] = 1;
    monsterAttr[LUCIFER][M_MAX_HEALTH] = 300;
    monsterAttr[LUCIFER][M_AGI] = 20;
    monsterAttr[LUCIFER][M_ABSORB] = 5;
    monsterAttr[LUCIFER][M_DAMAGE] = 30;
    monsterAttr[LUCIFER][M_COLOR] = BLUE;
    monsterAttr[LUCIFER][M_VOLUME] = 1;
    monsterAttr[SUCCUBUS][M_MAX_HEALTH] = 350;
    monsterAttr[SUCCUBUS][M_AGI] = -20;
    monsterAttr[SUCCUBUS][M_ABSORB] = 0;
    monsterAttr[SUCCUBUS][M_DAMAGE] = 50;
    monsterAttr[SUCCUBUS][M_COLOR] = RED;
    monsterAttr[SUCCUBUS][M_VOLUME] = 2;
    monsterAttr[DULLAHAN][M_MAX_HEALTH] = 400;
    monsterAttr[DULLAHAN][M_AGI] = 0;
    monsterAttr[DULLAHAN][M_ABSORB] = 5;
    monsterAttr[DULLAHAN][M_DAMAGE] = 50;
    monsterAttr[DULLAHAN][M_COLOR] = RED;
    monsterAttr[DULLAHAN][M_VOLUME] = 1;
    monsterAttr[SKELETON][M_MAX_HEALTH] = 500;
    monsterAttr[SKELETON][M_AGI] = -50;
    monsterAttr[SKELETON][M_ABSORB] = 0;
    monsterAttr[SKELETON][M_DAMAGE] = 5;
    monsterAttr[SKELETON][M_COLOR] = WHITE;
    monsterAttr[SKELETON][M_VOLUME] = 2;
    monsterAttr[TITAN][M_MAX_HEALTH] = 1000;
    monsterAttr[TITAN][M_AGI] = 60;
    monsterAttr[TITAN][M_ABSORB] = 10;
    monsterAttr[TITAN][M_DAMAGE] = 90;
    monsterAttr[TITAN][M_COLOR] = YELLOW;
    monsterAttr[TITAN][M_VOLUME] = 3;
    itemAttr[SMALL_SWORD][I_CATEGORY] = WEAPON;
    itemAttr[SMALL_SWORD][I_VALUE] = 5;
    itemAttr[SMALL_SWORD][I_ASSET] = 1;
    itemAttr[BRONZE_SWORD][I_CATEGORY] = WEAPON;
    itemAttr[BRONZE_SWORD][I_VALUE] = 10;
    itemAttr[BRONZE_SWORD][I_ASSET] = 0;
    itemAttr[STEEL_BLADE][I_CATEGORY] = WEAPON;
    itemAttr[STEEL_BLADE][I_VALUE] = 15;
    itemAttr[STEEL_BLADE][I_ASSET] = 0;
    itemAttr[ANNEAL_BLADE][I_CATEGORY] = WEAPON;
    itemAttr[ANNEAL_BLADE][I_VALUE] = 20;
    itemAttr[ANNEAL_BLADE][I_ASSET] = 0;
    itemAttr[TYRANT_DRAGON][I_CATEGORY] = WEAPON;
    itemAttr[TYRANT_DRAGON][I_VALUE] = 30;
    itemAttr[TYRANT_DRAGON][I_ASSET] = 0;
    itemAttr[ELUCIDATOR][I_CATEGORY] = WEAPON;
    itemAttr[ELUCIDATOR][I_VALUE] = 50;
    itemAttr[ELUCIDATOR][I_ASSET] = 0;
    itemAttr[HOOD_CAPE][I_CATEGORY] = ARMORY;
    itemAttr[HOOD_CAPE][I_VALUE] = 1;
    itemAttr[HOOD_CAPE][I_ASSET] = 1;
    itemAttr[COAT_OF_MIDNIGHT][I_CATEGORY] = ARMORY;
    itemAttr[COAT_OF_MIDNIGHT][I_VALUE] = 5;
    itemAttr[COAT_OF_MIDNIGHT][I_ASSET] = 0;
    itemAttr[HEAL_CRYSTAL][I_CATEGORY] = POTION;
    itemAttr[HEAL_CRYSTAL][I_VALUE] = 50;
    itemAttr[HEAL_CRYSTAL][I_ASSET] = 0;
    itemAttr[MANA_CRYSTAL][I_CATEGORY] = POTION;
    itemAttr[MANA_CRYSTAL][I_VALUE] = 50;
    itemAttr[MANA_CRYSTAL][I_ASSET] = 0;
    dungeonAttr[DUNGEON_1][D_MONSTER_1] = GOLEM;
    dungeonAttr[DUNGEON_1][D_MONSTER_2] = DWARF;
    dungeonAttr[DUNGEON_1][D_MONSTER_3] = DWARF;
    dungeonAttr[DUNGEON_1][D_REWARD_1] = BRONZE_SWORD;
    dungeonAttr[DUNGEON_1][D_REWARD_2] = COAT_OF_MIDNIGHT;
    dungeonAttr[DUNGEON_1][D_REWARD_3] = -1;
    dungeonAttr[DUNGEON_1][D_EXP] = 200;
    dungeonAttr[DUNGEON_2][D_MONSTER_1] = GOBLIN;
    dungeonAttr[DUNGEON_2][D_MONSTER_2] = GOLEM;
    dungeonAttr[DUNGEON_2][D_MONSTER_3] = -1;
    dungeonAttr[DUNGEON_2][D_REWARD_1] = STEEL_BLADE;
    dungeonAttr[DUNGEON_2][D_REWARD_2] = HEAL_CRYSTAL;
    dungeonAttr[DUNGEON_2][D_REWARD_3] = MANA_CRYSTAL;
    dungeonAttr[DUNGEON_2][D_EXP] = 250;
    dungeonAttr[DUNGEON_3][D_MONSTER_1] = GARGOYLE;
    dungeonAttr[DUNGEON_3][D_MONSTER_2] = GHOST;
    dungeonAttr[DUNGEON_3][D_MONSTER_3] = GOBLIN;
    dungeonAttr[DUNGEON_3][D_REWARD_1] = ANNEAL_BLADE;
    dungeonAttr[DUNGEON_3][D_REWARD_2] = HEAL_CRYSTAL;
    dungeonAttr[DUNGEON_3][D_REWARD_3] = MANA_CRYSTAL;
    dungeonAttr[DUNGEON_3][D_EXP] = 300;
    dungeonAttr[DUNGEON_4][D_MONSTER_1] = LUCIFER;
    dungeonAttr[DUNGEON_4][D_MONSTER_2] = SUCCUBUS;
    dungeonAttr[DUNGEON_4][D_MONSTER_3] = DULLAHAN;
    dungeonAttr[DUNGEON_4][D_REWARD_1] = TYRANT_DRAGON;
    dungeonAttr[DUNGEON_4][D_REWARD_2] = HEAL_CRYSTAL;
    dungeonAttr[DUNGEON_4][D_REWARD_3] = MANA_CRYSTAL;
    dungeonAttr[DUNGEON_4][D_EXP] = 350;
    dungeonAttr[DUNGEON_5][D_MONSTER_1] = SKELETON;
    dungeonAttr[DUNGEON_5][D_MONSTER_2] = SKELETON;
    dungeonAttr[DUNGEON_5][D_MONSTER_3] = TITAN;
    dungeonAttr[DUNGEON_5][D_REWARD_1] = ELUCIDATOR;
    dungeonAttr[DUNGEON_5][D_REWARD_2] = HEAL_CRYSTAL;
    dungeonAttr[DUNGEON_5][D_REWARD_3] = MANA_CRYSTAL;
    dungeonAttr[DUNGEON_5][D_EXP] = 400;
}

char* getPlayerName(PlayerType type) {
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

char* getMonsterName(MonsterType type) {
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

char* getItemName(ItemType type) {
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

char* intToString(int i) {
    char* str;
    str = malloc(sizeof(int));
    sprintf(str, "%d", i);

    return str;
}