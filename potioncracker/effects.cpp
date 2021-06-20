#include "effects.h"

std::unordered_map<unsigned, const Effect*> g_single_effect_map;

void init_single_effect_map() {
    for (int i = 0; i < sizeof(g_effect_table) / sizeof(Effect); i++) {
        const Effect* e = &g_effect_table[i];
        g_single_effect_map[(e->r << 16) | (e->g << 8) | e->b] = e;
    }
}