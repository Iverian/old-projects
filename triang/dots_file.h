#pragma once

#include <vector>
#include <cls_triangulation.h>

dots_cont dots_gen(int p_count, const unsigned p_scale);
void dots_save(dots_cont& dots, const unsigned scale, const char* filename);
unsigned dots_get(dots_cont& dots, const char* filename);
int dot_order(const cls_dot& p_dot);
