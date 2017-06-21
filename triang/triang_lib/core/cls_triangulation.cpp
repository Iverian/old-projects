#include "cls_triangulation.h"
#include "cls_dbg_draw.h"
#include "cls_find_node.h"
#include "cls_round_about.h"
#include "cls_triang_node.h"
#include "exceptions.h"

static cls_triang_node not_exist;
static constexpr auto cache_growth = 3;

cls_triangulation::cls_triangulation(dots_cont& p_dots, unsigned p_scale)
    : _cont()
    , _cache(this, p_scale, cache_growth)
    , _dots_total(3)
{
    DBG__SETDOTCOUNT(_dots_total);
    add_first_tri(p_dots);

	DBG__DRAW("first_tri_added",
		*this
	);

    for (auto i = p_dots.begin() + 3; i != p_dots.end(); ++i)
        add_dot(*i);

	DBG__DRAW("finished",
		*this
	);
}

cls_triangulation::cls_triangulation(cls_triangle& p_first, unsigned p_scale)
    : _cont()
    , _cache(this, p_scale)
    , _dots_total(0)
{
    if (p_first.is_exist()) {
        add_node(cls_triang_node(this, p_first));
        _dots_total = 3;
        DBG__SETDOTCOUNT(_dots_total);
    }
}

void cls_triangulation::add_dot(const cls_dot& p_dot)
{
    add_cont add;

    DBG__DRAW("dot_to_add",
		*this,
		color::red, p_dot
	);

    auto found_node = cls_find_node(this)(p_dot, _cache.index(p_dot));
    if (!at(found_node.tri).is_vertex(p_dot)) {

        switch (found_node.rel) {
        case enu_belong::INSIDE:
            std::tie(_cache.set(p_dot), add) = add_dot_inside(p_dot, found_node.tri);
            break;
        case enu_belong::OUTSIDE:
            std::tie(_cache.set(p_dot), add) = add_dot_outside(p_dot, found_node.tri, found_node.edge);
            break;
        case enu_belong::EDGE_A:
        case enu_belong::EDGE_B:
        case enu_belong::EDGE_C:
            std::tie(_cache.set(p_dot), add) = add_dot_edge(p_dot, found_node.tri, found_node.edge);
            break;
        default:
            break;
        }
        for (auto i : add)
            delone_rebuild_node(i);
        DBG__INCDOTCOUNT;
        _cache.growth(++_dots_total);
        _cont.shrink_to_fit();
    }
}

std::tuple<cls_triangulation::index_type, std::vector<cls_triangulation::index_type>>
cls_triangulation::add_dot_inside(const cls_dot& p_dot, const index_type p_found_tri)
{
    add_cont add;
    auto tmp(at(p_found_tri));

    at(p_found_tri) = { this, p_dot, tmp[A], tmp[B] };
    auto a = p_found_tri;
    auto b = add_node({ this, p_dot, tmp[A], tmp[C] });
    auto c = add_node({ this, p_dot, tmp[B], tmp[C] });

    at(a).set_neigh(at(b), at(c), tmp.get_neigh(C));
    at(b).set_neigh(at(a), at(c), tmp.get_neigh(B));
    at(c).set_neigh(at(a), at(b), tmp.get_neigh(A));

    add.push_back(a);
    add.push_back(b);
    add.push_back(c);

    DBG__DRAW("inside_dot_added",
    	*this,
    	color::red, at(a),
    	color::green, at(b),
    	color::blue, at(c)
    );

    return std::make_tuple(a, std::move(add));
}

std::tuple<cls_triangulation::index_type, std::vector<cls_triangulation::index_type>>
cls_triangulation::add_dot_edge(const cls_dot& p_dot, const index_type p_found_tri, const cls_vertex p_vertex)
{
    add_cont add;
    auto edge = at(p_found_tri).get_edge(p_vertex);
    auto neigh_index = at(p_found_tri).get_neigh_index(p_vertex);
    auto tmp = at(p_found_tri);
    at(p_found_tri) = { this, p_dot, tmp[p_vertex], edge[0] };

    auto a = p_found_tri;
    auto b = add_node({ this, p_dot, tmp[p_vertex], edge[1] });

    if (neigh_index != NOT_DEF) {
        auto diff_ver = tmp.diff_ver(p_vertex);
        auto tmp_neigh = at(neigh_index);
        at(neigh_index) = { this, p_dot, diff_ver, edge[0] };

        auto c = neigh_index;
        auto d = add_node(cls_triang_node(this, p_dot, diff_ver, edge[1]));

        at(a).set_neigh(at(b), at(c), tmp.get_neigh(p_vertex.const_cycle(2)));
        at(b).set_neigh(at(a), at(d), tmp.get_neigh(p_vertex.const_cycle()));
        at(c).set_neigh(at(a), at(d), tmp_neigh.get_neigh(edge[1]));
        at(d).set_neigh(at(b), at(c), tmp_neigh.get_neigh(edge[0]));

        add.push_back(c);
        add.push_back(d);

        DBG__DRAW("edge_dot_added_4",
        	*this,
        	color::green, at(a), at(c),
        	color::blue, at(b), at(d)
        );

    } else {
        at(a).set_neigh(at(b), tmp.get_neigh(p_vertex.const_cycle(2)), not_exist);
        at(b).set_neigh(at(a), tmp.get_neigh(p_vertex.const_cycle()), not_exist);

        DBG__DRAW("edge_dot_added_2",
        	*this,
        	color::green, at(a),
        	color::blue, at(b)
        );
    }
    add.push_back(a);
    add.push_back(b);
    return std::make_tuple(a, std::move(add));
}

