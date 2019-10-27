#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncursesw/curses.h>
#include "header/game-struct.h"
#include "header/level.h"
#include "header/game.h"
#include "header/physic.h"
#include "header/vector.h"

static void updatePhysic(Entity* e);
static bool hasVerticalObstacle(Location loc, AABB hitbox, Vector offset, float *ground_y);
static bool hasHorizontalObstacle(Location loc, AABB hitbox, Vector offset);

bool overlaps(AABB a, AABB b) {
    if (abs(a.centre[0] - b.centre[0]) > a.radius[0] + b.radius[0])
        return false;
    if (abs(a.centre[1] - b.centre[1]) > a.radius[1] + b.radius[1])
        return false;
    return true;
}

void updateControl(int key, Bias* bias) {
    switch (key) {
        case KEY_RIGHT:
            if (bias->left) {
                bias->left = false;
                bias->times = 0;
            } else {
                bias->right = true;
                bias->times = getFramesDuringTime(1000);
            }
            break;
        case KEY_LEFT:
            if (bias->right) {
                bias->right = false;
                bias->times = 0;
            } else {
                bias->left = true;
                bias->times = getFramesDuringTime(1000);
            }
            break;
        case KEY_UP:
            bias->up = true;
            break;
    }

    if (bias->times >= 0) {
        if (bias->times-- == 0) {
            bias->right = false;
            bias->left = false;
        }
    }
}

void updateEntities() {
    Entity* iter = getEntity(NULL);

    while (iter != NULL && iter->valid) {
        updatePhysic(iter);
        iter = iter->next;
    }
}

static void updatePhysic(Entity* e) {
    if (e == NULL || !e->valid) {
        return;
    }

    static int sleep = 30;
    float ground_y = 0.0;
    Location* l = &e->loc;
    Vector* last_pos = &l->last_pos;
    AABB* hitbox = &e->hitbox;
    Bias* bias = &e->bias;

    // Update movement records
    l->last_pos[0] = l->pos[0];
    l->last_pos[1] = l->pos[1];
    l->last_pos[0] = l->spd[0];
    l->last_pos[1] = l->spd[1];
    l->wasOnGround = l->onGround;
    l->pos[0] += l->spd[0] * deltaTime;
    l->pos[1] += l->spd[1] * deltaTime;

    // Update hitbox position
    hitbox->centre[0] = l->pos[0] + e->offset[0];
    hitbox->centre[1] = l->pos[1] + e->offset[1];

    // Perform vertical collision
    if (l->spd[1] <= 0.0 && hasVerticalObstacle(*l, *hitbox, e->offset, &ground_y)) {
        l->pos[1] = ground_y + hitbox->radius[1] - e->offset[1];
        l->spd[1] = 0.0;
        l->onGround = true;
    } else {
        l->onGround = false;
    }
    
    // Perform horizontal interaction
    if (--sleep == 0) {
        Tile tile = getTileAt((int) l->pos[0], (int) l->pos[1]);
        sleep = 30;

        switch (tile) {
            case PORTAL_1:
            case PORTAL_2:
            case PORTAL_3:
            case PORTAL_4:
            case PORTAL_5:
            {
                Portal *portal = getPortal(tile);

                if (portal != NULL) {
                    setStage(portal->dest);
                    destructLevel();
                    generateLevel();
                }
                break;
            }
        }
    }
    

    // Handle controls and gravity
    if (bias->left) {
        l->spd[0] = -10.0;
    }
    else if (bias->right) {
        l->spd[0] = 10.0;
    }
    else {
        l->spd[0] = 0.0;
    }

    if (bias->up) {
        bias->up = false;
        l->spd[1] = 10.0;
    }

    if (!l->onGround) {
        l->spd[1] -= 15.0 * deltaTime;
    }
}

/**
 * Returns true if entity will collide with the obstacle in vertical direction.
 * If so, ground_y will point to y-coordinate above ground.
 **/
static bool hasVerticalObstacle(Location loc, AABB hitbox, Vector offset, float *ground_y) {
    Vector centre, lBottom, rBottom, tile;

    add(loc.pos, offset, &centre);
    sub(centre, hitbox.radius, &lBottom);
    add_m(lBottom, 0.0, -1.0, &lBottom);
    add_m(lBottom, 2 * hitbox.radius[0], 0.0, &rBottom);
    tile[0] = lBottom[0];
    tile[1] = lBottom[1];

    while (tile[0] <= rBottom[0]) {
        if (getTileAt(tile[0], tile[1]) == BLOCK) {
            *ground_y = tile[1] + 1;
            return true;
        }

        if (tile[0] == rBottom[0]) {
            break;
        }

        tile[0] = rBottom[0] > tile[0] + 1 ? rBottom[0] : tile[0] + 1;
    }
    return false;
}

/**
 * TODO Planned to be implemented
 * Returns true if entity will collide with the obstacle in horizontal direction.
 * If so, ground_y will point to x-coordinate before the contact.
 **/
static bool hasHorizontalObstacle(Location loc, AABB hitbox, Vector offset) {}