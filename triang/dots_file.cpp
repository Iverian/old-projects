#include "dots_file.h"

#include <fstream>
#include <iostream>
#include <random>
#include <set>

bool operator<(const cls_dot& lhs, const cls_dot& rhs);

dots_cont dots_gen(int p_count, const unsigned p_scale)
{
    dots_cont retval;
    std::set<int> gen_cont;
    std::minstd_rand0 r(time(nullptr));
    for (uint32_t i = 0; i < p_count; ++i) {
        cls_dot x = { int(r() % p_scale), int(r() % p_scale) };
        auto ord = dot_order(x);
        if (gen_cont.find(ord) == gen_cont.end()) {
            gen_cont.insert(ord);
            retval.push_back(x);
        }
    }
    return std::move(retval);
}

bool operator<(const cls_dot& lhs, const cls_dot& rhs)
{
    return dot_order(lhs) < dot_order(rhs);
}

void dots_save(dots_cont& dots, const unsigned scale, const char* filename)
{
    std::ofstream out(filename, std::ios_base::binary);
    if (out) {
        out.write(reinterpret_cast<const char*>(&scale), sizeof scale);
        for (auto i : dots) {
            int d[] = { i.x(), i.y() };
            out.write(reinterpret_cast<const char*>(d), sizeof d);
        }
    }
}

unsigned dots_get(dots_cont& dots, const char* filename)
{
    unsigned scale = 0;
    std::fstream in(filename, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
    if (!in) {
        std::cerr << "unable to get dots";
        std::terminate();
    }
    in.read(reinterpret_cast<char* const>(&scale), sizeof scale);
    for (int xy[2]; !in.read(reinterpret_cast<char* const>(xy), sizeof xy).eof();) {
        cls_dot tmp(xy[0], xy[1]);
        dots.push_back(tmp);
    }
    return scale;
}

int dot_order(const cls_dot& p_dot)
{
    return (p_dot.x() + p_dot.y() + 2) * (p_dot.x() + p_dot.y() + 1) / 2 + p_dot.y();
}
