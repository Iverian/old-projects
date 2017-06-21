#pragma once

#include <union_t.h>
#include <token_id.h>
#include <basic_symbol_mapper.h>

namespace tok {

	using symbol_mapper = basic_symbol_mapper<symbol_id>;

struct token {
    using data_t = util::union_t<symbol_id, double, std::string>;

    token(const token_id& p_id = token_id::NIL);

    token(const double& p_number);
    token(const std::string& p_word);
    token(std::string&& p_word) noexcept;
    token(const symbol_id& p_symbol_id);

    bool is_valid() const;
    token& make_invalid(const std::string& p_invalid_symbols);
    std::string invalid_token_data() const;

    const token_id& id() const;
	bool eof() const;
	bool eol() const;
	bool empty() const;

	double to_num() const;
	std::string to_word() const;
	symbol_id to_symbol() const;
	std::string raw() const;
	static const symbol_mapper& map();

private:
    token_id _id;
    data_t _dat;

	static const symbol_mapper _mapper;
};

}
