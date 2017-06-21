#include "cls_triangle.h"
#include "exceptions.h"

void cls_triangle::cls_triangle_iterator::check_link() const
{
	if (_link == nullptr || !_link->is_exist())
		THROW(cls_invalid_iterator);
}

cls_triangle::cls_triangle_iterator::cls_triangle_iterator(const cls_triangle* p_link, const enu_vertex p_ver)
	: _link(p_link)
	, _ver(p_ver)
{
}

cls_triangle::cls_triangle_iterator::cls_triangle_iterator(const cls_triangle_iterator& that)
	: cls_triangle_iterator(that._link, that._ver)
{
}

cls_triangle::cls_triangle_iterator& cls_triangle::cls_triangle_iterator::operator=(const cls_triangle_iterator& that)
{
	if (&that != this) {
		_link = that._link;
		_ver = that._ver;
	}
	return *this;
}

cls_triangle::cls_triangle_iterator& cls_triangle::cls_triangle_iterator::operator++()
{
	check_link();
	++_ver;
	return *this;
}

cls_triangle::cls_triangle_iterator cls_triangle::cls_triangle_iterator::operator++(int)
{
	check_link();
	auto tmp(*this);
	++(*this);
	return tmp;
}

cls_triangle::cls_triangle_iterator cls_triangle::cls_triangle_iterator::cycle(const unsigned p_times)
{
	check_link();
	_ver.cycle(p_times);
	return *this;
}

cls_triangle::cls_triangle_iterator cls_triangle::cls_triangle_iterator::const_cycle(const unsigned p_times) const
{
	check_link();
	auto retval(*this);
	return retval.cycle(p_times);
}

cls_triangle::cls_triangle_iterator::operator int() const
{
	check_link();
	return int(_ver);
}

cls_triangle::cls_triangle_iterator::operator cls_vertex() const
{
	check_link();
	return _ver;
}

const cls_dot& cls_triangle::cls_triangle_iterator::operator*() const
{
	check_link();
	if (_ver == cls_vertex(enu_vertex::NIL))
		THROW(cls_iter_not_deferenciable);
	return (*const_cast<cls_triangle*>(_link))[_ver];
}

const cls_dot* cls_triangle::cls_triangle_iterator::operator->() const
{
	check_link();
	if (_ver == cls_vertex(enu_vertex::NIL))
		THROW(cls_iter_not_deferenciable);
	return &(*const_cast<cls_triangle*>(_link))[_ver];
}

bool cls_triangle::cls_triangle_iterator::operator==(const cls_triangle_iterator op2) const
{
	check_link();
	if (_link == op2._link)
		return _ver == op2._ver;
	THROW(cls_iter_not_assignable);
}

bool cls_triangle::cls_triangle_iterator::operator!=(const cls_triangle_iterator op2) const
{
	return !(*this == op2);
}
