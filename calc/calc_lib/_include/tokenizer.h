#pragma once

#include <token.h>

#include <istream>

namespace tok {

	std::string get_word(std::istream& p_is);
	double get_number(std::istream& p_is);

struct tokenizer {
    explicit tokenizer(std::istream& p_stream = std::cin);
	tokenizer(const tokenizer& p_that);

    std::istream& is() const; 
	tokenizer& set_istream(std::istream& p_is);

	void putback();
    void reset_token();
	void skip_line();
    const token& next();
    const token& cur() const;

private:
    token _tok;
    std::istream* _is;
	bool _putback_flag;
};

}
