#include <calc_core.h>
#include <expr_context.h>
#include <utility.h>
#include "debug.h"

#include <fstream>

using namespace calc;
using namespace std;
using namespace tok;
using namespace expr;
using namespace util;
using namespace context;

const unique_ptr<calc_core> calc_core::_inst(new calc_core());

const calc_core::key_map_t calc_core::_map({{ "decl", keyword_id::DECLARE }, { "const", keyword_id::CONST }, { "help", keyword_id::HELP },});

int calc_core::exec()
{
    while (!tok.next().eof()) {
        try {
            auto key = get_key(tok.cur().to_word());
            switch (key) {
            case keyword_id::DECLARE:
            case keyword_id::CONST:
                declare(key);
                tok.skip_line();
                break;
            case keyword_id::HELP:
                help();
                tok.skip_line();
                break;
			default: {
				auto cntx = expr_context::global().make_context();
				tok.putback();
				os() << "\t" << expression(tok)(cntx).str() << endl;
				break;
			}
            }
        }
        CATCH(exc)
        {
            os() << exc.what() << endl;
            tok.skip_line();
        }
    }
    return 0;
}

#define CHECK_DECL_ERR(cond, expected) CHECK_IF((cond), err::invalid_define_syntax, "expected " + std::string(expected) + "but received: " + tok.cur().raw())

void calc_core::declare(keyword_id p_key)
{
    string expr_name;
    vector<string> argname;
    expression expr;
	auto is_const = false;

	if (p_key == keyword_id::DECLARE)
		is_const = get_key(tok.next().to_word()) == keyword_id::CONST;
	else if (p_key == keyword_id::CONST)
		is_const = true;

	if (is_const)
		tok.next();
    CHECK_DECL_ERR(tok.cur().id() != token_id::WRD, "expression_id");

    expr_name = tok.cur().to_word();
    if (tok.next().to_symbol() == symbol_id::LPR) {
        while (!tok.next().empty() && tok.cur().to_symbol() != symbol_id::RPR) {
			if (tok.cur().to_symbol() == symbol_id::COM)
				continue;
            CHECK_DECL_ERR(tok.cur().id() != token_id::WRD, "argument_id");
            argname.push_back(tok.cur().to_word());
        }
        CHECK_DECL_ERR(tok.cur().to_symbol() != symbol_id::RPR, ")");
        tok.next();
    }

	tok.putback();
    expr = expression(tok, argname);

    expr_context::global().insert(expr_name, expr, is_const);
}

void calc_core::help() const
{
    ifstream help(help_filename);
    echo(help, os());
}

keyword_id calc_core::get_key(const string& p_str)
{
    auto iter = _map.find(p_str);
    return iter == _map.end() ? keyword_id::NIL : iter->second;
}

ostream& calc_core::os() const
{
    return *_os;
}

istream& calc_core::is() const
{
    return tok.is();
}

calc_core& calc_core::set_ostream(ostream& p_os)
{
    _os = &p_os;
    return *this;
}

calc_core& calc_core::set_istream(istream& p_is)
{
    tok = tokenizer(p_is);
    return *this;
}

calc_core& calc_core::instance()
{
    return *_inst;
}

const calc_core::key_map_t& calc_core::map()
{
    return _map;
}

calc_core::calc_core()
    : tok(cin)
    , _os(&cout)
{
}
