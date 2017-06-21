#include <expr_context.h>
#include <func_mapper.h>

#include <math_helper.h>

using namespace std;
using namespace context;
using namespace expr;
using namespace math;

namespace context {
expression empty_expr;
}

expr_context* expr_context::_global(nullptr);

expr_context::expr_context(init_global)
    : _parent(nullptr)
    , _map()
{
}

expr_context& expr_context::global()
{
    if (!_global)
        _global = new expr_context(init_global());
    return *_global;
}

expr_context::expr_context()
    : _parent(_global)
    , _map()
{
}

expr_context& expr_context::parent() const
{
	return is_glob() ? *_global : *_parent;
}

expr_context expr_context::make_context()
{
    return expr_context(this);
}

expr_context::status expr_context::get_status(const string &p_name) const
{
	return func_mapper::instance().present(p_name) ? CONST : get_status_private(p_name);
}

expr_context::expr_context(expr_context* const p_parent)
    : _parent(p_parent)
	, _map()
{
}

bool expr_context::is_glob() const
{
	return this->_parent == nullptr;
}

const expr_context::expr_t& expr_context::get(const string& p_name) const
{
    if (present_here(p_name))
        return _map.at(p_name).get();
    if (!is_glob())
        return parent().get(p_name);
    return empty_expr;
}

bool expr_context::present_here(const string& p_name) const
{
    return _map.find(p_name) != _map.end();
}

void expr_context::insert(const string& p_name, const expr_t& p_expr, bool p_const)
{
    if (present_here(p_name))
        _map.at(p_name)() = tie(p_expr, p_const);
    else {
        CHECK_IF(func_mapper::instance().present(p_name), err::const_expr_modify_attempt);
        _map.insert(make_pair(p_name, expr_wrap(p_expr, p_const)));
    }
}

void expr_context::set(const string& p_name, const expr_t& p_expr)
{
    CHECK_IF(func_mapper::instance().present(p_name), err::const_expr_modify_attempt, "expr name: " + p_name);
	if (present_here(p_name))
		_map.at(p_name).get() = p_expr;
	else if (!is_glob())
		parent().set(p_name, p_expr);
	else
		THROW(err::expr_not_declared, "expr name: " + p_name);
}

expr_context::expr_t expr_context::exec(const string& p_name, const vector<expr_t>& p_argval)
{
    expr_t retval;
    if (func_mapper::instance().present(p_name))
        retval = func_mapper::instance().exec(p_name, p_argval);
	else {
		auto expr = get(p_name);
		auto cntx = make_context();
		retval = expr.empty() ? expr_t(p_name) : expr(cntx, p_argval);
	}
    return retval;
}

size_t expr_context::argcount(const string& p_name) const
{
    size_t retval;
    if (func_mapper::instance().present(p_name))
        retval = func_mapper::instance().argcount(p_name);
    else
        retval = get(p_name).argcount();
    return retval;
}

expr_context::status expr_context::get_status_private(const string &p_name) const
{
	auto retval = NOT_PRESENT;
	if (present_here(p_name))
		retval = _map.at(p_name).is_const() ? CONST : MUTABLE;
	if (retval == NOT_PRESENT && !is_glob())
		retval = parent().get_status_private(p_name);
	return retval;
}
