#include "lexer/Lexer.h"
#include <gtest/gtest.h>

namespace li::test
{


TEST(LexerTest, parseToken)
{
    Lexer lexer("@identifierString;,(){}12345[]\"Hello world!\"977.21= == != > >= < <=!+-*/% += -= *= /= %=++--let fun true false if else return while");

	struct
	{
		li::Token::Type expectedToken;
		string expectedLiteral;
	} tests[] = {
		{ Token::Illegal,			"@" },
		{ Token::Identifier,		"identifierString" },
        { Token::Semicolon,			";" },
        { Token::Comma,				"," },
        { Token::LParen,			"(" },
        { Token::RParen,			")" },
        { Token::LBrace,			"{" },
        { Token::RBrace,			"}" },
        { Token::Integer,			"12345" },
		{ Token::LBracket,			"[" },
		{ Token::RBracket,			"]" },
		{ Token::String,			"Hello world!" },
		{ Token::Float,				"977.21" },
        { Token::Assign,			"=" },
		{ Token::Equal,				"==" },
		{ Token::NotEqual,			"!=" },
        { Token::Greater,			">" },
        { Token::GreaterEqual,		">=" },
        { Token::Less,				"<" },
        { Token::LessEqual,			"<=" },
        { Token::LogicalNegation,	"!" },
        { Token::Plus,				"+" },
        { Token::Minus,				"-"},
        { Token::Asterisk,			"*" },
        { Token::Slash,				"/" },
		{ Token::Modulus,			"%" },
		{ Token::AddAssign,			"+=" },
		{ Token::SubAssign,			"-=" },
		{ Token::MulAssign,			"*=" },
		{ Token::DivAssign,			"/=" },
		{ Token::ModulusAssign,		"%=" },
		{ Token::Increment,			"++" },
		{ Token::Decrement,			"--" },
		{ Token::Let,				"let" },
		{ Token::Fun,				"fun" },
		{ Token::True,				"true" },
		{ Token::False,				"false" },
		{ Token::If,				"if" },
		{ Token::Else,				"else" },
		{ Token::Return,			"return" },
		{ Token::While,				"while" },
		{ Token::Eof,				"" }
    };
	int i = 0;
	for (const auto& [expectedType, expectedLiteral] : tests)
	{
		auto token = lexer.parseToken();
		auto type = token->type;
		auto literal = token->literal;

		EXPECT_EQ(type, expectedType);
		EXPECT_EQ(literal, expectedLiteral);
		i++;
	}
}


}