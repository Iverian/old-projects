#pragma once

#include "cls_triangle.h"
#include "utility.h"

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <cstdint>

#ifndef N__DRAW

class cls_dbg_draw {
public:
    struct cls_dr_variant {
        enum class enu_value {
            drawable,
            color
        };
        cls_dr_variant(const cls_drawable& p_d);
        cls_dr_variant(const cls_image::color_type& p_c);
        enu_value which() const;
        const cls_image::color_type& color() const;
        const cls_drawable& drawable() const;

    private:
        enu_value _which;
        const cls_drawable& _d;
        const cls_image::color_type& _c;
    };
	cls_dbg_draw(const cls_dbg_draw&) = delete;
	cls_dbg_draw(cls_dbg_draw&&) = delete;

	static cls_dbg_draw& instance();

    void dbg_draw(const std::string& p_base_filename, const std::initializer_list<const cls_dr_variant>& p_rule_list);
	const size_t& set_count(const size_t& p_count);
	const size_t& inc();

private:
    static constexpr auto _img_dir = "./dbg_img/";
	static constexpr auto _img_type = ".bmp";
    static constexpr auto scale = 1000;
    static constexpr auto margin = 50;

	cls_dbg_draw();

	static std::unique_ptr<cls_dbg_draw> _instance;

    uint32_t _id;
    size_t _count;
	cls_rand_letter _r;
};

#define DBG__DRAW(basic_filename, ...) cls_dbg_draw::instance().dbg_draw(basic_filename, {__VA_ARGS__ })
#define DBG__SETDOTCOUNT(dot_count) cls_dbg_draw::instance().set_count(dot_count)
#define DBG__INCDOTCOUNT cls_dbg_draw::instance().inc()

#else

#define DBG__DRAW(basic_filename, ...)
#define DBG__SETDOTCOUNT(dot_count)
#define DBG__INCDOTCOUNT

#endif
