#include <tokenizer.h>

#include <cctype>

using namespace tok;
using namespace std;
using namespace util;

string tok::get_word(istream& p_is)
{
    char c;
    string retval;
    while (p_is.get(c) && !std::isalnum(c))
        ;
    if (p_is)
        for (retval += c; p_is.get(c) && std::isalpha(c);)
            retval += c;
    p_is.putback(c);
    return retval;
}

double tok::get_number(istream& p_is)
{
    auto retval = 0.;
    p_is >> retval;
    return retval;
}

tokenizer::tokenizer(istream& p_stream)
    : _tok()
    , _is(&p_stream)
	, _putback_flag(false)
{
}

tokenizer::tokenizer(const tokenizer& p_that)
	: _tok(p_that._tok)
	, _is(p_that._is)
	, _putback_flag(p_that._putback_flag)
{
}

istream& tokenizer::is() const
{
    return *_is;
}

tokenizer& tokenizer::set_istream(istream& p_is)
{
    _is = &p_is;
    return *this;
}

void tokenizer::putback()
{
    _putback_flag = true;
}

void tokenizer::reset_token()
{
    _tok = token();
}

void tokenizer::skip_line()
{
    for (; is() && _tok.id() != token_id::EOL; next())
        ;
}

const token& tokenizer::next()
{
    if (!_putback_flag) {
        char c;
        while (is().get(c) && c != '\n' && isspace(c))
            ;
        if (!_is)
            _tok = token_id::END;
        else if (c == '\n')
            _tok = token_id::EOL;
        else {
            _is->putback(c);
            if (isdigit(c))
                _tok = get_number(*_is);
            else if (isalpha(c))
                _tok = get_word(*_is);
            else {
                auto symbol = token::map().get_symbol(*_is);
                switch (symbol.first) {
                case symbol_id::TIE:
                    while (*_is && next().id() == token_id::EOL)
                        ;
                    break;
                case symbol_id::NIL:
                    _tok.make_invalid(string(1, symbol.second));
                    break;
                default:
                    _tok = symbol.first;
                    break;
                }
            }
        }
    } else
        _putback_flag = false;
    return _tok;
}

const token& tokenizer::cur() const
{
    return _tok;
}
