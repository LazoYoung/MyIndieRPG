#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "game.h"
#include "physic.h"

bool overlaps(AABB a, AABB b) {
    if (abs(a.centre_x - b.centre_x) > a.radius_x + b.radius_x)
        return false;
    if (abs(a.centre_y - b.centre_y) > a.radius_y + b.radius_y)
        return false;
    return true;
}

void updatePhysic(Entity* e) {
    if (e == NULL || !e->valid) {
        return;
    }

    Location* l = &e->loc;
    AABB* hitbox = &e->hitbox;
    l->last_pos_x = l->pos_x;
    l->last_pos_y = l->pos_y;
    l->last_spd_x = l->spd_x;
    l->last_spd_y = l->spd_y;
    l->wasOnGround = l->onGround;
    l->pos_x = l->spd_x * deltaTime;

    // TODO Handle ground collisions

    hitbox->centre_x = l->pos_x;
    hitbox->centre_y = l->pos_y + e->offset_y;

    // TODO Implement controls
}