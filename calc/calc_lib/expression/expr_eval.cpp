#include <expr_eval.h>
#include <expr_maker.h>
#include <utility.h>

#include <cmath>

using namespace expr;
using namespace tok;
using namespace util;
using namespace std;
using namespace context;

expr_eval::expr_eval(const expr_t& p_expr, expr_context& p_context, const vector<expr_t>& p_argval)
    : _argval()
    , _context(p_context)
    , _result()
{
    CHECK_IF(p_argval.size() < p_expr.argcount(), err::too_few_arguments);
    for (auto& i : p_argval)
        _argval.push_back(i(p_context).root_ptr());

    _result._argcount = 0;
    _result._root = exec(p_expr._root);
}

expression expr_eval::operator()() const
{
    return _result;
}

node_ptr expr_eval::exec(const node_ptr& p_ptr)
{
    auto retval = make_empty();
    switch (p_ptr->id()) {
    case node_id::EXP:
        retval = _context.exec(p_ptr->expr(), get_expr_list(exec_children(p_ptr))).root_ptr();
        break;
    case node_id::OPR:
        switch (p_ptr->oper()) {
        case symbol_id::DEF:
        case symbol_id::DEF_CONST:
        case symbol_id::GLOB:
        case symbol_id::GLOB_CONST: {
            auto name = p_ptr->at(0)->expr();
            retval = p_ptr->size() == 2 ? p_ptr->at(1) : make_node(0.);
            auto is_const
                = p_ptr->oper() == symbol_id::DEF_CONST || p_ptr->oper() == symbol_id::GLOB_CONST;
            if (p_ptr->oper() == symbol_id::DEF || p_ptr->oper() == symbol_id::DEF_CONST)
                _context.insert(name, retval, is_const);
            else
                expr_context::global().insert(name, retval, is_const);
            break;
        }
        case symbol_id::ASG: {
            retval = exec(p_ptr->at(1));
            _context.set(p_ptr->at(0)->expr(), retval);
            break;
        }
        case symbol_id::DROP: {
            auto child = exec_children(p_ptr);
            if (!child.empty())
                retval = child.back();
            break;
        }
        case symbol_id::IF: {
            auto cond = exec(p_ptr->at(0));
            if (cond->id() == node_id::NUM)
                retval = !is_zero(cond->num()) ? exec(p_ptr->at(1)) : exec(p_ptr->at(2));
            else
                retval = make_node(symbol_id::IF, cond, p_ptr->at(1), p_ptr->at(2));
            break;
        }
        default:
            retval = local_eval(make_node(p_ptr->oper(), exec_children(p_ptr)));
            break;
        }
        break;
    case node_id::ARG:
        retval = _argval.at(p_ptr->arg());
        break;
    case node_id::NUM:
        retval = p_ptr;
        break;
    default:
        break;
    }
    return retval;
}

vector<node_ptr> expr_eval::exec_children(const node_ptr& p_ptr)
{
    vector<node_ptr> retval;
    for (auto& i : *p_ptr)
        retval.push_back(exec(i));
    return retval;
}

vector<expression> expr_eval::get_expr_list(const vector<node_ptr>& p_vec)
{
    vector<expr_t> retval;
    for (auto& i : p_vec)
        retval.push_back(expr_t(i));
    return retval;
}

inline bool all_children_is_nums(const node_ptr& p_ptr)
{
    auto retval = true;
    for (auto& i : *p_ptr)
        if (i->id() != node_id::NUM) {
            retval = false;
            break;
        }
    return retval;
}

node_ptr expr_eval::local_eval(const node_ptr& p_node)
{
    auto retval = p_node;
    switch (p_node->id()) {
    case node_id::OPR:
        switch (p_node->oper()) {
        case symbol_id::IF:
            if (p_node->size() == 3 && p_node->at(0)->id() == node_id::NUM)
                retval = !is_zero(p_node->at(0)->num()) ? p_node->at(1) : p_node->at(2);
            break;
        case symbol_id::NOT:
            if (p_node->size() == 1 && p_node->at(0)->id() == node_id::NUM) {
                retval = make_node(is_zero(p_node->at(0)->num()) ? 1. : 0.);
            }
            break;
        case symbol_id::MIN:
            if (p_node->size() == 1) {
                if (p_node->at(0)->id() == node_id::NUM)
                    retval = make_node(-p_node->at(0)->num());
                break;
            }
        default:
            if (p_node->size() == 2 && p_node->at(0)->id() == node_id::NUM
                && p_node->at(0)->id() == p_node->at(1)->id()
                && expression::map().prior(p_node->oper()) != priority_mapper::no_prior)
                retval = make_node(
                    exec_binary_op(p_node->oper(), p_node->at(0)->num(), p_node->at(1)->num()));
            break;
        }
        break;
    case node_id::EXP: {
        auto name = p_node->expr();
        if (expr_context::global().get_status(name) == expr_context::CONST
            && p_node->size() == expr_context::global().argcount(name)
            && all_children_is_nums(p_node))
            retval = expr_context::global().exec(name, get_expr_list(p_node->child())).root_ptr();
    }
    default:
        break;
    }
    return retval;
}

double expr::exec_binary_op(const symbol_id& p_bin, double x, double y)
{
    auto retval = 0.;
    switch (p_bin) {
    case symbol_id::SUM:
        retval = x + y;
        break;
    case symbol_id::MIN:
        retval = x - y;
        break;
    case symbol_id::MUL:
        retval = x * y;
        break;
    case symbol_id::DIV:
        CHECK_IF(is_zero(y), err::division_by_zero);
        retval = x / y;
        break;
    case symbol_id::MOD:
        retval = std::fmod<double>(x, y);
        break;
    case symbol_id::POW:
        retval = std::pow<double>(x, y);
        break;
    case symbol_id::AND:
        retval = bool(x) && bool(y);
        break;
    case symbol_id::OR:
        retval = bool(x) || bool(y);
        break;
    case symbol_id::XOR:
        retval = bool(x) ^ bool(y);
        break;
    case symbol_id::EQ:
        retval = fuzzy_cmp(x, y);
        break;
    case symbol_id::NEQ:
        retval = !fuzzy_cmp(x, y);
        break;
    case symbol_id::LESS:
        retval = x < y;
        break;
    case symbol_id::MORE:
        retval = x > y;
        break;
    case symbol_id::LESS_EQ:
        retval = x < y || fuzzy_cmp(x, y);
        break;
    case symbol_id::MORE_EQ:
        retval = x > y || fuzzy_cmp(x, y);
        break;
    default:
        break;
    }
    return retval;
}
