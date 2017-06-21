#include <token.h>
#include <string>

using namespace std;
using namespace tok;

#define s(__x__) symbol_id::__x__

const symbol_mapper token::_mapper(
    {   { "(", s(LPR) },
        { ")", s(RPR) },
        { "=", s(ASG) },
        { ",", s(COM) },
        { "$", s(DEF) },
        { "$!", s(DEF_CONST) },
        { "@", s(GLOB)},
        { "@!", s(GLOB_CONST)},
        { "\\", s(TIE) },
        { ";", s(DROP) },

        { "+", s(SUM) },
        { "-", s(MIN) },
        { "*", s(MUL) },
        { "/", s(DIV) },
        { "%", s(MOD) },
        { "**", s(POW) },

        { "?", s(IF) },
        { ":", s(ELSE) },

        { "==", s(EQ) },
        { "!=", s(NEQ) },
        { "<", s(LESS) },
        { ">", s(MORE) },
        { "<=", s(LESS_EQ) },
        { ">=", s(MORE_EQ) },
        { "&", s(AND) },
        { "|", s(OR) },
        { "^", s(XOR) },
        { "!", s(NOT) } });

#undef s

token::token(const token_id& p_id)
    : _id(p_id)
    , _dat()
{
    if (_id == token_id::NIL)
        _dat = string();
}

token::token(const double& p_number)
    : _id(token_id::NUM)
    , _dat(p_number)
{
}

token::token(const std::string& p_word)
    : _id(token_id::WRD)
    , _dat(p_word)
{
}

token::token(std::string&& p_word) noexcept
    : _id(token_id::WRD)
    , _dat(p_word)
{
    p_word = string();
}

token::token(const symbol_id& p_symbol_id)
    : _id(token_id::SYM)
    , _dat(p_symbol_id)
{
    if (_dat == symbol_id::NIL) {
        _id = token_id::NIL;
        _dat = string();
    }
}

bool token::is_valid() const
{
    return _id != token_id::NIL;
}

token& token::make_invalid(const string& p_invalid)
{
    *this = token_id::NIL; // makes token invalid
    _dat = p_invalid;
    return *this;
}

string token::invalid_token_data() const
{
    return is_valid() ? string() : _dat;
}

const token_id& token::id() const
{
    return _id;
}

bool token::eof() const
{
    return _id == token_id::END;
}

bool token::eol() const
{
    return _id == token_id::EOL;
}

bool token::empty() const
{
    return eol() || eof();
}

const symbol_mapper& token::map()
{
    return _mapper;
}

double token::to_num() const
{
    return _dat;
}

string token::to_word() const
{
    return _dat;
}

symbol_id token::to_symbol() const
{
    return _dat;
}

string token::raw() const
{
    string retval;
    switch (_id) {
    case token_id::NIL:
        retval = invalid_token_data();
        break;
    case token_id::END:
        retval = "\\0";
        break;
    case token_id::EOL:
        retval = "\\n";
        break;
    case token_id::WRD:
        retval = to_word();
        break;
    case token_id::NUM:
        retval = to_string(to_num());
        break;
    case token_id::SYM:
        retval = map().at(to_symbol());
        break;
    }
    return retval;
}
