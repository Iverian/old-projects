#pragma once

#include <string>
#include <type_traits>

namespace util {

template <class... Args>
class union_t;
template <class U, class... Args>
U convert(const union_t<Args...>& arg);

template <>
struct union_t<> {
    static int index() { return -1; }
};

template <class U, class... Args>
U convert(const union_t<>& arg)
{
    return arg;
}

template <class T, class... Args>
class union_t<T, Args...> {
    enum class data_stored { ITEM, NEXT };

public:
    using item_t = typename std::decay<T>::type;
    using next_t = union_t<Args...>;

    explicit union_t();
    explicit union_t(const item_t& p_dat);
    template <class Other>
    explicit union_t(const Other& p_dat);
    union_t& operator=(const item_t& p_dat);
    template <class Other>
    union_t& operator=(const Other& p_dat);
    union_t(const union_t& that);
    union_t& operator=(const union_t& that);
    int index() const;
    ~union_t();

    operator item_t() const { return flag == data_stored::ITEM ? dat.m_item : item_t(); }
    template <class Other>
    operator Other() const
    {
        return flag == data_stored::NEXT ? dat.m_next : Other();
    }

private:
    union union_data {
        union_data();
        union_data(const item_t& p_dat);
        union_data(const next_t& p_next);
        template <class Other>
        union_data(const Other& p_dat);
        ~union_data();

        item_t m_item;
        next_t m_next;
    };

    union_data dat;
    data_stored flag;
};

template <class T, class... Args>
inline void emplace(T& item, const Args&... args)
{
    new (&item) T(args...);
}

template <class T>
inline void destruct(T& item)
{
    item.~T();
}

template <class T, class... Args>
union_t<T, Args...>::union_data::union_data()
{
    emplace(m_item);
}

template <class T, class... Args>
union_t<T, Args...>::union_data::union_data(const item_t& p_dat)
{
    emplace(m_item);
}

template <class T, class... Args>
union_t<T, Args...>::union_data::union_data(const next_t& p_next)
{
    emplace(m_next, p_next);
}

template <class T, class... Args>
template <class Other>
union_t<T, Args...>::union_data::union_data(const Other& p_dat)
{
    emplace(m_next, p_dat);
}

template <class T, class... Args>
union_t<T, Args...>::union_data::~union_data()
{
}

template <class T, class... Args>
union_t<T, Args...>::union_t()
{
    flag = data_stored::ITEM;
    emplace(dat.m_item);
}

template <class T, class... Args>
union_t<T, Args...>::union_t(const item_t& p_dat)
{
    flag = data_stored::ITEM;
    emplace(dat.m_item, p_dat);
}

template <class T, class... Args>
template <class Other>
union_t<T, Args...>::union_t(const Other& p_dat)
    : dat(p_dat)
{
    flag = data_stored::NEXT;
}

template <class T, class... Args>
union_t<T, Args...>& union_t<T, Args...>::operator=(const item_t& p_dat)
{
    if (flag == data_stored::ITEM)
        emplace(dat.m_item, p_dat);
    else {
        destruct(dat.m_next);
        emplace(dat.m_item, p_dat);
        flag = data_stored::ITEM;
    }
    return *this;
}

template <class T, class... Args>
template <class Other>
union_t<T, Args...>& union_t<T, Args...>::operator=(const Other& p_dat)
{
    if (flag == data_stored::ITEM) {
        destruct(dat.m_item);
        emplace(dat.m_next, p_dat);
        flag = data_stored::NEXT;
    } else
        emplace(dat.m_next, p_dat);
    return *this;
}

template <class T, class... Args>
union_t<T, Args...>::union_t(const union_t& that)
{
    flag = that.flag;
    if (flag == data_stored::ITEM)
        emplace(dat.m_item, that.dat.m_item);
    else
        emplace(dat.m_next, that.dat.m_next);
}

template <class T, class... Args>
union_t<T, Args...>& union_t<T, Args...>::operator=(const union_t& that)
{
    if (that.flag == data_stored::ITEM)
        if (flag == data_stored::ITEM)
            emplace(dat.m_item, that.dat.m_item);
        else {
            destruct(dat.m_next);
            emplace(dat.m_item, that.dat.m_item);
        }
    else if (flag == data_stored::NEXT)
        emplace(dat.m_next, that.dat.m_next);
    else {
        destruct(dat.m_item);
        emplace(dat.m_next, that.dat.m_next);
    }
    flag = that.flag;
    return *this;
}

template <class T, class... Args>
int union_t<T, Args...>::index() const
{
    return flag == data_stored::ITEM ? 0 : dat.m_next.index() + 1;
}

template <class T, class... Args>
union_t<T, Args...>::~union_t()
{
    if (flag == data_stored::ITEM)
        destruct(dat.m_item);
    else
        destruct(dat.m_next);
}
}
