#pragma once

#include <exception_helper.h>
#include <tokenizer.h>

#include <memory>

DECL_EXCEPT_(invalid_define_syntax)

namespace calc {

enum class keyword_id { NIL, DECLARE, CONST, HELP };

struct calc_core {
    using key_map_t = std::map<std::string, keyword_id>;

    static constexpr auto help_filename = "help.txt";

    int exec();

    std::ostream& os() const;
    std::istream& is() const;
    calc_core& set_ostream(std::ostream& p_os);
    calc_core& set_istream(std::istream& p_is);

    static calc_core& instance();
    static const key_map_t& map();

private:
    explicit calc_core();

    void declare(keyword_id p_key);
    void help() const;

    static keyword_id get_key(const std::string& p_str);

    static const std::unique_ptr<calc_core> _inst;
    static const key_map_t _map;

    tok::tokenizer tok;
    std::ostream* _os;
};
}
