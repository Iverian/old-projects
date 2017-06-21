#pragma once

enum enu_vertex {
    NIL = 0,
    A,
    B,
    C
};

class cls_vertex {
    enu_vertex _ver;

public:
    explicit cls_vertex(const enu_vertex& p_ver = enu_vertex::NIL);
    cls_vertex(const cls_vertex& that);
    cls_vertex(cls_vertex&& that) noexcept;
    cls_vertex& operator=(const enu_vertex& p_ver);
    cls_vertex& operator=(const cls_vertex& that);
    bool operator==(const cls_vertex& that) const;
    bool operator!=(const cls_vertex& p_ver) const;
    cls_vertex& operator++();
    cls_vertex operator++(int);
    cls_vertex& cycle(const unsigned p_times = 1);
	cls_vertex const_cycle(const unsigned p_times = 1) const;
	cls_vertex& rev_cycle(const unsigned p_times = 1);
	cls_vertex rev_const_cycle(const unsigned p_times = 1) const;
    explicit operator int() const;
	explicit operator bool() const;
	operator enu_vertex() const;
};

cls_vertex& ext_cycle(cls_vertex& ver);
