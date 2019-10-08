/* Skill codes are recorded in bit-manner. Cap: 64 */

// Sword skill
#define SINGLE_SHOT 0
#define RAGE_SPIKE 1
#define SONIC_LEAP 2
#define HORIZONTAL_ARC 3
#define HORIZONTAL_SQUARE 4
#define CROSS_BLOCK 5
#define DOUBLE_CIRCULAR 6
#define STARBURST_STREAM 7

// Axe skill 
#define DOUBLE_CLEAVE 20
#define WHIRL_WIND 21

// Movement skill
#define DOUBLE_JUMP 30

// Passive skill
#define EXP_BONUS 40
#define HEALTH_RECOVERY 41
#define MANA_RECOVERY 42

typedef struct str_p_attr {
    unsigned short agility; // 1 ~ 100
    unsigned short strength; // 1 ~ 100
    unsigned short health; // 0 ~ 100
    unsigned short mp; // 1 ~ 100
    unsigned short level;
    unsigned long exp; // Level-up cost: 200 + (50 * level)
    long long skills; // All the skills achieved by player
    int coin;
    const char* name; // Character name
} PlayerAttribute;

extern PlayerAttribute p_attr;

extern void startGame(PlayerAttribute);
extern void assignSkill(PlayerAttribute*, short);
extern bool isSkillAvailable(short skill_code);