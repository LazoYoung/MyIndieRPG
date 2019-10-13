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

typedef struct {
    char agility; // 1 ~ 100
    char strength; // 1 ~ 100
    char health; // 0 ~ 100
    char mp; // 1 ~ 100
    unsigned int level;
    unsigned long exp; // Level-up cost: 200 + (50 * level)
    const char* name; // Character name
} PlayerAttribute;

typedef struct {
    const char* name;
} Item;

typedef struct {
    Item* item;
    long long skills; // Achieved skills are recorded as codes in bit-manner.
    int coin;
} Inventory;

extern PlayerAttribute p_attr;
extern Inventory inv;
extern bool inGame;
extern const float deltaTime; // How much time does 1 frame take in seconds
extern const int fps; // How many frames and operations are made in 1 sec

extern void startGame();
extern void assignSkill(char);
extern bool hasSkill(char);
extern bool doTick();
extern int getFramesDuringTime(int); // fps : 1(s) = result : input(ms)