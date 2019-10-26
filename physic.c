#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncursesw/curses.h>
#include "level.h"
#include "game.h"
#include "physic.h"
#include "vector.h"

static bool hasGround(Location loc, AABB hitbox, Vector offset, float *ground_y);

bool overlaps(AABB a, AABB b) {
    if (abs(a.centre[0] - b.centre[0]) > a.radius[0] + b.radius[0])
        return false;
    if (abs(a.centre[1] - b.centre[1]) > a.radius[1] + b.radius[1])
        return false;
    return true;
}

void updateControl(int key, Bias* bias) {
    switch (key) {
        case KEY_RIGHT: // This is confirmed to work
            if (bias->left) {
                bias->left = false;
                bias->times = 0;
            }
            bias->right = true;
            bias->times += getFramesDuringTime(1000);
            break;
        case KEY_LEFT:
            if (bias->right) {
                bias->right = false;
                bias->times = 0;
            }
            bias->left = true;
            bias->times += getFramesDuringTime(1000);
            break;
        case KEY_DOWN:
            bias->left = false;
            bias->right = false;
            bias->times = 0;
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

void updatePhysic(Entity* e) {
    if (e == NULL || !e->valid) {
        return;
    }

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

    if (l->spd[1] <= 0.0 && hasGround(*l, *hitbox, e->offset, &ground_y)) {
        l->pos[1] = ground_y + hitbox->radius[1] - e->offset[1];
        l->spd[1] = 0.0;
        l->onGround = true;
    } else {
        l->onGround = false;
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

    if (bias->up && l->onGround) {
        bias->up = false;
        l->spd[1] = 10.0;
    }

    if (!l->onGround) {
        l->spd[1] -= 15.0 * deltaTime;
    }

    // TODO Handle entity, wall collision
}

/**
 * Returns whether the entity is colliding with the ground.
 * If true, ground_y will point to the y-coordinate above ground.
 **/
static bool hasGround(Location loc, AABB hitbox, Vector offset, float *ground_y) {
    Vector *centre, *lBottom, *rBottom, tile;

    centre = add(loc.pos, offset);
    lBottom = sub(*centre, hitbox.radius);
    lBottom = add_m(*lBottom, 0.0, -1.0);
    rBottom = add_m(*lBottom, 2 * hitbox.radius[0], 0.0);
    tile[0] = (*lBottom)[0];
    tile[1] = (*lBottom)[1];

    while (tile[0] < (*rBottom)[0]) {
        if (getTileAt(tile[0], tile[1]) == TILE_BLOCK) {
            *ground_y = tile[1] + 1;
            return true;
        }

        tile[0] = (*rBottom)[0] > tile[0] + 1 ? (*rBottom)[0] : tile[0] + 1;
    }
    return false;
}