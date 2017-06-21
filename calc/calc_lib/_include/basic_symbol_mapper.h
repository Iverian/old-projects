#pragma once

#include <exception_helper.h>

#include <algorithm>
#include <map>
#include <set>

DECL_EXCEPT_NODEFMSG(invalid_symbol);

namespace tok {

template <class SymbolId>
struct basic_symbol_mapper {
    using symbol_type = SymbolId;
    using pair_cont = std::map<std::string, SymbolId>;
    using init_pair = typename pair_cont::value_type;

    friend struct hoarder;

    struct hoarder {
        using search_range
            = std::pair<typename pair_cont::const_iterator, typename pair_cont::const_iterator>;
        friend struct basic_symbol_mapper;

        bool operator()(char p_add);
        symbol_type id() const;
        explicit operator bool() const;

    private:
        explicit hoarder(const basic_symbol_mapper& p_parent);
        search_range narrow_search_range(char p_add) const;
        static bool range_empty(const search_range& p_range);

        const basic_symbol_mapper& _parent;
        SymbolId _id;
        search_range _range;
        std::string _buf;
    };

    explicit basic_symbol_mapper(const std::initializer_list<init_pair>& p_list);
    const std::string& at(const symbol_type& p_id) const;
    hoarder make_hoarder() const;
    std::pair<symbol_type, char> get_symbol(std::istream& p_is) const;

private:
    std::set<char> _characters;
    pair_cont _map;
};
}

template <class SymbolId>
bool range_empty(const typename tok::basic_symbol_mapper<SymbolId>::hoarder::search_range& p_range)
{
    return p_range.first == p_range.second;
}

template <class SymbolId>
bool tok::basic_symbol_mapper<SymbolId>::hoarder::operator()(char p_add)
{
    auto retval = true;
    _range = narrow_search_range(p_add);
    if (!range_empty(_range)) {
        _buf += p_add;
        if (_buf == _range.first->first)
            _id = _range.first->second;
    } else
        retval = false;
    return retval;
}

template <class SymbolId>
typename tok::basic_symbol_mapper<SymbolId>::symbol_type
tok::basic_symbol_mapper<SymbolId>::hoarder::id() const
{
    return _id;
}

template <class SymbolId>
tok::basic_symbol_mapper<SymbolId>::hoarder::operator bool() const
{
    return !range_empty(_range);
}

template <class SymbolId>
tok::basic_symbol_mapper<SymbolId>::hoarder::hoarder(const basic_symbol_mapper& p_parent)
    : _parent(p_parent)
    , _id(SymbolId(0))
    , _range(p_parent._map.cbegin(), p_parent._map.cend())
    , _buf()
{
}

template <class SymbolId>
typename tok::basic_symbol_mapper<SymbolId>::hoarder::search_range
tok::basic_symbol_mapper<SymbolId>::hoarder::narrow_search_range(char p_add) const
{
    search_range retval(_range.second, _range.second);
    auto is_in_range = false;
    auto pos = _buf.size();
    for (auto i = _range.first; i != _range.second; ++i)
        if (!is_in_range) {
            if (i->first[pos] == p_add) {
                retval.first = i;
                is_in_range = true;
            }
        } else if (i->first[pos] != p_add) {
            retval.second = i;
            is_in_range = false;
        }
    return retval;
}

template <class SymbolId>
bool tok::basic_symbol_mapper<SymbolId>::hoarder::range_empty(const search_range& p_range)
{
    return p_range.first == p_range.second;
}

template <class SymbolId>
tok::basic_symbol_mapper<SymbolId>::basic_symbol_mapper(const std::initializer_list<init_pair>& p_list)
    : _characters()
    , _map(p_list)
{
    for (auto& i : p_list)
        for (auto& j : i.first)
            _characters.insert(j);
}

template <class SymbolId>
const std::string& tok::basic_symbol_mapper<SymbolId>::at(const symbol_type& p_id) const
{
    return (*std::find_if(_map.begin(), _map.end(), [&p_id](const init_pair& p_pair) {
        return p_pair.second == p_id;
    })).first;
}

template <class SymbolId>
typename tok::basic_symbol_mapper<SymbolId>::hoarder tok::basic_symbol_mapper<SymbolId>::make_hoarder() const
{
    return hoarder(*this);
}

template <class SymbolId>
std::pair<typename tok::basic_symbol_mapper<SymbolId>::symbol_type, char>
tok::basic_symbol_mapper<SymbolId>::get_symbol(std::istream& p_is) const
{
    char c;
    auto hoard = make_hoarder();
    while (p_is.get(c) && hoard(c))
        ;
    if (hoard.id() != SymbolId::NIL)
        p_is.putback(c);
    return std::make_pair(hoard.id(), c);
}
