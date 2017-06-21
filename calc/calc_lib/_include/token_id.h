#pragma once

namespace tok
{	

enum class token_id {
    NIL,
    END,
	EOL,
    NUM,
    WRD,
    SYM
};

enum class symbol_id {
    NIL,

    LPR,
    RPR,
    ASG,
    COM,
	TIE,

	DEF,
	DEF_CONST,
	GLOB,
	GLOB_CONST,
	DROP,

    SUM,
    MIN,
    MUL,
    DIV,
    MOD,
    POW, // = "**"

    IF,
    ELSE,

    EQ, // = "=="
    NEQ, // = "!="
    LESS,
    MORE,
    LESS_EQ, // = "<="
    MORE_EQ, // = ">="
    AND,
    OR,
    XOR,
    NOT,
};

}