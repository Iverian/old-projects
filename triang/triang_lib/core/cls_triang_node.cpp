#include "exceptions.h"
#include "cls_triang_node.h"
#include "cls_triangulation.h"

static cls_triang_node not_exist;

void cls_triang_node::check_link() const
{
    if (_link == nullptr)
        THROW(cls_invalid_node);
}

cls_triang_node::cls_triang_node()
    : cls_triang_node(nullptr, cls_triangle())
{
}

cls_triang_node::cls_triang_node(cls_triangulation* const p_link)
	: cls_triangle()
	, _link(p_link)
	, _neigh{NOT_DEF, NOT_DEF, NOT_DEF}
{
}

cls_triang_node::cls_triang_node(cls_triangulation* const p_link, const cls_triangle& p_tri)
    : cls_triangle(p_tri)
    , _link(p_link)
{
}

cls_triang_node::cls_triang_node(cls_triangulation* const p_link, const cls_dot& p_a, const cls_dot& p_b, const cls_dot& p_c)
    : cls_triangle(p_a, p_b, p_c)
    , _link(p_link)
    , _neigh{ NOT_DEF, NOT_DEF, NOT_DEF }
{
}

cls_triang_node::cls_triang_node(cls_triangulation* const p_link, const cls_dot p_arr[3])
    : cls_triangle(p_arr[0], p_arr[1], p_arr[2])
    , _link(p_link)
    , _neigh{ NOT_DEF, NOT_DEF, NOT_DEF }
{
}

cls_triang_node::cls_triang_node(const cls_triang_node& that)
    : cls_triangle(that)
    , _link(that._link)
    , _neigh{ that._neigh[0], that._neigh[1], that._neigh[2] }
{
}

cls_triang_node& cls_triang_node::operator=(const cls_triang_node& that)
{
    this->cls_triangle::operator=(that);
    if (_link != that._link)
        THROW(cls_invalid_node);
    for (auto i = 0; i < 3; ++i)
        _neigh[i] = that._neigh[i];
    return *this;
}

bool cls_triang_node::operator==(const cls_triang_node& op2) const
{
    auto retval = _link == op2._link && this->cls_triangle::operator==(op2);
    if (retval)
        for (auto i = 0; i < 3; ++i)
            if (_neigh[i] != op2._neigh[i]) {
                retval = false;
                break;
            }
    return retval;
}

bool cls_triang_node::operator!=(const cls_triang_node& op2) const
{
    return !(*this == op2);
}

bool cls_triang_node::delone_check(const cls_triangle& p_tri) const
{
    bool retval;
    auto id = is_neigh(p_tri);
    if (id != enu_vertex::NIL)
        retval = delone_check_dot(diff_ver(p_tri));
    else
        retval = delone_check_dot(p_tri[enu_vertex::A]) && delone_check_dot(p_tri[enu_vertex::B]) && delone_check_dot(p_tri[enu_vertex::C]);
	return retval;
}

cls_triang_node& cls_triang_node::get_neigh(const cls_vertex p_ver) const
{
    check_link();
    if (p_ver == cls_vertex(enu_vertex::NIL))
        THROW(cls_invalid_node);
    return (size_t(_neigh[int(p_ver)]) < _link->size()) ? ((*_link)[_neigh[int(p_ver)]]) : not_exist;
}

cls_triang_node& cls_triang_node::get_neigh(const enu_vertex p_ver) const
{
    return get_neigh(cls_vertex(p_ver));
}

cls_triang_node& cls_triang_node::get_neigh(const cls_dot& p_dot) const
{
    return get_neigh(is_vertex(p_dot));
}

ptrdiff_t cls_triang_node::get_neigh_index(const cls_vertex p_ver) const
{
	check_link();
	if (p_ver == enu_vertex::NIL)
		THROW(cls_invalid_node);
	return _neigh[int(p_ver)];
}

ptrdiff_t cls_triang_node::get_neigh_index(const enu_vertex p_ver) const
{
	return get_neigh_index(cls_vertex(p_ver));
}

cls_vertex cls_triang_node::is_neigh(const cls_triangle& p_tri) const
{
    check_link();
	cls_vertex retval(NIL);
	if (p_tri.is_exist()) {
		cls_vertex not_match(NIL);
		auto count = 0;
		for (auto i = begin(); i != end(); ++i) {
			auto probe = (p_tri.is_vertex(*i) == NIL) ? false : true;
			count += probe;
			if (!probe)
				not_match = i;
		}
		retval = (count == 2) ? not_match : cls_vertex(NIL);
	}
    return retval;
}

void cls_triang_node::set_neigh(cls_triang_node& p_node)
{
	cls_vertex id;
	if (p_node.is_exist()) {
		id = is_neigh(p_node);
		if (id != enu_vertex::NIL && _neigh[int(id)] != _link->get_pos(p_node))
		{
			auto pos = _link->get_pos(p_node);
			_neigh[int(id)] = ((pos != std::distance(_link->begin(), _link->end())) ? pos : NOT_DEF);
			p_node.set_neigh(*this);
		}
	}
}

void cls_triang_node::set_neigh(cls_triang_node& op1, cls_triang_node& op2, cls_triang_node& op3)
{
	set_neigh(op1);
	set_neigh(op2);
	set_neigh(op3);
}

void cls_triang_node::set_neigh(cls_triang_node& op1, cls_triang_node& op2)
{
	set_neigh(op1);
	set_neigh(op2);
}

int cls_triang_node::count_neigh() const
{
	auto retval = 0;
	check_link();
	for (auto i = 0; i < 3; ++i)
		if (static_cast<size_t>(_neigh[i]) >= 0 && _neigh[i] < _link->size())
			++retval;
	return retval;
}

cls_dot cls_triang_node::diff_ver(const cls_vertex p_ver) const
{
    check_link();
    cls_dot retval;
    if (p_ver != cls_vertex(enu_vertex::NIL)) {
        auto neigh = get_neigh(p_ver);
        if (neigh == not_exist)
            THROW(cls_bad_pointer);
        for (auto i = begin(); i != end(); ++i)
            if (is_vertex(neigh[i]) == cls_vertex(enu_vertex::NIL)) {
                retval = neigh[i];
                break;
            }
    }
    return retval;
}

cls_dot cls_triang_node::diff_ver(const enu_vertex p_ver) const
{
	return diff_ver(cls_vertex(p_ver));
}

cls_dot cls_triang_node::diff_ver(const cls_triangle& p_tri) const
{
	check_link();
	cls_dot retval;
	auto ver = is_neigh(p_tri);
	if (ver != cls_vertex(enu_vertex::NIL))
		for (auto i = begin(); i != end(); ++i)
			if (is_vertex(p_tri[i]) == cls_vertex(enu_vertex::NIL))
			{
				retval = p_tri[i];
				break;
			}
	return retval;
}

