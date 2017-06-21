#pragma once

#include "cls_image.h"

class cls_drawable
{
public:
	virtual cls_image& draw(cls_image& p_img, const cls_image::color_type& color) const = 0;
	virtual ~cls_drawable() {};
};