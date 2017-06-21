#include "cls_vertex.h"

cls_vertex::cls_vertex(const enu_vertex& p_ver)
    : _ver(p_ver)
{
}

cls_vertex::cls_vertex(const cls_vertex& that)
    : _ver(that._ver)
{
}

cls_vertex::cls_vertex(cls_vertex&& that) noexcept
    : _ver(that._ver)
{
    that._ver = enu_vertex::NIL;
}

cls_vertex& cls_vertex::operator=(const enu_vertex& p_ver)
{
    _ver = p_ver;
    return *this;
}

cls_vertex& cls_vertex::operator=(const cls_vertex& that)
{
    _ver = that._ver;
    return *this;
}

bool cls_vertex::operator==(const cls_vertex& that) const
{
    return _ver == that._ver;
}

bool cls_vertex::operator!=(const cls_vertex& p_ver) const
{
    return !(*this == p_ver);
}

cls_vertex& cls_vertex::operator++()
{
    switch (_ver) {
    case enu_vertex::NIL:
        _ver = enu_vertex::A;
        break;
    case enu_vertex::A:
        _ver = enu_vertex::B;
        break;
    case enu_vertex::B:
        _ver = enu_vertex::C;
        break;
    case enu_vertex::C:
        _ver = enu_vertex::NIL;
        break;
	default:
		break;
    }
    return *this;
}

cls_vertex cls_vertex::operator++(int)
{
    auto retval(*this);
    ++(*this);
    return retval;
}

cls_vertex& cls_vertex::cycle(const unsigned p_times)
{
    if (_ver != enu_vertex::NIL)
        for (size_t i = 0; i < p_times % 3; ++i)
            switch (_ver) {
            case enu_vertex::A:
                _ver = enu_vertex::B;
                break;
            case enu_vertex::B:
                _ver = enu_vertex::C;
                break;
            case enu_vertex::C:
                _ver = enu_vertex::A;
                break;
            case enu_vertex::NIL:
			default:
                break;
            }
    return *this;
}

cls_vertex cls_vertex::const_cycle(const unsigned p_times) const
{
    auto retval(*this);
    return retval.cycle(p_times);
}

cls_vertex& cls_vertex::rev_cycle(const unsigned p_times)
{
	if (_ver != enu_vertex::NIL)
		for (size_t i = 0; i < p_times % 3; ++i)
			switch (_ver) {
			case enu_vertex::A:
				_ver = enu_vertex::C;
				break;
			case enu_vertex::B:
				_ver = enu_vertex::A;
				break;
			case enu_vertex::C:
				_ver = enu_vertex::B;
				break;
			case enu_vertex::NIL:
			default:
				break;
			}
	return *this;
}

cls_vertex cls_vertex::rev_const_cycle(const unsigned p_times) const
{
	auto retval(*this);
	return retval.rev_cycle(p_times);
}

cls_vertex::operator int() const
{
    return int(_ver) - 1;
}

cls_vertex::operator bool() const
{
	return _ver != enu_vertex::NIL;
}

cls_vertex::operator enu_vertex() const
{
    return _ver;
}

cls_vertex& ext_cycle(cls_vertex& ver)
{
	if (ver)
		++ver;
	else
		ver = enu_vertex::A;
	return ver;
}
