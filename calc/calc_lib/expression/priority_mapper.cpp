#include <priority_mapper.h>

using namespace std;
using namespace expr;
using namespace tok;

priority_mapper::priority_mapper(const initializer_list<initializer_list<symbol_id>>& p_list)
    : _mapper()
    , _max()
{
    _max = 0;
    for (auto& i : p_list) {
        for (auto& j : i)
            if (j != symbol_id::NIL && _mapper.find(j) == _mapper.end())
                _mapper.insert(make_pair(j, _max));
        if (i.size())
            ++_max;
    }
    --_max;
}

bool priority_mapper::has_prior(const symbol_id& p_sym, const int& p_prior) const
{
    return prior(p_sym) == p_prior;
}

int priority_mapper::prior(const symbol_id& p_sym) const
{
    auto retval = no_prior;
    auto iter = _mapper.find(p_sym);
    if (iter != _mapper.end())
        retval = iter->second;
    return retval;
}

const int& priority_mapper::max_prior() const
{
    return _max;
}
