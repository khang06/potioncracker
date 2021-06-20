#include <z3++.h>
#include <fmt/core.h>
#include <numeric>
#include <execution>
#include "potioncracker.h"

inline z3::expr sum_bv32(z3::expr_vector vec) {
    z3::expr sum = vec[0].ctx().bv_val(0, 32);
    for (auto x : vec)
        sum = sum + x; // += doesn't work
    return sum;
}

std::vector<PotionCrackerResultEntry> crack_potion(PotionCrackerSettings& settings) {
    std::vector<PotionCrackerResultEntry> output;

    if (settings.effect_count == 0 || settings.effect_count > sizeof(g_effect_table) / sizeof(Effect))
        throw "Invalid effect count!";

    if (settings.effect_count == 1) {
        // can be lazy and just look up the color
        unsigned packed_color = (settings.r << 16) | (settings.g << 8) | settings.b;
        if (g_single_effect_map.find(packed_color) != g_single_effect_map.end())
            output.push_back({ g_single_effect_map[packed_color], 0xFFFFFFFF });
        return output;
    }

    z3::context c;
    z3::solver s(c);

    // define variables
    std::vector<EffectVars> effect_vars;
    for (unsigned i = 0; i < settings.effect_count; i++) {
        EffectVars v = {
            c.bv_const(fmt::format("effectr{}", i).c_str(), 32),
            c.bv_const(fmt::format("effectg{}", i).c_str(), 32),
            c.bv_const(fmt::format("effectb{}", i).c_str(), 32),
            c.bv_const(fmt::format("effectmul{}", i).c_str(), 32),
            c.bv_const(fmt::format("effectpacked{}", i).c_str(), 32),
        };

        // to help out the solver, they will be capped as they should
        s.add(z3::ule(v.r, 255));
        s.add(z3::ule(v.g, 255));
        s.add(z3::ule(v.b, 255));
        s.add(z3::ule(v.mul, 255));
        s.add(z3::ugt(v.mul, 0));
        
        // this will speed up comparing against a specific effect (uniqueness, blacklisting, etc)
        // for bit shifting, i have to call the function directly instead of using the overload
        // otherwise it converts the expr into an int for some reason
        s.add(v.packed == (z3::shl(v.r, 16) | z3::shl(v.g, 8) | v.b));

        // now for the actual constraints
        {
            // ensure that the chosen colors correspond to a valid effect
            // this was a one-liner in python, but the same can't be done with expr_vector
            z3::expr_vector vec1(c);
            for (auto& e : g_effect_table) {
                if (!settings.invisible && strcmp(e.name, "invisibility") == 0)
                    continue;
                if (!settings.glowing && strcmp(e.name, "glowing") == 0)
                    continue;
                vec1.push_back(v.packed == ((e.r << 16) | (e.g << 8) | e.b));
            }
            s.add(z3::mk_or(vec1));
        }
        if (settings.respect_max_potency) {
            // ensure that the corresponding effect is within the limits
            for (auto& e : g_effect_table) {
                // these are probably unnecessary
                if (!settings.invisible && strcmp(e.name, "invisibility") == 0)
                    continue;
                if (!settings.glowing && strcmp(e.name, "glowing") == 0)
                    continue;
                s.add(z3::ite(v.packed == ((e.r << 16) | (e.g << 8) | e.b), v.mul <= c.num_val(e.max_potency, c.bv_sort(32)), v.mul == v.mul));
            }
        }
        effect_vars.push_back(v);
    }
    // defining this speeds up calculation for some reason
    z3::expr mul_sum = c.bv_const("mulsum", 32);
    z3::expr_vector mul_vec(c);
    for (auto& x : effect_vars)
        mul_vec.push_back(x.mul);
    s.add(mul_sum == sum_bv32(mul_vec));

    // constraints
    // make sure every effect is unique
    z3::expr_vector packed_vec(c);
    for (auto& x : effect_vars)
        packed_vec.push_back(x.packed);
    s.add(z3::distinct(packed_vec));

    // TODO: blacklist

    z3::expr_vector rmul_vec(c);
    z3::expr_vector gmul_vec(c);
    z3::expr_vector bmul_vec(c);
    for (auto& x : effect_vars) {
        rmul_vec.push_back(x.r * x.mul);
        gmul_vec.push_back(x.g * x.mul);
        bmul_vec.push_back(x.b * x.mul);
    }
    s.add(sum_bv32(rmul_vec) / mul_sum == c.num_val(settings.r, c.bv_sort(32)));
    s.add(sum_bv32(gmul_vec) / mul_sum == c.num_val(settings.g, c.bv_sort(32)));
    s.add(sum_bv32(bmul_vec) / mul_sum == c.num_val(settings.b, c.bv_sort(32)));

    switch (s.check()) {
    case z3::sat:
        fmt::print("sat\n");
        //std::cout << s.to_smt2();
        for (auto& e : effect_vars) {
            unsigned packed_color = s.get_model().eval(e.packed).get_numeral_uint();
            if (g_single_effect_map.find(packed_color) == g_single_effect_map.end())
                throw "this shouldn't be possible";
            output.push_back({ g_single_effect_map[packed_color], s.get_model().eval(e.mul).get_numeral_uint() });
        }
        break;
    case z3::unsat:
        fmt::print("unsat\n");
        break;
    case z3::unknown:
        fmt::print("wtf\n");
        break;
    }
    return output;
}