#pragma once

#include <expr_node.h>
#include <exception_helper.h>

#include <vector>
#include <priority_mapper.h>

namespace tok {
	struct tokenizer;
}

namespace context {
	struct expr_context;
}

DECL_EXCEPT_(too_few_arguments);
DECL_EXCEPT_(empty_expression_call);

namespace expr {

struct expression {
    friend struct expr_maker;
	friend struct expr_eval;

	explicit expression();
	explicit expression(const double& p_num);
	explicit expression(const std::string& p_expr_name);
	explicit expression(tok::tokenizer& p_tok, const std::vector<std::string>& p_argname = std::vector<std::string>());

    expression operator()(context::expr_context& p_context) const;
    expression operator()(context::expr_context& p_context, const std::vector<expression>& p_argval) const;

    std::string str() const;
    size_t argcount() const;

	bool empty() const;
	bool is_number() const;
	double to_number() const;

	static const priority_mapper& map();

private:
	expression(const node_ptr& p_ptr);

	std::string to_str(const node_ptr& p_node) const;
	expr_node& root() const;
	node_ptr root_ptr() const;

	static const priority_mapper _mapper;
    node_ptr _root;
    size_t _argcount;
};
}
