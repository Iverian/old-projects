#pragma once

#include "cls_triangulation.h"
#include "cls_triangle.h"

class cls_triang_node;
class cls_triangulation;

class cls_find_node {
    cls_triangulation* const _link;

public:
    struct result {
        ptrdiff_t tri;
        cls_vertex edge;
        enu_belong rel;
	    result();

	    result(const result& that)
            : tri(that.tri)
            , edge(that.edge)
            , rel(that.rel)
        {
        }
        result& operator=(const result& that)
        {
            tri = that.tri;
            rel = that.rel;
            edge = that.edge;
            return *this;
        }
    };
    explicit cls_find_node(cls_triangulation* const p_link);
    result operator()(const cls_dot& p_dot, const cls_triangulation::index_type p_curr) const;
};
