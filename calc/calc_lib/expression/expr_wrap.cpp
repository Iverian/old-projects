#include <expr_wrap.h>

using namespace context;
using namespace std;

expr_wrap::expr_wrap(const expr_t& p_item, bool p_const)
    : _const(p_const)
    , _item(p_item)
{
}

bool expr_wrap::is_const() const
{
    return _const;
}

const expr_wrap::expr_t& expr_wrap::get() const
{
    return _item;
}

expr_wrap::expr_t& expr_wrap::get()
{
    CHECK_IF(_const, err::const_expr_modify_attempt);
    return _item;
}

tuple<expr_wrap::expr_t&, bool&> expr_wrap::operator()()
{
    CHECK_IF(_const, err::const_expr_modify_attempt);
    return tie(_item, _const);
}

