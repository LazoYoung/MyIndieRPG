#include "header/level.h"

void generateLobby() {
    Portal shop = {true, PORTAL_1, SHOP, BLUE};
    Portal dun_test = {true, PORTAL_2, DUNGEON_TEST, MAGENTA};

    assignPortal(shop);
    assignPortal(dun_test);
    setTileAt(19, 6, PORTAL_1);
    setTileAt(20, 6, PORTAL_1);
    setTileAt(21, 6, PORTAL_1);
    setTileAt(39, 6, PORTAL_2);
    setTileAt(40, 6, PORTAL_2);
    setTileAt(41, 6, PORTAL_2);
}