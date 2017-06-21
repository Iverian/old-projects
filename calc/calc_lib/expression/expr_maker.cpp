#include <expr_eval.h>
#include <expr_maker.h>

#include <sstream>

using namespace std;
using namespace tok;
using namespace expr;
using namespace util;

expression expr_maker::operator()() const
{
    return _res;
}

expr_maker::expr_maker(tokenizer& p_is, const vector<string>& p_argname)
    : _tok(p_is)
    , _argname(p_argname)
    , _res()
{
    _res._argcount = _argname.size();
    _res._root = expr(true);
}

node_ptr expr_maker::expr(bool p_get)
{
    auto retval = expr_ternary(p_get);
    for (;;) {
        if (_tok.cur().to_symbol() == symbol_id::DROP) {
            retval = make_node(symbol_id::DROP, drop(retval));
            retval->add(expr_ternary(true));
        } else
            return retval;
    }
}

node_ptr expr_maker::expr_ternary(bool p_get, int p_prior)
{
    auto retval = next_expr(p_get, p_prior);
    for (;;) {
        if (_tok.cur().to_symbol() == symbol_id::IF) {
            auto true_ = next_expr(true, p_prior);
            CHECK_IF(_tok.cur().to_symbol() != symbol_id::ELSE, err::else_operator_missing);
            auto false_ = next_expr(true, p_prior);
            retval = expr_eval::local_eval(make_node(symbol_id::IF, retval, true_, false_));
        } else
            return retval;
    }
}

node_ptr expr_maker::expr_binary(bool p_get, int p_prior)
{
    auto retval = next_expr(p_get, p_prior);
    for (;;) {
        auto cur = _tok.cur().to_symbol();
        if (expression::map().has_prior(cur, p_prior))
            retval = expr_eval::local_eval(make_node(cur, retval, next_expr(true, p_prior)));
        else
            return retval;
    }
}

node_ptr expr_maker::primary(bool p_get)
{
    auto retval = make_empty();
    if (p_get)
        _tok.next();
    switch (_tok.cur().id()) {
    case token_id::NUM:
        retval = make_node(_tok.cur().to_num());
        _tok.next();
        break;
    case token_id::WRD: {
        auto word = _tok.cur().to_word();
        retval = make_node(word);
        _tok.next();
        switch (_tok.cur().to_symbol()) {
        case symbol_id::LPR:
            while (_tok.cur().is_valid() && _tok.cur().id() != token_id::END
                && _tok.cur().to_symbol() != symbol_id::RPR)
                retval->add(expr(true));

            CHECK_IF(
                !_tok.cur().is_valid(), err::primary_expected, "received: " + _tok.cur().raw());
            CHECK_IF(_tok.cur().id() == token_id::END || _tok.cur().to_symbol() != symbol_id::RPR,
                err::missing_right_parenth, "received: " + _tok.cur().raw());

            retval = expr_eval::local_eval(retval);
            _tok.next();
            break;
        case symbol_id::ASG:
            retval = make_node(symbol_id::ASG, retval, expr_ternary(true));
            break;
        default: {
            auto arg_no = is_argname(word);
            if (arg_no != not_arg)
                retval = make_node(arg_no);
        }
        }
        break;
    }
    case token_id::SYM:
        switch (_tok.cur().to_symbol()) {
        case symbol_id::MIN:
        case symbol_id::NOT: {
            auto cur = _tok.cur().to_symbol();
            retval = expr_eval::local_eval(make_node(cur, primary(true)));
            break;
        }
        case symbol_id::DEF:
        case symbol_id::DEF_CONST:
        case symbol_id::GLOB:
        case symbol_id::GLOB_CONST:
            retval = def(primary(true), _tok.cur().to_symbol());
            break;
        case symbol_id::LPR:
            retval = expr(true);
            CHECK_IF(_tok.cur().to_symbol() != symbol_id::RPR, err::missing_right_parenth,
                "received: " + _tok.cur().raw());
            _tok.next();
            break;
        case symbol_id::RPR:
        case symbol_id::COM:
            break;
        default:
            THROW(err::primary_expected, "received: " + _tok.cur().raw());
        }
        break;
    case token_id::NIL:
    default:
        THROW(err::primary_expected, "received: " + _tok.cur().raw());
    }
    return retval;
}

node_ptr expr_maker::def(const node_ptr& p_ptr, const symbol_id& p_id) const
{
    CHECK_IF(p_ptr->id() != node_id::EXP, err::invalid_local_expr_decl);
    CHECK_IF(p_ptr->size() > 1, err::invalid_local_expr_decl);
    return p_ptr->size() ? make_node(p_id, p_ptr, p_ptr->at(0)) : make_node(p_id, p_ptr);
}

vector<node_ptr> expr_maker::drop(const node_ptr& p_ptr)
{
    return drop_helper()(p_ptr);
}

size_t expr_maker::is_argname(const string& p_str) const
{
    auto iter = find(_argname.begin(), _argname.end(), p_str);
    return (iter != _argname.end()) ? size_t(distance(_argname.begin(), iter)) : size_t(not_arg);
}
node_ptr expr_maker::next_expr(bool p_get, int p_prior)
{
    return p_prior == expression::map().max_prior() ? primary(p_get)
                                                    : expr_binary(p_get, p_prior + 1);
}

bool expr_maker::is_def_symbol(const tok::symbol_id& p_id)
{
    auto retval = false;
    switch (p_id) {
    case symbol_id::DEF:
    case symbol_id::DEF_CONST:
    case symbol_id::GLOB:
    case symbol_id::GLOB_CONST:
        retval = true;
        break;
    default:
        break;
    }
    return retval;
}

vector<node_ptr> expr_maker::drop_helper::operator()(const node_ptr& p_ptr)
{
    exec_private(p_ptr);
    return _keep;
}

void expr_maker::drop_helper::exec_private(const node_ptr &p_ptr)
{
    if (p_ptr->id() == node_id::EXP ||
        p_ptr->oper() == symbol_id::ASG ||
        is_def_symbol(p_ptr->oper()))
        _keep.push_back(p_ptr);
    else
        for (size_t i = 0, size = p_ptr->size(); i != size; ++i)
            exec_private(p_ptr->at(i));
}
