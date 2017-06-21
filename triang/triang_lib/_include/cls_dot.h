#pragma once

#include "cls_drawable.h"

#include <initializer_list>

class cls_dot : public cls_drawable {
    int _x;
    int _y;

	static constexpr auto draw_rad = 3;
public:
    explicit cls_dot(const int p_x = 0, const int p_y = 0);
	cls_dot(const std::initializer_list<int>& p_list);
	cls_dot(const cls_dot& that);
	cls_dot& operator=(const cls_dot& that);
    int x() const;
    int y() const;
    bool operator==(const cls_dot& op2) const;
    bool operator!=(const cls_dot& op2) const;
    friend int sqr_distance(const cls_dot& op1, const cls_dot& op2);
    friend bool on_same_line(const cls_dot& a, const cls_dot& b, const cls_dot& c);
	cls_image& draw(cls_image& p_img, const cls_image::color_type& p_color) const override;
};
