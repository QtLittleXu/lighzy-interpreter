#include "lexer/Token.h"

namespace li
{


const map<Token::Type, string> Token::_typeNames =
{
	{ Illegal,			"illegal"},
	{ Eof,				"eof"},
	{ Identifier,		"identifier"},
	{ Semicolon,		"semicolon(\";\")"},
	{ Comma,			"comma(\",\")"},
	{ LParen,			"left_parenthese(\"(\")"},
	{ RParen,			"right_parenthes(\")\")"},
	{ LBrace,			"left_brace(\"{\")"},
	{ RBrace,			"right_brace(\"}\")"},

	{ Integer,			"integer" },
	{ String,			"string" },

	{ Equal,			"equal(\"==\")"},
	{ NotEqual,			"not_equal(\"!=\")"},
	{ Greater,			"greater_sign(\">\")"},
	{ Less,				"less_sign(\"<\")"},
	{ LogicalNegation,	"logical_negation(\"!\")"},
	{ Assign,			"assign(\"=\")"},
	{ Plus,				"plus(\"+\")"},
	{ Minus,			"minus(\"-\")"},
	{ Asterisk,			"asterisk(\"*\")"},
	{ Slash,			"slash(\"/\")"},

	{ Let,				"let" },
	{ Fun,				"fun" },
	{ True,				"true" },
	{ False,			"false" },
	{ If,				"if" },
	{ Else,				"else" },
	{ Return,			"return" }
};


}