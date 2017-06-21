#pragma once

#include <exception>
#include <iostream>

#ifdef _MSC_VER
#define _WHAT_MODIFIERS
#elif defined(__GNUG__)
#define _WHAT_MODIFIERS _GLIBCXX_USE_NOEXCEPT
#else
#define _WHAT_MODIFIERS
#endif

namespace err {
struct my_except : std::exception {
    const char* what() const _WHAT_MODIFIERS { return msg.c_str(); };
protected:
    std::string msg;
};
}

#define DECL_EXCEPT(exc_name, def_msg)                                                            \
    namespace err {                                                                               \
        struct exc_name : my_except {                                                             \
            exc_name(const std::string& p_msg)                                                    \
                : my_except()                                                                     \
            {                                                                                     \
                std::string def(#def_msg);                                                        \
                msg = (!def.empty() ? (def + ": ") : "") + p_msg;                                 \
            }                                                                                     \
        };                                                                                        \
    }
#define DECL_EXCEPT_(exc_name) DECL_EXCEPT(exc_name, exc_name)
#define DECL_EXCEPT_NODEFMSG(exc_name) DECL_EXCEPT(exc_name, "")

#ifndef NDEBUG
#define _THROW_DBG_STR (std::string(__FILE__) + " : " + std::to_string(__LINE__) + " : ")
#else
#define _THROW_DBG_STR std::string("")
#endif

#define THROW(exc_name, ...)                                                                      \
    throw exc_name("[ERROR] : " + _THROW_DBG_STR + std::string(__VA_ARGS__))

#define CHECK_IF(cond, exc_name, ...)                                                             \
    do {                                                                                          \
        if (cond)                                                                                 \
            THROW(exc_name, __VA_ARGS__);                                                         \
    } while (0)

#define CATCH(var_name) catch (const std::exception& var_name)
