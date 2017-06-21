#pragma once

#include <union_t.h>
#include <token_id.h>

#include <initializer_list>
#include <memory>
#include <vector>

namespace expr {
struct expr_node;
}

template <class T, class... Types>
std::shared_ptr<expr::expr_node> make_node(const T& data, const Types&... args);

std::shared_ptr<expr::expr_node> make_empty();

namespace expr {

enum class node_id { NIL, OPR, NUM, EXP, ARG };

struct expr_node;
using node_ptr = std::shared_ptr<expr_node>;

struct expr_node {
    using data_t = util::union_t<tok::symbol_id, double, std::string, size_t>;
    using iter_t = std::vector<node_ptr>::const_iterator;

    expr_node(const data_t& p_item = data_t());
    expr_node(const data_t& p_item, const std::initializer_list<node_ptr>& p_children);
    expr_node(const data_t& p_item, const std::vector<node_ptr>& p_children);

    expr_node(const expr_node& p_node);
    expr_node(expr_node&& p_node) noexcept;

    bool empty() const;
    const node_id& id() const;
    tok::symbol_id oper() const;
    double num() const;
    std::string expr() const;
    size_t arg() const;

    iter_t begin() const;
    iter_t end() const;
    const std::vector<node_ptr>& child() const;
    size_t size() const;
    void add(const node_ptr& p_ptr);
    const node_ptr& at(const size_t& p_index) const;

private:
    void set_id();

    node_id _id;
    data_t _dat;
    std::vector<node_ptr> _children;
};
}

inline std::shared_ptr<expr::expr_node> make_empty()
{
    return std::shared_ptr<expr::expr_node>(nullptr);
}

template <class T, class... Types>
inline std::shared_ptr<expr::expr_node> make_node(const T& data, const Types&... args)
{
    return std::make_shared<expr::expr_node>(expr::expr_node::data_t(data),
        std::initializer_list<expr::node_ptr>({reinterpret_cast<const expr::node_ptr&>(args)...}));
}
