#pragma once

#include <expr_context.h>
#include <expr_node.h>
#include <expression.h>
#include <exception_helper.h>

DECL_EXCEPT_(division_by_zero);

namespace expr {

struct expr_eval {
    using expr_t = expression;
    explicit expr_eval(const expr_t& p_expr, context::expr_context& p_context, const std::vector<expr_t>& p_argval);
	static node_ptr local_eval(const node_ptr& p_node);
    expression operator()() const;

private:
    node_ptr exec(const node_ptr& p_ptr);
	std::vector<node_ptr> exec_children(const node_ptr& p_ptr);

	static std::vector<expression> get_expr_list(const std::vector<node_ptr>& p_vec);

    std::vector<node_ptr> _argval;
    context::expr_context& _context;
    expr_t _result;
};

double exec_binary_op(const tok::symbol_id& p_bin, double x, double y);

}
