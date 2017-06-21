#include "cls_dbg_draw.h"
#include "utility.h"

#include <string>

#ifndef N__DRAW

static const cls_dot nil(0, 0);

std::unique_ptr<cls_dbg_draw> cls_dbg_draw::_instance(new cls_dbg_draw());

cls_dbg_draw& cls_dbg_draw::instance()
{
	return *_instance;
}

void cls_dbg_draw::dbg_draw(const std::string& p_base_filename, const std::initializer_list<const cls_dr_variant>& p_rule_list)
{
    cls_image img(scale, scale, margin);
    auto current = color::black;
    for (auto& i : p_rule_list)
        switch (i.which()) {
        case cls_dr_variant::enu_value::color:
            current = i.color();
            break;
        case cls_dr_variant::enu_value::drawable:
            i.drawable().draw(img, current);
            break;
        default:
            break;
        }
    auto filename = _img_dir + std::to_string(_id++) + "[" + std::to_string(_count) + "]" + p_base_filename;
	if (file_exist(filename))
		filename += "_";
    for (; file_exist(filename); filename += _r.next())
        ;
    filename += _img_type;
    img.save(filename);
}

const size_t& cls_dbg_draw::set_count(const size_t& p_count)
{
	return _count = p_count;
}

const size_t& cls_dbg_draw::inc()
{
	return ++_count;
}

cls_dbg_draw::cls_dr_variant::cls_dr_variant(const cls_drawable& p_d)
    : _which(enu_value::drawable)
    , _d(p_d)
    , _c(color::black)
{
}

cls_dbg_draw::cls_dr_variant::cls_dr_variant(const cls_image::color_type& p_c)
    : _which(enu_value::color)
    , _d(nil)
    , _c(p_c)
{
}

cls_dbg_draw::cls_dr_variant::enu_value cls_dbg_draw::cls_dr_variant::which() const
{
    return _which;
}

const cls_image::color_type& cls_dbg_draw::cls_dr_variant::color() const
{
    return _c;
}

const cls_drawable& cls_dbg_draw::cls_dr_variant::drawable() const
{
    return _d;
}

cls_dbg_draw::cls_dbg_draw()
    : _id(0)
    , _count(3)
    , _r()
{
}

#endif
