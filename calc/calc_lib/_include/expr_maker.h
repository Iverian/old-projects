#pragma once

#include <exception_helper.h>
#include <expression.h>
#include <priority_mapper.h>
#include <tokenizer.h>

DECL_EXCEPT_(missing_right_parenth);
DECL_EXCEPT_(primary_expected);
DECL_EXCEPT_(else_operator_missing);
DECL_EXCEPT_(invalid_syntax);
DECL_EXCEPT_(invalid_local_expr_decl);

namespace expr {

struct expr_maker {
    friend struct expression;
    expression operator()() const;

private:
    struct drop_helper {
        std::vector<node_ptr> operator()(const node_ptr& p_ptr);

    private:
        void exec_private(const node_ptr& p_ptr);

        std::vector<node_ptr> _keep;
    };

    static constexpr auto not_arg = -1;

    explicit expr_maker(tok::tokenizer& p_is,
        const std::vector<std::string>& p_argname = std::vector<std::string>());

    node_ptr expr(bool p_get);
    node_ptr expr_ternary(bool p_get, int p_prior = 1);
    node_ptr expr_binary(bool p_get, int p_prior = 2);
    node_ptr primary(bool p_get);

    node_ptr next_expr(bool p_get, int p_prior);

    node_ptr def(const node_ptr& p_ptr, const tok::symbol_id& p_id) const;
    static bool is_def_symbol(const tok::symbol_id& p_id);

    static std::vector<node_ptr> drop(const node_ptr& p_ptr);

    size_t is_argname(const std::string& p_str) const;

    tok::tokenizer& _tok;
    std::vector<std::string> _argname;
    expression _res;
};
}
