#include <stdio.h>
#include <stdlib.h>
#include "header/data.h"

PlayerType playerType;
int playerAttr[PT_SIZE][PA_SIZE];
int monsterAttr[MT_SIZE][MA_SIZE];
int itemAttr[IT_SIZE][IA_SIZE];

void initData() {
    playerAttr[KIRITO][P_MAX_HEALTH] = 100;
    playerAttr[KIRITO][P_MAX_MP] = 100;
    playerAttr[KIRITO][P_AGI] = 30;
    playerAttr[KIRITO][P_STR] = 30;
    playerAttr[KIRITO][P_LEVEL] = 0;
    playerAttr[KIRITO][P_EXP] = 0;
    playerAttr[ASUNA][P_MAX_HEALTH] = 100;
    playerAttr[ASUNA][P_MAX_MP] = 100;
    playerAttr[ASUNA][P_AGI] = 50;
    playerAttr[ASUNA][P_STR] = 20;
    playerAttr[ASUNA][P_LEVEL] = 0;
    playerAttr[ASUNA][P_EXP] = 0;
    playerAttr[KLEIN][P_MAX_HEALTH] = 100;
    playerAttr[KLEIN][P_MAX_MP] = 100;
    playerAttr[KLEIN][P_AGI] = 30;
    playerAttr[KLEIN][P_STR] = 30;
    playerAttr[KLEIN][P_LEVEL] = 0;
    playerAttr[KLEIN][P_EXP] = 0;
    playerAttr[AGIL][P_MAX_HEALTH] = 100;
    playerAttr[AGIL][P_MAX_MP] = 100;
    playerAttr[AGIL][P_AGI] = 20;
    playerAttr[AGIL][P_STR] = 50;
    playerAttr[AGIL][P_LEVEL] = 0;
    playerAttr[AGIL][P_EXP] = 0;
    monsterAttr[GOLEM][M_MAX_HEALTH] = 50;
    monsterAttr[GOLEM][M_AGI] = -50;
    monsterAttr[GOLEM][M_ABSORB] = 0;
    monsterAttr[GOLEM][M_DAMAGE] = 5;
    itemAttr[SMALL_SWORD][I_CATEGORY] = WEAPON;
    itemAttr[SMALL_SWORD][I_VALUE] = 5;
    itemAttr[BRONZE_SWORD][I_CATEGORY] = WEAPON;
    itemAttr[BRONZE_SWORD][I_VALUE] = 10;
    itemAttr[STEEL_BLADE][I_CATEGORY] = WEAPON;
    itemAttr[STEEL_BLADE][I_VALUE] = 15;
    itemAttr[HOOD_CAPE][I_CATEGORY] = ARMORY;
    itemAttr[HOOD_CAPE][I_VALUE] = 1;
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
        case HOOD_CAPE:
            return "Hood Cape";
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