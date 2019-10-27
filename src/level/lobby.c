#include "header/level.h"

void generateLobby(Tile **tiles) {
    Portal shop = {true, PORTAL_1, SHOP, BLUE};
    Portal dun_test = {true, PORTAL_2, DUNGEON_TEST, MAGENTA};

    assignPortal(shop);
    assignPortal(dun_test);

    tiles[6][19] = tiles[6][20] = tiles[6][21] = PORTAL_1;
    tiles[6][30] = PORTAL_2;
}