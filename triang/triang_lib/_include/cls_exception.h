#pragma once

#include "cls_info.h"

#include <iostream>
#include <list>

#ifdef _MSC_VER
#define _WHAT_MODIFIERS
#elif defined(__GNUG__)
#define _WHAT_MODIFIERS _GLIBCXX_USE_NOEXCEPT
#else
#define _WHAT_MODIFIERS
#endif

#define __THROW__(cls_exception_derivative_type, add_name, add_val)                               \
    do {                                                                                          \
        cls_exception_derivative_type der;                                                        \
        std::list<cls_info> info_list;                                                            \
        info_list.push_back(cls_info("file", __FILE__));                                          \
        info_list.push_back(cls_info("line", std::to_string(__LINE__)));                          \
        if (add_name != "")                                                                       \
            info_list.push_back(cls_info(add_name, add_val));                                     \
        der.add_info(info_list);                                                                  \
        throw der;                                                                                \
    } while (0)

#define THROW(cls_exception_derivative_type) __THROW__(cls_exception_derivative_type, "", "")

class cls_exception : public std::exception {
    int _err_code;
    std::string _err_msg;
    std::string _err_info;

protected:
    cls_exception(int p_err_code, std::string p_err_msg);

public:
    const char* what() const _WHAT_MODIFIERS;
    void add_info(std::list<cls_info>& x);
    friend std::ostream& operator<<(std::ostream& os, cls_exception& x);
};
