#include <stdio.h>
#include <stdlib.h>
#include "header/data.h"
#include "header/game.h"
#include "header/screen.h"

PlayerType playerType;

// These arrays below store progress data in Integer. (Not structure)

int playerData[PT_SIZE][PA_SIZE];
int monsterData[MT_SIZE][MA_SIZE];
int itemData[IT_SIZE][IA_SIZE];
int dungeonData[DT_SIZE][DA_SIZE];

const char* dataFileName = "data.txt";

static void saveInventory();

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

int loadData() {
    FILE* file = fopen(dataFileName, "rb");
    int* arr;
    int row_size[5], column_size[5];

    if (!file) {        
        initData();
        return saveData();
    }

    row_size[0] = PT_SIZE;
    row_size[1] = MT_SIZE;
    row_size[2] = IT_SIZE;
    row_size[3] = DT_SIZE;
    row_size[4] = 1;
    column_size[0] = PA_SIZE;
    column_size[1] = MA_SIZE;
    column_size[2] = IA_SIZE;
    column_size[3] = DA_SIZE;
    column_size[4] = 2;

    for (int i = 0; i < 5; i++) {
        for (int r = 0; r < row_size[i]; r++) {
            if (feof(file)) {
                return EOF;
            }

            arr = malloc(column_size[i] * sizeof(int));
            fread(arr, sizeof(int), column_size[i], file);

            switch (i) {
                case 0:
                    for (int c = 0; c < column_size[i]; c++) {
                        playerData[r][c] = *(arr + c);
                    }
                    break;
                case 1:
                    for (int c = 0; c < column_size[i]; c++) {
                        monsterData[r][c] = *(arr + c);
                    }
                    break;
                case 2:
                    for (int c = 0; c < column_size[i]; c++) {
                        itemData[r][c] = *(arr + c);
                    }
                    break;
                case 3:
                    for (int c = 0; c < column_size[i]; c++) {
                        dungeonData[r][c] = *(arr + c);
                    }
                    break;
                case 4:
                    row = *arr;
                    column = *(arr + 1);
                    break;
            }

            free(arr);
        }
    }

    loadInventory();
    return fclose(file);
}

int saveData() {
    FILE* file;
    int* data[5];
    int row_size[5], column_size[5];
    int resolutionData[1][2] = {{row, column}};
    size_t count_r;

    if (!(file = fopen(dataFileName, "w"))) return EOF;
    if (!(file = freopen(dataFileName, "ab", file))) return EOF;

    saveInventory();
    data[0] = playerData[0];
    data[1] = monsterData[0];
    data[2] = itemData[0];
    data[3] = dungeonData[0];
    data[4] = resolutionData[0];
    row_size[0] = PT_SIZE;
    row_size[1] = MT_SIZE;
    row_size[2] = IT_SIZE;
    row_size[3] = DT_SIZE;
    row_size[4] = 1;
    column_size[0] = PA_SIZE;
    column_size[1] = MA_SIZE;
    column_size[2] = IA_SIZE;
    column_size[3] = DA_SIZE;
    column_size[4] = 2;

    for (int i = 0; i < 5; i++) {
        for (int r = 0; r < row_size[i]; r++) {
            count_r = fwrite(data[i] + r*column_size[i], sizeof(int), column_size[i], file);

            if (column_size[i] > count_r) {
                if (feof(file))
                    return EOF;
                
                return ferror(file);
            }
        }
    }

    return fclose(file);
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

void loadInventory() {
    int slot[IC_SIZE] = {0};

    for (int i = 0; i < IC_SIZE; i++) {
        for (int n = 0; n < SLOT_CAP; n++) {
            inv.items[i][n] = -1;
        }

        inv.equipment[i] = -1;
    }

    for (int item = 0; item < IT_SIZE; item++) {
        if (itemData[item][I_ASSET]) {
            int category = itemData[item][I_CATEGORY];
            inv.items[category][slot[category]++] = item;

            if (itemData[item][I_EQUIP]) {
                inv.equipment[category] = item;
            }
        }
    }
}

static void saveInventory() {
    for (int c = 0; c < IC_SIZE; c++) {
        int equip = inv.equipment[c];

        for (int i = 0; i < SLOT_CAP; i++) {
            int item = inv.items[c][i];

            if (item > -1) {
                itemData[item][I_ASSET] = 1;
            }
        }

        if (equip > -1) {
            itemData[equip][I_EQUIP] = 1;
        }
    }
}