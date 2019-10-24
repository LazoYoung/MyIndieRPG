#include <math.h>
#include <stdlib.h>
#include <ncursesw/curses.h>
#include "game.h"
#include "physic.h"
#include "vector.h"

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

    // TODO Handle ground conllision
    

    // TODO bias data is corrupted. always true
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
        l->spd[1] -= 10.0 * deltaTime;
    }

    // TODO Handle entity, wall collision
}

bool hasGround(Entity entity, float ground_y) {
    Vector centre, lBottom;
    addVector(entity.loc.pos, entity.offset, &centre);
    subVector(centre, entity.hitbox.radius, &lBottom);
}