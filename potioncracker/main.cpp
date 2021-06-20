#include <vector>
#include <z3++.h>
#include <stdio.h>
#include <fmt/core.h>
#include <chrono>
#include "effects.h"
#include "potioncracker.h"

#define CATCH_EXCEPTIONS

int main() {
    init_single_effect_map();
    auto start = std::chrono::high_resolution_clock::now();
#ifdef CATCH_EXCEPTIONS
    try {
#endif
        PotionCrackerSettings settings;
        settings.respect_max_potency = true; // otherwise it will take a really, really long time
        //settings.invisible = true;
        //settings.glowing = true;
        // golden apple test
        settings.r = 117;
        settings.g = 91;
        settings.b = 139;
        for (int i = 1; i < sizeof(g_effect_table) / sizeof(Effect); i++) {
            printf("trying %d effects\n", i);
            settings.effect_count = i;
            auto res = crack_potion(settings);
            for (auto& r : res)
                fmt::print("{} {}\n", r.effect->name, r.potency);
            if (res.size() > 0)
                break;
        }
#ifdef CATCH_EXCEPTIONS
    }
    catch (z3::exception& e) {
        fmt::print("exception!!!! {}\n", e.msg());
    }
#endif

    fmt::print("ran in {} ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count());
    return 0;
}