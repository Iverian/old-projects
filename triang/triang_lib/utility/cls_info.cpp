#include "cls_info.h"

cls_info::cls_info()
	: _param("")
	, _value("")
{
}

cls_info::cls_info(std::string p_param, std::string p_value)
	: _param(p_param)
	, _value(p_value)
{
}

std::string cls_info::param() const
{
	return _param;
}

std::string cls_info::value() const
{
	return _value;
}