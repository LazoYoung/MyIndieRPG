#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncursesw/curses.h>
#include "header/game-struct.h"
#include "header/data.h"
#include "header/level.h"
#include "header/game.h"
#include "header/screen.h"
#include "header/physic.h"
#include "header/vector.h"

struct {
    bool active;
    int length;
    int span;
    int dtg;
    int damage;
    Vector pos;
    Vector norm;
} trail;

static bool hasVerticalObstacle(Location loc, AABB hitbox, Vector offset, float *ground_y);
static bool spawnSwordTrail(Vector pos, Vector norm, int damage);
static void attack(Entity* entity, Entity* victim, float distance);

bool overlaps(AABB a, AABB b) {
    if (abs(a.centre[0] - b.centre[0]) > a.radius[0] + b.radius[0])
        return false;
    if (abs(a.centre[1] - b.centre[1]) > a.radius[1] + b.radius[1])
        return false;
    return true;
}

void updateControl(int key, Entity* p) {
    Bias *bias = &p->bias;
    int potion = inv.equipment[POTION];

    switch (key) {
        case 'd': // Right
            if (bias->leftSpan > 0) {
                bias->leftSpan = 0;
            } else {
                bias->rightSpan = getFramesDuringTime(500);
            }
            bias->facing = true;
            break;
        case 'a': // Left
            if (bias->rightSpan > 0) {
                bias->rightSpan = 0;
            } else {
                bias->leftSpan = getFramesDuringTime(500);
            }
            bias->facing = false;
            break;
        case 's': // Stop
            bias->leftSpan = 0;
            bias->rightSpan = 0;
            break;
        case 'w': // Jump
            bias->up = true;
            break;
        case 'h': // Use Heal/Mana potion
            if (potion == HEAL_CRYSTAL) {
                p->health += itemData[potion][I_VALUE];
                removeItem(HEAL_CRYSTAL);
            }
            else {
                setPromptMode(DIALOGUE_PROMPT);
                mvwprintw(getPromptWindow(0), 2, 2, "You don't have a potion equipped.");
            }
            break;
        case 'm':
            if (potion == MANA_CRYSTAL) {
                p->mp += itemData[potion][I_VALUE];
                removeItem(MANA_CRYSTAL);
            }
            else {
                setPromptMode(DIALOGUE_PROMPT);
                mvwprintw(getPromptWindow(0), 2, 2, "You don't have a potion equipped.");
            }
            break;
        case 'i': // Inventory
            setPromptMode(INV_CATEGORY_PROMPT);
            break;
        case ' ': { // Skill (costs MP)
            int weapon = inv.equipment[WEAPON];
            Vector norm = {1.0, 0.0};
            Vector src;

            if (!bias->facing) {
                norm[0] = -1.0;
            }

            src[0] = p->loc.pos[0] + norm[0] * 3;
            src[1] = p->loc.pos[1];
            
            if (weapon < 0) {
                setPromptMode(DIALOGUE_PROMPT);
                mvwprintw(getPromptWindow(0), 3, 3, "You must be equipped with a sword to activate the skill.");
            }
            else if (p->mp >= 40 && spawnSwordTrail(src, norm, 10 * itemData[weapon][I_VALUE])) {
                p->mp -= 40;
            }
            break;
        }
        case 'k': // Attack
            if (bias->attackCooldown == 0) {
                int i = 0;
                Entity *victim;
                float dist;
                bool succeed;

                while (i < MAX_ENTITY) {
                    victim = getEntityByID(i++);        

                    if (!victim || strcmp(victim->name, p->name) == 0)
                        continue;

                    dist = distance(victim->loc.pos, p->loc.pos);

                    if (dist < 3.0) {
                        attack(p, victim, dist);
                        succeed = true;
                    }
                }

                if (succeed)
                    bias->attackCooldown = getFramesDuringTime(700);
            }
            break;
    }
}

