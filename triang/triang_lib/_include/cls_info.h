#pragma once

#include <string>

class cls_info {
    std::string _param;
    std::string _value;

public:
    cls_info();
    cls_info(std::string p_param, std::string p_value);
    std::string param() const;
    std::string value() const;
};

