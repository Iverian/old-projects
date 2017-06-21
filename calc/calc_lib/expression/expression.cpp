#include <expr_context.h>
#include <expr_eval.h>
#include <expr_maker.h>
#include <expression.h>

#include <string>

using namespace std;
using namespace expr;
using namespace tok;
using namespace context;

#define s(__x__) symbol_id::__x__

const priority_mapper expression::_mapper({
    // binary but special
    {s(DROP)},
    // ternary
    {s(IF), s(ELSE)},
    // binary
    {s(OR)}, {s(XOR)},
    {s(AND)},
    {s(EQ), s(NEQ)},
    {s(LESS), s(MORE), s(LESS_EQ), s(MORE_EQ)},
    {s(SUM), s(MIN)},
    {s(MUL), s(DIV), s(MOD)},
    {s(POW)},
});

#undef s

expression::expression()
    : _root(nullptr)
    , _argcount(0)
{
}

expression::expression(const double& p_num)
    : _root(make_node(p_num))
    , _argcount(0)
{
}

expression::expression(const string& p_expr_name)
    : _root(make_node(p_expr_name))
    , _argcount(0)
{
}

expression expression::operator()(
    expr_context& p_context, const vector<expression>& p_argval) const
{
    if (empty())
        return expression();
    return expr_eval(*this, p_context, p_argval)();
}

string expression::str() const
{
    return !empty() ? to_str(_root) : string();
}

size_t expression::argcount() const
{
    return _argcount;
}

bool expression::empty() const
{
    return _root == nullptr;
}

bool expression::is_number() const
{
    return !empty() && root().id() == node_id::NUM && root().size() == 0;
}

double expression::to_number() const
{
    return !empty() ? root().num() : 0.;
}

const priority_mapper& expression::map()
{
    return _mapper;
}

expression::expression(const node_ptr& p_ptr)
    : _root(p_ptr)
    , _argcount(0)
{
}

expression::expression(tokenizer& p_tok, const vector<string>& p_argname)
{
    *this = expr_maker(p_tok, p_argname)();
}

inline string parenthness(bool cond, const string& x) {
    return cond ? "(" + x + ")" : x;
}

inline int priority(const symbol_id& x) {
    return expression::map().prior(x);
}

string expression::to_str(const node_ptr& p_node) const
{
    string retval;
    vector<string> child;
    for (size_t i = 0; i != p_node->size(); ++i)
        child.push_back(to_str(p_node->at(i)));
    switch (p_node->id()) {
    case node_id::EXP:
        retval = p_node->expr();
        if (!child.empty()) {
            retval += "(";
            for (auto i = child.begin(); i != child.end(); ++i)
                retval += *i + ((next(i) == child.end()) ? ")" : ", ");
        }
        break;
    case node_id::OPR:
        switch (p_node->oper()) {
        case symbol_id::IF:
            retval = "(" + child.at(0) + ") " + token(symbol_id::IF).raw() + " (" + child.at(1)
                + ") " + token(symbol_id::ELSE).raw() + " (" + child.at(2) + ")";
            break;
        case symbol_id::NOT:
            retval = token(symbol_id::NOT).raw() + parenthness(p_node->at(0)->id() != node_id::NUM
                                                           && p_node->at(0)->id() != node_id::ARG,
                                                       child.at(0));
            break;
        case symbol_id::MIN:
            if (p_node->size() == 1) {
                retval
                    = token(symbol_id::MIN).raw() + parenthness(p_node->at(0)->id() != node_id::NUM
                                                            && p_node->at(0)->id() != node_id::ARG,
                                                        child.at(0));
                break;
            }
        case symbol_id::DEF:
        case symbol_id::DEF_CONST:
            retval = token(p_node->oper()).raw() + child.at(0) + "("
                + ((p_node->size() >= 2) ? child.at(1) : "0") + ")";
            break;
        default:
            retval = parenthness(priority(p_node->at(0)->oper()) < priority(p_node->oper()), child.at(0))
                + token(p_node->oper()).raw()
                + parenthness(priority(p_node->at(1)->oper()) < priority(p_node->oper()), child.at(1));
            break;
        }
        break;
    case node_id::NUM:
        retval = to_string(p_node->num());
        break;
    case node_id::ARG:
        retval = "#" + to_string(p_node->arg());
        break;
    default:
        break;
    }
    return retval;
}

expr_node& expression::root() const
{
    return *_root;
}

node_ptr expression::root_ptr() const
{
    return _root;
}

expression expression::operator()(context::expr_context& p_context) const
{
    return operator()(p_context, std::vector<expression>());
}
