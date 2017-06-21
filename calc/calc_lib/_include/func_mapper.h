#pragma once

#include <expression.h>

#include <map>
#include <memory>

namespace context {

struct func_mapper {
    using func_t = double (*)(double);
    using expr_t = expr::expression;
    using cont_t = std::map<std::string, func_t>;

    static const func_mapper& instance();

    bool present(const std::string& p_name) const;
    expr_t exec(const std::string& p_name, const std::vector<expr_t>& p_arg) const;
    size_t argcount(const std::string& p_name) const;

private:
    func_mapper(const std::initializer_list<cont_t::value_type>& p_list);

    static std::unique_ptr<func_mapper> _inst;
    cont_t _map;
};

}
