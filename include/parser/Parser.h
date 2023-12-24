#pragma once

#include "lexer/Lexer.h"
#include "ast/basic/Expression.hpp"
#include "ast/Program.hpp"
#include "ast/LetStatement.hpp"
#include "ast/ReturnStatement.hpp"
#include "ast/BlockStatement.hpp"
#include "ast/ArgumentsStatement.hpp"
#include "ast/ExpressionsStatement.hpp"
#include <functional>

namespace li
{


class Parser
{
public:
	using prefix_parse_fun = function<shared_ptr<Expression>()>;
	using infix_parse_fun = function<shared_ptr<Expression>(const shared_ptr<Expression>&)>;
	enum PrecedenceType
	{
		Lowest, Equals, LessGreater, Sum, Product, Prefix, Call
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

private:
	// functions of parsing
	shared_ptr<Statement> parse_statement();
	shared_ptr<Statement> parse_expression_statement();
	shared_ptr<LetStatement> parse_let_statement();
	shared_ptr<ReturnStatement> parse_return_statement();
	shared_ptr<BlockStatement> parse_block_statement();

	shared_ptr<ArgumentsStatement> parse_arguments();
	shared_ptr<ExpressionsStatement> parse_expressions();

	shared_ptr<Expression> parse_expression(PrecedenceType precedence);
	shared_ptr<Expression> parse_intger();
	shared_ptr<Expression> parse_identifier();
	shared_ptr<Expression> parse_prefix();
	shared_ptr<Expression> parse_group();
	shared_ptr<Expression> parse_bool();
	shared_ptr<Expression> parse_if();
	shared_ptr<Expression> parse_function();
	shared_ptr<Expression> parse_string();

	shared_ptr<Expression> parse_infix(const shared_ptr<Expression>& left);
	shared_ptr<Expression> parse_call(const shared_ptr<Expression>& fun);

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
		{ Token::LParen,		bind(&Parser::parse_call, this, placeholders::_1) }
	};

	const map<Token::Type, PrecedenceType> _tokenTypePrecedences = {
		{ Token::Equal,Equals },
		{ Token::NotEqual,Equals },
		{ Token::Plus,Sum },
		{ Token::Minus,Sum },
		{ Token::Asterisk,Product },
		{ Token::Slash,Product },
		{ Token::LogicalNegation,Prefix },
		{ Token::Greater,LessGreater },
		{ Token::GreaterEqual,LessGreater },
		{ Token::Less,LessGreater },
		{ Token::LessEqual,LessGreater },
		{ Token::LParen,Call }
	};
};


}
