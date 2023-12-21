#include "lexer/Lexer.h"
#include <gtest/gtest.h>

namespace li::test
{


TEST(LexerTest, parseToken)
{
    Lexer lexer("@ identifierString ; , () {} 12345 = == != > >= < <= ! + - * / let fun true false if else return");

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
		{ Token::Let,				"let" },
		{ Token::Fun,				"fun" },
		{ Token::True,				"true" },
		{ Token::False,				"false" },
		{ Token::If,				"if" },
		{ Token::Else,				"else" },
		{ Token::Return,			"return" },
		{ Token::Eof,				"" }
    };
	int i = 0;
	for (const auto& [expectedType, expectedLiteral] : tests)
	{
		auto token = lexer.parseToken();
		auto type = token->type();
		auto literal = token->literal();

		EXPECT_EQ(type, expectedType);
		EXPECT_EQ(literal, expectedLiteral);
		i++;
	}
}


}