#include "lexer/Token.h"

namespace li
{


const map<Token::Type, string> Token::typeNames =
{
	{ Illegal,			"illegal"},
	{ Eof,				"eof"},
	{ Identifier,		"identifier"},
	{ Semicolon,		"semicolon(\";\")"},
	{ Comma,			"comma(\",\")"},
	{ LParen,			"left_parenthese(\"(\")"},
	{ RParen,			"right_parenthese(\")\")"},
	{ LBrace,			"left_brace(\"{\")"},
	{ RBrace,			"right_brace(\"}\")"},
	{ DoubleQuotes,		"double_quotes(\")" },
	{ LBracket,			"left_bracket(\"[\")" },
	{ RBracket,			"right_bracket(\"]\")" },

	{ Integer,			"integer" },
	{ String,			"string" },
	{ Float,			"float" },

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
	{ Modulus,			"modulus(\"%\")" },
	{ AddAssign,		"addition_assign(\"+=\")" },
	{ SubAssign,		"subtraction_assign(\"-=\")" },
	{ MulAssign,		"multiplication_assign(\"*=\")" },
	{ DivAssign,		"division_assign(\"/=\")" },
	{ ModulusAssign,	"modulus_assign(\"%=\")" },
	{ Increment,		"increment(\"++\")" },
	{ Decrement,		"decrement(\"--\")" },

	{ Let,				"let" },
	{ Fun,				"fun" },
	{ True,				"true" },
	{ False,			"false" },
	{ If,				"if" },
	{ Else,				"else" },
	{ Return,			"return" },
	{ While,			"while" }
};


}