std::tuple<cls_triangulation::index_type, std::vector<cls_triangulation::index_type>>
cls_triangulation::add_dot_outside(const cls_dot& p_dot, const index_type p_found_tri, const cls_vertex p_edge)
{
    add_cont add;
    auto edge = at(p_found_tri).get_edge(p_edge);
    add.push_back(add_node({ this, p_dot, edge[0], edge[1] }));

    DBG__DRAW("outside_tri_pair",
    	*this,
    	color::blue, at(p_found_tri),
    	color::green, at(add[0])
    );

    at(add[0]).set_neigh(at(p_found_tri));

    cls_round_about round(this, &add);
    round(p_dot, edge[0], p_found_tri, add[0]);
    round(p_dot, edge[1], p_found_tri, add[0]);

    for (auto i = 0u; i < add.size(); ++i)
        for (auto j = i + 1; j < add.size(); ++j)
            at(add[i]).set_neigh(at(add[j]));

    DBG__DRAW("outside_dot_added",
		*this,
		color::red, p_dot
	);

    return std::make_tuple(add[0], std::move(add));
}

void cls_triangulation::delone_rebuild_node(const index_type p_index)
{
    auto& node = at(p_index);
    for (auto i = node.begin(); i != node.end(); ++i) {
        auto& neigh = node.get_neigh(i);
        if (neigh.is_exist() && !node.delone_check(neigh)) {

            DBG__DRAW("before_rebuild",
            	*this,
            	color::blue, node,
            	color::green, neigh
            );

            cls_dot diff_ver[] = { *i, node.diff_ver(i) };
            cls_dot same_ver[] = { *i.const_cycle(), *i.const_cycle(2) };
            cls_triang_node old[] = { node, neigh };
            node = { this, same_ver[0], diff_ver[0], diff_ver[1] };
            neigh = { this, same_ver[1], diff_ver[0], diff_ver[1] };
            node.set_neigh(neigh, old[0].get_neigh(same_ver[1]), old[1].get_neigh(same_ver[1]));
            neigh.set_neigh(node, old[0].get_neigh(same_ver[0]), old[1].get_neigh(same_ver[0]));

            DBG__DRAW("after_rebuild",
				*this,
				color::blue, node,
				color::green, neigh
			);

            for (auto j = node.begin(); j != node.end(); ++j)
                if (node.get_neigh(j).is_exist())
                    delone_rebuild_node(node.get_neigh_index(j));
            for (auto j = neigh.begin(); j != neigh.end(); ++j)
                if (neigh.get_neigh(j).is_exist())
                    delone_rebuild_node(neigh.get_neigh_index(j));
            break;
        }
    }
}

void cls_triangulation::add_first_tri(dots_cont& p_dots)
{
    cls_dot fir_tri[3] = { *p_dots.begin(), *(++p_dots.begin()) };
    dot_iter i;
    for (i = p_dots.begin() + 2; i != p_dots.end(); ++i) {
        if (!on_same_line(fir_tri[0], fir_tri[1], *i)) {
            fir_tri[2] = *i;
            _cont.push_back({ this, fir_tri });
            auto j = p_dots.begin();
            std::advance(j, 2);
            std::iter_swap(j, i);
            break;
        }
    }
    if (i == p_dots.end())
        THROW(cls_bad_dots);
}

cls_triangulation::index_type cls_triangulation::add_node(cls_triang_node&& p_node)
{
    _cont.push_back(p_node);
    return _cont.size() - 1;
}

cls_triangulation::index_type cls_triangulation::add_node(const cls_triang_node& p_node)
{
    _cont.push_back(p_node);
    return _cont.size() - 1;
}

cls_triangulation::index_type cls_triangulation::get_pos(const cls_triang_node& p_node) const
{
    if (p_node._link != this)
        THROW(cls_bad_link);
    auto retval = ptrdiff_t(&p_node - &_cont[0]);
    if (!(retval >= 0 && retval < intmax_t(_cont.size())))
        retval = std::distance(_cont.begin(), find(_cont.begin(), _cont.end(), p_node));
    return retval;
}

cls_triangulation::size_type cls_triangulation::size() const
{
    return _cont.size();
}

cls_image& cls_triangulation::draw(cls_image& p_image, const cls_image::color_type& p_color) const
{
    for (auto& i : _cont)
        i.draw(p_image, p_color);
    return p_image;
}

cls_triangulation::tri_iter cls_triangulation::begin()
{
    return _cont.begin();
}

cls_triangulation::tri_iter cls_triangulation::end()
{
    return _cont.end();
}

cls_triang_node& cls_triangulation::operator[](index_type index)
{
    if (index >= 0 && static_cast<decltype(_cont.size())>(index) < _cont.size())
        return _cont[index];
    THROW(cls_out_of_range);
}

const cls_triang_node& cls_triangulation::operator[](index_type index) const
{
    if (index >= 0 && static_cast<decltype(_cont.size())>(index) < _cont.size())
        return _cont[index];
    THROW(cls_out_of_range);
}

cls_triang_node& cls_triangulation::at(index_type p_index)
{
    return (*this)[p_index];
}

const cls_triang_node& cls_triangulation::at(index_type p_index) const
{
    return (*this)[p_index];
}
