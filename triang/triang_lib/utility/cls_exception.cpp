#include "cls_exception.h"

#include <fstream>
#include <string>

cls_exception::cls_exception(int p_err_code, std::string p_err_msg)
    : std::exception()
    , _err_code(p_err_code)
    , _err_msg(p_err_msg)
    , _err_info("")
{
}

const char* cls_exception::what() const
{
    auto tmp = std::to_string(_err_code) + ": " + _err_msg;
    if (_err_info != "")
        tmp += "(" + _err_info + ")";
    auto retval = new char[tmp.length() + 1];
    strncpy_s(retval, tmp.length() + 1, tmp.c_str(), tmp.length() + 1);
    return retval;
}

void cls_exception::add_info(std::list<cls_info>& x)
{
    for (auto i:x)
        _err_info += i.param() + ": " + i.value() + "; ";
}

std::ostream& operator<<(std::ostream& os, cls_exception& x)
{
    auto tmp = x.what();
    os << "[ERROR]: " << tmp << "\n";
    delete[] tmp;
    return os;
}
