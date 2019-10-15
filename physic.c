#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <ncursesw/curses.h>
#include "game.h"
#include "physic.h"
#include "vector.h"

bool overlaps(AABB a, AABB b) {
    if (abs(a.centre.x - b.centre.x) > a.radius.x + b.radius.x)
        return false;
    if (abs(a.centre.y - b.centre.y) > a.radius.y + b.radius.y)
        return false;
    return true;
}

void updateControl(int key, Control* ctrl) {
    switch (key) {
        case KEY_RIGHT:
            if (ctrl->left) {
                ctrl->left = false;
                ctrl->times = 0;
            }
            ctrl->right = true;
            ctrl->times += getFramesDuringTime(1000);
            break;
        case KEY_LEFT:
            if (ctrl->right) {
                ctrl->right = false;
                ctrl->times = 0;
            }
            ctrl->left = true;
            ctrl->times += getFramesDuringTime(1000);
            break;
        case KEY_DOWN:
            ctrl->left = false;
            ctrl->right = false;
            ctrl->times = 0;
            break;
        case KEY_UP:
            ctrl->up = true;
            ctrl->times += getFramesDuringTime(500);
            break;
    }

    if (ctrl->times > 0) {
        ctrl->times--;
    }
}

void updatePhysic(Entity* e) {
    if (e == NULL || !e->valid) {
        return;
    }

    Location* l = &e->loc;
    AABB* hitbox = &e->hitbox;
    Control ctrl = e->ctrl;

    // Update movement records
    l->last_pos.x = l->pos.x;
    l->last_pos.y = l->pos.y;
    l->last_spd.x = l->spd.x;
    l->last_spd.y = l->spd.y;
    l->wasOnGround = l->onGround;
    l->pos.x += l->spd.x * deltaTime;
    l->pos.y += l->spd.y * deltaTime;

    // Update hitbox
    hitbox->centre.x = l->pos.x + e->offset.x;
    hitbox->centre.y = l->pos.y + e->offset.y;

    // TODO Handle ground conllision
    

    // Handle controls and gravity
    if (ctrl.left) {
        l->spd.x = -5.0;
    }
    else if (ctrl.right) {
        l->spd.x = 5.0;
    }
    else {
        l->spd.x = 0.0;
    }
    if (ctrl.up & l->onGround) {
        l->spd.y = 5.0;
    }
    if (!l->onGround) {
        l->spd.y -= 2.0 * deltaTime;
    }

    // TODO Handle entity, wall collision
}

bool hasGround(Entity entity, float ground_y) {
    Vector centre = addVector(entity.loc.pos, entity.offset);
    Vector lBottom = subVector(centre, entity.hitbox.radius);
}