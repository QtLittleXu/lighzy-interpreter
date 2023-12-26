#pragma once

#include "lexer/Lexer.h"
#include "ast/Program.hpp"
#include "ast/LetStat.hpp"
#include "ast/ReturnStat.hpp"
#include "ast/BlockStat.hpp"
#include "ast/ArgumentsStat.hpp"
#include "ast/ExpressionsStat.hpp"
#include <functional>

namespace li
{


class Parser
{
public:
	using prefix_parse_fun = function<shared_ptr<Expr>()>;
	using infix_parse_fun = function<shared_ptr<Expr>(const shared_ptr<Expr>&)>;
	enum PrecedenceType
	{
		Lowest, Equals, LessGreater, Sum, Product, Assign, Prefix, Call
	};

public:
	Parser(const shared_ptr<Lexer>& lexer);
	shared_ptr<Program> parseProgram();

public:
	const vector<string>& outputs() const
	{
		return _outputs;
	}

private:
	void parse_token();
	PrecedenceType get_precedence(Token::Type type);
	string pos_string() const;
	void no_prefix_parse_fun_error(Token::Type type);
	bool expect_token_type(Token::Type type);
	void integer_parse_error();
	void assign_operand_type_error(const string& id);

private:
	// functions of parsing
	shared_ptr<Stat> parse_statement();
	shared_ptr<Stat> parse_expr_stat();
	shared_ptr<LetStat> parse_let_stat();
	shared_ptr<ReturnStat> parse_return_stat();
	shared_ptr<BlockStat> parse_block_stat();

	shared_ptr<ArgumentsStat> parse_args();
	shared_ptr<ExpressionsStat> parse_exprs();

	shared_ptr<Expr> parse_expr(PrecedenceType precedence);
	shared_ptr<Expr> parse_intger();
	shared_ptr<Expr> parse_identifier();
	shared_ptr<Expr> parse_prefix();
	shared_ptr<Expr> parse_group();
	shared_ptr<Expr> parse_bool();
	shared_ptr<Expr> parse_if();
	shared_ptr<Expr> parse_function();
	shared_ptr<Expr> parse_string();

	shared_ptr<Expr> parse_infix(const shared_ptr<Expr>& left);
	shared_ptr<Expr> parse_call(const shared_ptr<Expr>& fun);
	shared_ptr<Expr> parse_assign(const shared_ptr<Expr>& id);

private:
	shared_ptr<Lexer> _lexer;
	shared_ptr<Token> _current;
	shared_ptr<Token> _next;
	vector<string> _outputs;

	const map<Token::Type, prefix_parse_fun> _prefixParseFuns = {
		{ Token::Integer,			bind(&Parser::parse_intger, this) },
		{ Token::Identifier,		bind(&Parser::parse_identifier, this) },
		{ Token::LogicalNegation,	bind(&Parser::parse_prefix, this) },
		{ Token::LParen,			bind(&Parser::parse_group, this) },
		{ Token::Minus,				bind(&Parser::parse_prefix, this) },
		{ Token::True,				bind(&Parser::parse_bool, this) },
		{ Token::False,				bind(&Parser::parse_bool, this) },
		{ Token::If,				bind(&Parser::parse_if, this) },
		{ Token::String,			bind(&Parser::parse_string, this) },
		{ Token::Fun,				bind(&Parser::parse_function, this) }
	};

	const map<Token::Type, infix_parse_fun> _infixParseFuns = {
		{ Token::Plus,			bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::Minus,			bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::Asterisk,		bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::Slash,			bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::Equal,			bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::NotEqual,		bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::Greater,		bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::GreaterEqual,	bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::Less,			bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::LessEqual,		bind(&Parser::parse_infix, this, placeholders::_1) },
		{ Token::LParen,		bind(&Parser::parse_call, this, placeholders::_1) },
		{ Token::Assign,		bind(&Parser::parse_assign, this, placeholders::_1) },
	};

	const map<Token::Type, PrecedenceType> _tokenTypePrecedences = {
		{ Token::Equal,				Equals },
		{ Token::NotEqual,			Equals },
		{ Token::Plus,				Sum },
		{ Token::Minus,				Sum },
		{ Token::Asterisk,			Product },
		{ Token::Slash,				Product },
		{ Token::LogicalNegation,	Prefix },
		{ Token::Greater,			LessGreater },
		{ Token::GreaterEqual,		LessGreater },
		{ Token::Less,				LessGreater },
		{ Token::LessEqual,			LessGreater },
		{ Token::LParen,			Call },
		{ Token::Assign,			Assign }
	};
};


}
