#include <expr_node.h>

using namespace expr;
using namespace std;
using namespace tok;
using namespace util;

expr_node::expr_node(const data_t& p_item)
    : _dat(p_item)
    , _id()
    , _children()
{
    set_id();
}

expr_node::expr_node(const data_t& p_item, const initializer_list<node_ptr>& p_children)
    : _dat(p_item)
    , _id()
    , _children()
{
    set_id();
	for (auto& i : p_children)
		add(i);
}

expr_node::expr_node(const data_t& p_item, const vector<node_ptr>& p_children)
    : _dat(p_item)
    , _id()
    , _children()
{
    set_id();
	for (auto& i : p_children)
		add(i);
}

expr_node::expr_node(const expr_node& p_node)
	: _dat(p_node._dat)
	, _id(p_node._id)
	, _children(p_node._children)
{
}

expr_node::expr_node(expr_node&& p_node) noexcept
	: _dat(p_node._dat)
	, _id(p_node._id)
	, _children(move(p_node._children))
{
	p_node._dat = data_t();
	p_node._id = node_id::NIL;
}

bool expr_node::empty() const
{
    return _id == node_id::NIL;
}

const node_id& expr_node::id() const
{
    return _id;
}

symbol_id expr_node::oper() const
{
    return convert<symbol_id>(_dat);
}

double expr_node::num() const
{
    return convert<double>(_dat);
}

string expr_node::expr() const
{
    return convert<string>(_dat);
}

size_t expr_node::arg() const
{
    return convert<size_t>(_dat);
}

expr_node::iter_t expr_node::begin() const
{
    return _children.begin();
}

expr_node::iter_t expr_node::end() const
{
    return _children.end();
}

const vector<node_ptr>& expr_node::child() const
{
	return _children;
}

size_t expr_node::size() const
{
    return _children.size();
}

void expr_node::add(const node_ptr& p_ptr)
{
    if (p_ptr != nullptr && !p_ptr->empty())
        _children.push_back(p_ptr);
}

const node_ptr& expr_node::at(const size_t& p_index) const
{
    return _children.at(p_index);
}

void expr_node::set_id()
{
    switch (_dat.index()) {
    case 0:
        _id = oper() != symbol_id::NIL ? node_id::OPR : node_id::NIL;
        break;
    case 1:
        _id = node_id::NUM;
        break;
    case 2:
        _id = !expr().empty() ? node_id::EXP : node_id::NIL;
		break;
    case 3:
        _id = node_id::ARG;
		break;
    default:
        _id = node_id::NIL;
		break;
    }
}
