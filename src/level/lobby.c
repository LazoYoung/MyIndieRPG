#include <stdlib.h>
#include "header/level.h"
#include "header/data.h"

void generateLobby() {
    Portal d1 = {true, PORTAL_1, GREEN, DUNGEON, DUNGEON_1};
    Portal d2 = {true, PORTAL_2, BLUE, DUNGEON, DUNGEON_2};
    Portal d3 = {true, PORTAL_3, MAGENTA, DUNGEON, DUNGEON_3};
    Portal d4 = {true, PORTAL_4, YELLOW, DUNGEON, DUNGEON_4};
    Portal d5 = {true, PORTAL_5, RED, DUNGEON, DUNGEON_5};

    assignPortal(d1);
    assignPortal(d2);
    assignPortal(d3);
    assignPortal(d4);
    assignPortal(d5);
    setTileAt(39, 6, PORTAL_1);
    setTileAt(40, 6, PORTAL_1);
    setTileAt(41, 6, PORTAL_1);
    setTileAt(59, 6, PORTAL_2);
    setTileAt(60, 6, PORTAL_2);
    setTileAt(61, 6, PORTAL_2);
    setTileAt(79, 6, PORTAL_3);
    setTileAt(80, 6, PORTAL_3);
    setTileAt(81, 6, PORTAL_3);
    setTileAt(99, 6, PORTAL_4);
    setTileAt(100, 6, PORTAL_4);
    setTileAt(101, 6, PORTAL_4);
    setTileAt(119, 6, PORTAL_5);
    setTileAt(120, 6, PORTAL_5);
    setTileAt(121, 6, PORTAL_5);
}