#include "parser/Parser.h"
#include "ast/ExpressionStatement.hpp"
#include "ast/IntegerExpression.hpp"
#include "ast/InfixExpression.hpp"
#include "ast/PrefixExpression.hpp"
#include "ast/BoolExpression.hpp"
#include "ast/IfExpression.hpp"
#include "ast/FunctionExpression.hpp"
#include "ast/CallExpression.hpp"
#include "ast/StringExpression.hpp"
#include "ast/AssignExpression.hpp"
#include <memory>

namespace li
{


Parser::Parser(const shared_ptr<Lexer>& lexer) :
    _lexer(lexer)
{
    parse_token();
    parse_token();
}

void Parser::parse_token()
{
    _current = _next;
    _next = _lexer->parseToken();
}

Parser::PrecedenceType Parser::get_precedence(Token::Type type)
{
    auto it = _tokenTypePrecedences.find(type);
    if (it == _tokenTypePrecedences.end())
    {
        return Lowest;
    }
    return it->second;
}

string Parser::pos_string() const
{
	stringstream stream;
	stream << "char " << _lexer->pos() << ": ";
	return stream.str();
}

void Parser::no_prefix_parse_fun_error(Token::Type type)
{
	stringstream stream;
	stream << pos_string() << "error: no prefix parse function for " << Token::typeName(type);
	_outputs.push_back(stream.str());
}

bool Parser::expect_token_type(Token::Type type)
{
	if (_current->type() == type)
	{
		return false;
	}

	stringstream stream;
	stream << pos_string() << "error: expected token type to be " << Token::typeName(type) << ", but got " << _current->typeName();
	_outputs.push_back(stream.str());
	return true;
}

void Parser::integer_parse_error()
{
	stringstream stream;
	stream << pos_string() << "invalid integer: " << _current->literal();
	_outputs.push_back(stream.str());
}

void Parser::assign_operand_type_error(const string& id)
{
	stringstream buffer;
	buffer << "error - assign operand type error: " << id;
	_outputs.push_back(buffer.str());
}

shared_ptr<Program> Parser::parseProgram()
{
    auto program = make_shared<Program>(_current);
	while (_current->type() != Token::Eof)
	{
		auto statement = parse_statement();
		if (!statement)
		{
			continue;
		}

		program->addStatement(statement);
	}
	return program;
}

shared_ptr<Statement> Parser::parse_statement()
{
    switch (_current->type())
	{

	case Token::Semicolon:
		parse_token();
		return nullptr;

	case Token::Let:
		return parse_let_statement();

	case Token::Return:
		return parse_return_statement();

	default:
		return parse_expression_statement();

	}
}

shared_ptr<Statement> Parser::parse_expression_statement()
{
    auto statement = make_shared<ExpressionStatement>(_current);
	statement->setExpression(parse_expression(Lowest));
	return statement;
}

shared_ptr<Expression> Parser::parse_expression(PrecedenceType precedence)
{
    auto prefixFun = _prefixParseFuns.find(_current->type());
	if (prefixFun == _prefixParseFuns.end())
	{
		no_prefix_parse_fun_error(_current->type());
		parse_token();
		return nullptr;
	}

	auto leftExpr = prefixFun->second();
	while (precedence < get_precedence(_current->type()))
	{
		auto infixFun = _infixParseFuns.find(_current->type());
		if (infixFun == _infixParseFuns.end())
		{
			// the infix parse function is not found, it is a prefix expression
			return leftExpr;
		}
		leftExpr = infixFun->second(leftExpr);
	}
	return leftExpr;
}

shared_ptr<LetStatement> Parser::parse_let_statement()
{
    auto statement = make_shared<LetStatement>(_current);
	parse_token();
	if (expect_token_type(Token::Identifier)) return nullptr;
	statement->setName(dynamic_pointer_cast<IdentifierExpression>(parse_identifier()));
	
	if(expect_token_type(Token::Assign)) return nullptr;

	parse_token();
	statement->setValue(parse_expression(Lowest));

	return statement;
}

shared_ptr<ReturnStatement> Parser::parse_return_statement()
{
	auto statement = make_shared<ReturnStatement>(_current);
	
	parse_token();
	statement->setValue(parse_expression(Lowest));

	return statement;
}

shared_ptr<Expression> Parser::parse_intger()
{
	auto expr = make_shared<IntegerExpression>(_current);
	try
	{
		expr->setValue(stoull(_current->literal()));
		parse_token();
		return expr;
	}
	catch (exception&)
	{
		integer_parse_error();
		return nullptr;
	}
}

shared_ptr<Expression> Parser::parse_identifier()
{
	auto expr = make_shared<IdentifierExpression>(_current);
	expr->setValue(_current->literal());

	parse_token();
	return expr;
}

shared_ptr<Expression> Parser::parse_infix(const shared_ptr<Expression>& left)
{
	auto expr = make_shared<InfixExpression>(_current);
	expr->setLeft(left);
	expr->setOperator(_current->literal());

	auto cur = get_precedence(_current->type());
	parse_token();
	expr->setRight(parse_expression(cur));
	return expr;
}

shared_ptr<Expression> Parser::parse_prefix()
{
	auto expr = make_shared<PrefixExpression>(_current);
	expr->setOperator(_current->literal());
	
	parse_token();
	expr->setRight(parse_expression(Prefix));
	return expr;
}

shared_ptr<Expression> Parser::parse_group()
{
	parse_token();
	auto expr = parse_expression(Lowest);
	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();
	return expr;
}

shared_ptr<Expression> Parser::parse_bool()
{
	auto expr = make_shared<BoolExpression>(_current);
	expr->setValue(_current->type() == Token::True);
	
	parse_token();
	return expr;
}

shared_ptr<BlockStatement> Parser::parse_block_statement()
{
	auto block = make_shared<BlockStatement>(_current);
	if (expect_token_type(Token::LBrace)) return nullptr;
	parse_token();

	while (_current->type() != Token::RBrace && _current->type() != Token::Eof)
	{
		auto temp = parse_statement();
		if (!temp)
		{
			continue;
		}
		block->addStatement(temp);
	}

	if (expect_token_type(Token::RBrace)) return nullptr;
	parse_token();
	return block;
}

shared_ptr<Expression> Parser::parse_if()
{
	auto expr = make_shared<IfExpression>(_current);

	parse_token();
	if (expect_token_type(Token::LParen)) return nullptr;
	parse_token();

	expr->setCondition(parse_expression(Lowest));
	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();

	expr->setConsequence(parse_block_statement());

	if (_current->type() == Token::Else)
	{
		parse_token();
		expr->setAlternative(parse_block_statement());
	}
	return expr;
}

shared_ptr<ArgumentsStatement> Parser::parse_arguments()
{
	auto args = make_shared<ArgumentsStatement>(_current);
	if (expect_token_type(Token::LParen)) return nullptr;
	parse_token();

	while (_current->type() != Token::RParen && _current->type() != Token::Eof)
	{
		args->addArg(dynamic_pointer_cast<IdentifierExpression>(parse_identifier()));

		if (_current->type() == Token::Comma)
		{
			parse_token();
		}
	}

	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();
	return args;
}

shared_ptr<Expression> Parser::parse_function()
{
	auto fun = make_shared<FunctionExpression>(_current);

	parse_token();
	fun->setArgs(parse_arguments());
	fun->setBody(parse_block_statement());
	return fun;
}

shared_ptr<ExpressionsStatement> Parser::parse_expressions()
{
	auto exprs = make_shared<ExpressionsStatement>(_current);
	if (expect_token_type(Token::LParen)) return nullptr;
	parse_token();

	while (_current->type() != Token::RParen && _current->type() != Token::Eof)
	{
		exprs->addExpression(parse_expression(Lowest));

		if (_current->type() == Token::Comma)
		{
			parse_token();
		}
	}

	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();
	return exprs;
}

shared_ptr<Expression> Parser::parse_call(const shared_ptr<Expression>& fun)
{
	auto call = make_shared<CallExpression>(_current);
	call->setFun(fun);
	call->setExprs(parse_expressions());
	return call;
}

shared_ptr<Expression> Parser::parse_assign(const shared_ptr<Expression>& id)
{
	auto expr = make_shared<AssignExpression>(_current);
	if (id->type() != Node::Type::Identifier)
	{
		assign_operand_type_error(id->toString());
		parse_token();
		return nullptr;
	}

	expr->setId(id);
	parse_token();
	expr->setValue(parse_expression(Lowest));
	return expr;
}

shared_ptr<Expression> Parser::parse_string()
{
	auto expr = make_shared<StringExpression>(_current);
	expr->setValue(_current->literal());

	parse_token();
	return expr;
}

}
