#pragma once

#include <expression.h>
#include <exception_helper.h>

DECL_EXCEPT_(const_expr_modify_attempt);

namespace context {
struct expr_wrap {
    using expr_t = expr::expression;
    explicit expr_wrap(const expr_t& p_item, bool p_const);

    bool is_const() const;
    const expr_t& get() const;
    expr_t& get();
    std::tuple<expr_t&, bool&> operator()();

private:
    bool _const;
    expr_t _item;
};
}