void updatePhysic(Entity* e) {
    if (!e) return;

    static int sleep = 30;
    float ground_y = 0.0;
    Location* l = &e->loc;
    Vector* last_pos = &l->last_pos;
    AABB* hitbox = &e->hitbox;
    Bias* bias = &e->bias;
    int agility, max_hp;

    if (e->type[0] == PLAYER) {
        agility = playerData[e->type[1]][P_AGI];
        max_hp = playerData[e->type[1]][P_MAX_HEALTH];
    }
    else if (e->type[0] == MONSTER) {
        agility = monsterData[e->type[1]][M_AGI];
        max_hp = monsterData[e->type[1]][M_MAX_HEALTH];
    }

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

    // Handle controls and gravity
    if (bias->leftSpan > 0) {
        l->spd[0] = -10.0 * (1 + agility / 100.0);
    }
    else if (bias->rightSpan > 0) {
        l->spd[0] = 10.0 * (1 + agility / 100.0);
    }
    else {
        l->spd[0] = 0.0;
    }

    if (bias->up) {
        bias->up = false;
        l->spd[1] = 10.0 * (1 + agility / 100.0);
    }

    if (!l->onGround) {
        l->spd[1] -= 20.0 * deltaTime;
    }

    if (e->loc.onGround) {
        if (bias->leftSpan > 0)
            bias->leftSpan--;

        if (bias->rightSpan > 0)
            bias->rightSpan--;
    }

    // Combat functions
    if(bias->attackCooldown > 0)
        bias->attackCooldown--;
    
    if (getTileAt(l->pos[0], l->pos[1]) == TRAIL) {
        e->health -= trail.damage / getFramesDuringTime(1000);
    }

    if (e->type[0] == PLAYER) {
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
                        e->health = max_hp;
                        e->mp = playerData[e->type[1]][P_MAX_MP];

                        if (portal->dest == DUNGEON) {
                            dungeon = portal->dungeon;
                        }

                        generateLevel(portal->dest);
                    }
                    break;
                }
            }
        }
    }
    else if (e->type[0] == MONSTER) {
        Vector e_pos, t_pos;
        float dist;

        e_pos[0] = e->loc.pos[0];
        e_pos[1] = e->loc.pos[1];
        t_pos[0] = e->target->loc.pos[0];
        t_pos[1] = e->target->loc.pos[1];
        dist = distance(e_pos, t_pos);

        if (bias->attackCooldown == 0) {
            if (dist < 2.0) {
                attack(e, e->target, dist);
                bias->attackCooldown = getFramesDuringTime(1000);
            }
        }

        float xDelta = e_pos[0] - t_pos[0];
        float yDelta = e_pos[1] - t_pos[1];

        if (e_pos[0] < 2) {
            bias->leftSpan = 0;
            bias->facing = true;
        }
        else if (e_pos[0] > level_width - 2) {
            bias->rightSpan = 0;
            bias->facing = false;
        }
        
        if (xDelta > 1.0) {
            if (bias->rightSpan == 0 && bias->leftSpan == 0) {
                bias->leftSpan = getFramesDuringTime(300);
                bias->rightSpan = 0;
                bias->facing = false;
            }
        }
        else if (xDelta < -1.0) {
            if (bias->leftSpan == 0 && bias->rightSpan == 0) {
                bias->leftSpan = 0;
                bias->rightSpan = getFramesDuringTime(300);
                bias->facing = true;
            }
        }
        else if (yDelta < -1.0) {
            bias->up = true;
        }
    }
}

void updateSwordTrail() {
    if (!trail.active)
        return;

    trail.pos[0] += trail.norm[0];
    trail.pos[1] += trail.norm[1];
    trail.dtg--;

    if ((trail.span - trail.dtg) > trail.length) {
        Vector del;

        if (trail.dtg < -trail.length) {
            trail.active = false;
            return;
        }

        del[0] = trail.pos[0] - trail.norm[0] * trail.length;
        del[1] = trail.pos[1] - trail.norm[1] * trail.length;

        if (getTileAt(del[0], del[1]) == TRAIL) {
            setTileAt(del[0], del[1], AIR);
        }

        if (getTileAt(del[0], del[1] + 1) == TRAIL) {
            setTileAt(del[0], del[1] + 1, AIR);
        }
    }

    if (trail.dtg > 0) {
        if (getTileAt(trail.pos[0], trail.pos[1]) == AIR)
            setTileAt(trail.pos[0], trail.pos[1], TRAIL);

        if (getTileAt(trail.pos[0], trail.pos[1] + 1) == AIR)
            setTileAt(trail.pos[0], trail.pos[1] + 1, TRAIL);
    }
}

static bool spawnSwordTrail(Vector pos, Vector norm, int damage) {
    if (trail.active)
        return false;

    trail.active = true;
    trail.damage = damage;
    trail.pos[0] = pos[0];
    trail.pos[1] = pos[1];
    trail.norm[0] = norm[0];
    trail.norm[1] = norm[1];
    trail.dtg = trail.span = 30;
    trail.length = 5 + rand() % 5;
    return true;
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

static void attack(Entity* entity, Entity* victim, float distance) {
    const float crit_dist = 2.0;
    const float crit_mul = 2.0;
    int weapon = inv.equipment[WEAPON];
    int armory = inv.equipment[ARMORY];
    float damage = 0.0;
    int strength = 0, absorb = 0;

    if (entity->type[0] == PLAYER) {
        if (weapon > -1) {
            damage = itemData[weapon][I_VALUE];
        } else {
            damage = entity->damage;
        }

        strength = playerData[entity->type[1]][P_STR];
    }
    else if (entity->type[0] == MONSTER) {
        damage = entity->damage;
        absorb = monsterData[victim->type[1]][M_ABSORB];
    }

    if (entity->type[0] == PLAYER && crit_dist > distance) {
        damage *= (1 - crit_mul) / crit_dist * distance + crit_mul;
    }

    if (victim->type[0] == PLAYER && armory > -1) {
        damage -= itemData[armory][I_VALUE];
    }

    damage += damage * (strength / 100);
    damage -= absorb;

    victim->health -= (damage > 0 ? floorf(damage) : 0);
}