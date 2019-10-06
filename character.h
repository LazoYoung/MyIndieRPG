/**
 * Use bit masking/shifting to interpret these attribute macros.
 * One category can store up to [(bytes of type) * 8] different macros.
 * 
 * Passive Skill (int), Active Skill (int)
 */

// Passive Skill Attributes
#define PASSIVE_DUAL_WIELD 0
#define PASSIVE_SWIFTNESS 1
#define PASSIVE_EXP_BONUS 2
#define PASSIVE_DAMAGE_BONUS 3
#define PASSIVE_RECOVERY_BONUS 4

// Active Skill Attributes
#define ACTIVE_DOUBLE_JUMP 0

typedef struct character {
    char* name;
    int passive_skill;
    int active_skill;
} Character;