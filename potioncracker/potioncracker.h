#pragma once

#include <z3++.h>
#include <vector>
#include "effects.h"

struct PotionCrackerResultEntry {
    const Effect* effect;
    unsigned potency;
};

struct PotionCrackerSettings {
    std::vector<std::vector<PotionCrackerResultEntry>> blacklisted_outputs;
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned effect_count = 0;
    bool respect_max_potency = true;
    bool invisible = false;
    bool glowing = false;
};

struct EffectVars {
    z3::expr r;
    z3::expr g;
    z3::expr b;
    z3::expr mul;
    z3::expr packed;
};

std::vector<PotionCrackerResultEntry> crack_potion(PotionCrackerSettings& settings);