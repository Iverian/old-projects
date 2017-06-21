#pragma once

#include <exception_helper.h>
#include <expression.h>
#include <expr_wrap.h>

#include <map>

DECL_EXCEPT_(expr_rewrite_attempt);
DECL_EXCEPT_(expr_not_declared);

namespace context {

struct expr_context {
	enum status
	{
		NOT_PRESENT,
		MUTABLE,
		CONST
	};
    using expr_t = expr::expression;
    using cont_t = std::map<std::string, expr_wrap>;

    explicit expr_context();
    expr_context& parent() const;
    expr_context make_context();

	status get_status(const std::string &p_name) const;

    bool present_here(const std::string& p_name) const;
    void insert(const std::string& p_name, const expr_t& p_expr, bool p_const);
	void set(const std::string& p_name, const expr_t& p_expr);
    expr_t exec(const std::string& p_name, const std::vector<expr_t>& p_argval);
    size_t argcount(const std::string& p_name) const;

	static expr_context& global();
private:
	status get_status_private(const std::string &p_name) const;

    struct init_global {};
    explicit expr_context(init_global);
    explicit expr_context(expr_context* const p_parent);

    bool is_glob() const;
    const expr_t& get(const std::string& p_name) const;

    static expr_context* _global;

	expr_context* const _parent;
    cont_t _map;
};

}
