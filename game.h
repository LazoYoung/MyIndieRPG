/* Skill codes are recorded in bit-manner. Cap: 64 */

// Sword skill
#define SINGLE_SHOT 0
#define RAGE_SPIKE 1
#define SONIC_LEAP 2
#define HORIZONTAL_ARC 3
#define HORIZONTAL_SQUARE 4
#define DOUBLE_CIRCULAR 5

// Unique skill
#define DUAL_WIELD 20
#define CROSS_BLOCK 21
#define STARBURST_STREAM 22
#define AXE_BERSERK 30
#define DOUBLE_CLEAVE 31
#define WHIRL_WIND 32

// Miscellaneous skill
#define DOUBLE_JUMP 40
#define EXP_BONUS 41
#define HEALTH_RECOVERY 42
#define MANA_RECOVERY 43

typedef struct str_p_attr {
    unsigned short agility; // 1 ~ 100
    unsigned short strength; // 1 ~ 100
    unsigned short health; // 0 ~ 100
    unsigned short mp; // 1 ~ 100
    unsigned short level;
    unsigned long exp; // Level-up cost: 200 + (50 * level)
    const char* name; // Character name
} PlayerAttribute;

typedef struct sword {

} Sword;

typedef struct inv {
    Sword* sword;
    long long skills;
    int coin;
} Inventory;

extern PlayerAttribute p_attr;
extern Inventory inv;

extern void startGame();
extern void assignSkill(char);
extern bool hasSkill(char);