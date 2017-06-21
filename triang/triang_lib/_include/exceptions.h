#pragma once

#include "cls_exception.h"

#ifndef NDEBUG
#define DBG if(1)
#else
#define DBG if(0)
#endif

enum class enu_err_codes {
    out_of_range,
    bad_pointer,
    bad_dots,
    bad_triangle,
    bad_link,
    invalid_iterator,
    iter_not_assignable,
    iter_not_deferenciable,
    invalid_node,
    invalid_link
};

class cls_out_of_range : public cls_exception {
public:
    cls_out_of_range()
        : cls_exception(int(enu_err_codes::out_of_range),
              "_index is out of array's range")
    {
    }
};

class cls_bad_pointer : public cls_exception {
public:
    cls_bad_pointer()
        : cls_exception(int(enu_err_codes::bad_pointer),
              "dereference of nullptr")
    {
    }
};

class cls_bad_triangle : public cls_exception {
public:
    cls_bad_triangle()
        : cls_exception(int(enu_err_codes::bad_triangle),
              "unable to access nonexisting triangle")
    {
    }
};

class cls_bad_dots : public cls_exception {
public:
    cls_bad_dots()
        : cls_exception(int(enu_err_codes::bad_dots),
              "unable to triangulate a line")
    {
    }
};

class cls_bad_link : public cls_exception {
public:
    cls_bad_link()
        : cls_exception(int(enu_err_codes::bad_link),
              "nodes from different containers are not assignable")
    {
    }
};

class cls_invalid_iterator : public cls_exception {
public:
    cls_invalid_iterator()
        : cls_exception(int(enu_err_codes::invalid_iterator), "iterator is invalid")
    {
    }
};

class cls_iter_not_assignable : public cls_exception {
public:
    cls_iter_not_assignable()
        : cls_exception(int(enu_err_codes::iter_not_assignable),
              "iterators from different containers are not assignable")
    {
    }
};

class cls_iter_not_deferenciable : public cls_exception {
public:
    cls_iter_not_deferenciable()
        : cls_exception(int(enu_err_codes::iter_not_deferenciable), "iterator to end is not differenciable")
    {
    }
};

class cls_invalid_node : public cls_exception {
public:
    cls_invalid_node()
        : cls_exception(int(enu_err_codes::invalid_node), "triangulation node is invalid")
    {
    }
};

class cls_invalid_link : public cls_exception {
public:
    cls_invalid_link()
        : cls_exception(int(enu_err_codes::invalid_link), "link is invalid")
    {
    }
};