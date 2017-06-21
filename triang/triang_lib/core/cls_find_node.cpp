#include "cls_find_node.h"
#include "cls_triang_node.h"
#include "cls_triangulation.h"
#include "utility.h"
#include "cls_dbg_draw.h"

static cls_triang_node not_exist;

cls_find_node::result::result()
    : tri(NOT_DEF)
    , edge(enu_vertex::NIL)
    , rel(enu_belong::OUTSIDE)
{
}

cls_find_node::cls_find_node(cls_triangulation* const p_link)
    : _link(p_link)
{
}

inline enu_vertex to_ver(enu_belong x)
{
    auto retval = enu_vertex::NIL;
    switch (x) {
    case enu_belong::EDGE_A:
        retval = enu_vertex::A;
        break;
    case enu_belong::EDGE_B:
        retval = enu_vertex::B;
        break;
    case enu_belong::EDGE_C:
        retval = enu_vertex::C;
        break;
    default:
        break;
    }
    return retval;
}

#define AT(x) (_link->at(x))

cls_find_node::result cls_find_node::operator()(const cls_dot& p_dot, const cls_triangulation::index_type p_curr) const
{
    result retval;

	DBG__DRAW("add_search",
		reinterpret_cast<cls_drawable&>(*_link),
		color::green, AT(p_curr),
		color::red, p_dot
	);

    if (AT(p_curr).is_vertex(p_dot) == cls_vertex(enu_vertex::NIL)) {
        auto status = AT(p_curr).dot_belong(p_dot);
        if (status == enu_belong::OUTSIDE) {
            for (auto i = AT(p_curr).begin(); i != AT(p_curr).end(); ++i)
                if (!same_side(p_dot, AT(p_curr), i)) {
                    if (AT(p_curr).get_neigh(i).is_exist()) {
                        retval = operator()(p_dot, AT(p_curr).get_neigh_index(i));
                        if ((*_link)[retval.tri].is_exist())
                            break;
                    } else {
						retval.tri = p_curr;
                        retval.rel = status;
                        retval.edge = i;
                        break;
                    }
                }
        } else {
			retval.tri = p_curr;
            retval.rel = status;
            retval.edge = to_ver(status);
        }
    }
    return retval;
}

#undef AT
