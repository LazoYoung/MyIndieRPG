#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <ncursesw/curses.h>
#include "game.h"
#include "physic.h"

bool overlaps(AABB a, AABB b) {
    if (abs(a.centre_x - b.centre_x) > a.radius_x + b.radius_x)
        return false;
    if (abs(a.centre_y - b.centre_y) > a.radius_y + b.radius_y)
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
    l->last_pos_x = l->pos_x;
    l->last_pos_y = l->pos_y;
    l->last_spd_x = l->spd_x;
    l->last_spd_y = l->spd_y;
    l->wasOnGround = l->onGround;
    l->pos_x += l->spd_x * deltaTime;
    l->pos_y += l->spd_y * deltaTime;

    // Update hitbox
    hitbox->centre_x = l->pos_x;
    hitbox->centre_y = l->pos_y + e->offset_y;

    // TODO Handle ground conllision
    

    // Handle controls and gravity
    if (ctrl.left) {
        l->spd_x = -5.0;
    }
    else if (ctrl.right) {
        l->spd_x = 5.0;
    }
    else {
        l->spd_x = 0.0;
    }
    if (ctrl.up & l->onGround) {
        l->spd_y = 5.0;
    }
    if (!l->onGround) {
        l->spd_y -= 2.0 * deltaTime;
    }

    // TODO Handle entity, wall collision
}