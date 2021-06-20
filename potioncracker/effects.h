#pragma once

#include <string>
#include <unordered_map>

typedef struct {
    const char* name;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char max_potency; // for normal survival gameplay. servers *could* use higher values
} Effect;

//#define ONEPOINTEIGHT
// instant_health and instant_damage are excluded
constexpr Effect g_effect_table[] = {
    {"speed",               124, 175, 198, 2},
    {"slowness",            90,  108, 129, 6},
    {"haste",               217, 192, 67,  2},
    {"mining_fatigue",      74,  66,  23,  3},
    {"strength",            147, 36,  35,  2},
    {"jump_boost",          34,  255, 76,  2},
    {"nausea",              85,  29,  74,  1},
    {"regeneration",        205, 92,  171, 2},
    {"resistance",          153, 69,  58,  4},
    {"fire_resistance",     228, 154, 58,  1},
    {"water_breathing",     46,  82,  153, 1},
    {"invisibility",        127, 131, 146, 1},
    {"blindness",           31,  31,  35,  1},
    {"night_vision",        31,  31,  161, 1},
    {"hunger",              88,  118, 83,  3},
    {"weakness",            72,  77,  72,  1},
    {"poison",              78,  147, 49,  4},
    {"wither",              53,  42,  39,  2},
    {"health_boost",        248, 125, 35,  5}, // not seen in survival, using placeholder max level
    {"absorption",          37,  82,  165, 4},
    {"saturation",          248, 36,  35,  1},
#ifndef ONEPOINTEIGHT
    {"glowing",             148, 160, 97,  1},
    {"levitation",          206, 255, 255, 1},
    {"luck",                51,  153, 0,   1},
    {"unluck",              192, 164, 77,  5}, // not seen in survival, using placeholder max level
    {"slow_falling",        255, 239, 209, 1},
    {"conduit_power",       29,  194, 209, 1},
    {"dolphins_grace",      136, 163, 190, 1}, // wiki lists natural potency as "?", assuming max of 1
    {"bad_omen",            11,  97,  56,  6},
    {"hero_of_the_village", 68,  255, 68,  6},
#endif
};

extern std::unordered_map<unsigned, const Effect*> g_single_effect_map;
void init_single_effect_map();