#pragma once

#include <token_id.h>

#include <initializer_list>
#include <limits>
#include <map>

namespace expr {

struct priority_mapper {
    static constexpr auto no_prior = std::numeric_limits<int>().max();

    priority_mapper(const std::initializer_list<std::initializer_list<tok::symbol_id>>& p_list);
    bool has_prior(const tok::symbol_id& p_sym, const int& p_prior) const;
    int prior(const tok::symbol_id& p_sym) const;
    const int& max_prior() const;

private:
    std::map<tok::symbol_id, int> _mapper;
    int _max;
};
}
