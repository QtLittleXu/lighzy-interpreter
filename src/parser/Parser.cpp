#include "parser/Parser.h"
#include "ast/ExpressionStat.hpp"
#include "ast/IntegerExpr.hpp"
#include "ast/InfixExpr.hpp"
#include "ast/PrefixExpr.hpp"
#include "ast/BoolExpr.hpp"
#include "ast/IfExpr.hpp"
#include "ast/FunctionExpr.hpp"
#include "ast/CallExpr.hpp"
#include "ast/StringExpr.hpp"
#include "ast/AssignExpr.hpp"
#include "ast/FloatExpr.hpp"
#include "ast/ArrayExpr.hpp"
#include "ast/IndexExpr.hpp"

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
	if (_current->type == type)
	{
		return false;
	}

	stringstream stream;
	stream << pos_string() << "error: expected token type to be " << Token::typeName(type) << ", but got " << _current->typeName();
	_outputs.push_back(stream.str());
	return true;
}

void Parser::parse_number_error(const string& msg)
{
	stringstream buffer;
	buffer << pos_string() << "error: parse number failed, " << msg;
	_outputs.push_back(buffer.str());
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
	while (_current->type != Token::Eof)
	{
		auto stat = parse_statement();
		if (!stat)
		{
			continue;
		}

		program->statements.push_back(stat);
	}
	return program;
}

shared_ptr<Stat> Parser::parse_statement()
{
    switch (_current->type)
	{

	case Token::Semicolon:
		parse_token();
		return nullptr;

	case Token::Let:
		return parse_let_stat();

	case Token::Return:
		return parse_return_stat();

	case Token::While:
		return parse_while_stat();

	default:
		return parse_expr_stat();

	}
}

shared_ptr<Stat> Parser::parse_expr_stat()
{
    auto stat = make_shared<ExpressionStat>(_current);
	stat->expression = parse_expr(Lowest);
	return stat;
}

shared_ptr<WhileStat> Parser::parse_while_stat()
{
	auto stat = make_shared<WhileStat>(_current);
	parse_token();
	if (expect_token_type(Token::LParen)) return nullptr;
	parse_token();

	stat->condition = parse_expr(Lowest);
	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();

	stat->body = parse_block_stat();
	return stat;
}

shared_ptr<Expr> Parser::parse_expr(PrecedenceType precedence)
{
    auto prefixFun = _prefixParseFuns.find(_current->type);
	if (prefixFun == _prefixParseFuns.end())
	{
		no_prefix_parse_fun_error(_current->type);
		parse_token();
		return nullptr;
	}

	auto leftExpr = prefixFun->second();
	while (precedence < get_precedence(_current->type))
	{
		auto infixFun = _infixParseFuns.find(_current->type);
		if (infixFun == _infixParseFuns.end())
		{
			// the infix parse function is not found, it is a prefix Expr
			return leftExpr;
		}
		leftExpr = infixFun->second(leftExpr);
	}
	return leftExpr;
}

shared_ptr<LetStat> Parser::parse_let_stat()
{
    auto stat = make_shared<LetStat>(_current);
	parse_token();
	if (expect_token_type(Token::Identifier)) return nullptr;
	stat->name = dynamic_pointer_cast<IdentifierExpr>(parse_identifier());
	
	if(expect_token_type(Token::Assign)) return nullptr;

	parse_token();
	stat->value = parse_expr(Lowest);

	return stat;
}

shared_ptr<ReturnStat> Parser::parse_return_stat()
{
	auto stat = make_shared<ReturnStat>(_current);
	
	parse_token();
	stat->value = parse_expr(Lowest);

	return stat;
}

shared_ptr<Expr> Parser::parse_number()
{
	try
	{
		switch (_current->type)
		{

		case Token::Integer:
		{
			auto expr = make_shared<IntegerExpr>(_current);
			expr->value = stoull(_current->literal);
			parse_token();
			return expr;
		}

		case Token::Float:
		{
			auto expr = make_shared<FloatExpr>(_current);
			expr->value = stod(_current->literal);
			parse_token();
			return expr;
		}

		default:
			parse_number_error(_current->literal + " was not Token::Integer or Token::Float");
			parse_token();
			return nullptr;

		}
	}
	catch (invalid_argument&)
	{
		parse_number_error(_current->literal + " was not an number");
		parse_token();
		return nullptr;
	}
	catch (out_of_range&)
	{
		parse_number_error(_current->literal + " was out of range of uint64");
		parse_token();
		return nullptr;
	}
}

shared_ptr<Expr> Parser::parse_identifier()
{
	auto expr = make_shared<IdentifierExpr>(_current);
	expr->value = _current->literal;

	parse_token();
	return expr;
}

shared_ptr<Expr> Parser::parse_infix(const shared_ptr<Expr>& left)
{
	auto expr = make_shared<InfixExpr>(_current);
	expr->left = left;
	expr->operatorName = _current->literal;

	auto cur = get_precedence(_current->type);
	parse_token();
	expr->right = parse_expr(cur);
	return expr;
}

shared_ptr<Expr> Parser::parse_prefix()
{
	auto expr = make_shared<PrefixExpr>(_current);
	expr->operatorName = _current->literal;
	
	parse_token();
	expr->right = parse_expr(Prefix);
	return expr;
}

shared_ptr<Expr> Parser::parse_group()
{
	parse_token();
	auto expr = parse_expr(Lowest);
	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();
	return expr;
}

shared_ptr<Expr> Parser::parse_bool()
{
	auto expr = make_shared<BoolExpr>(_current);
	expr->value = _current->type == Token::True;
	
	parse_token();
	return expr;
}

shared_ptr<BlockStat> Parser::parse_block_stat()
{
	auto block = make_shared<BlockStat>(_current);
	if (expect_token_type(Token::LBrace)) return nullptr;
	parse_token();

	while (_current->type != Token::RBrace && _current->type != Token::Eof)
	{
		auto temp = parse_statement();
		if (!temp)
		{
			continue;
		}
		block->statements.push_back(temp);
	}

	if (expect_token_type(Token::RBrace)) return nullptr;
	parse_token();
	return block;
}

shared_ptr<Expr> Parser::parse_if()
{
	auto expr = make_shared<IfExpr>(_current);

	parse_token();
	if (expect_token_type(Token::LParen)) return nullptr;
	parse_token();

	expr->condition = parse_expr(Lowest);
	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();

	expr->consequence = parse_block_stat();

	if (_current->type == Token::Else)
	{
		parse_token();
		expr->alternative = parse_block_stat();
	}
	return expr;
}

shared_ptr<ArgumentsStat> Parser::parse_args()
{
	auto args = make_shared<ArgumentsStat>(_current);
	if (expect_token_type(Token::LParen)) return nullptr;
	parse_token();

	while (_current->type != Token::RParen && _current->type != Token::Eof)
	{
		args->args.push_back(dynamic_pointer_cast<IdentifierExpr>(parse_identifier()));

		if (_current->type == Token::Comma)
		{
			parse_token();
		}
	}

	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();
	return args;
}

shared_ptr<Expr> Parser::parse_function()
{
	auto fun = make_shared<FunctionExpr>(_current);

	parse_token();
	fun->args = parse_args();
	fun->body = parse_block_stat();
	return fun;
}

shared_ptr<ExpressionsStat> Parser::parse_exprs(Token::Type end)
{
	auto exprs = make_shared<ExpressionsStat>(_current);

	if (_current->type == end)
	{
		return exprs;
	}

	exprs->expressions.push_back(parse_expr(Lowest));

	while (_current->type == Token::Comma && _current->type != Token::Eof)
	{
		if (_current->type == Token::Comma)
		{
			parse_token();
		}

		exprs->expressions.push_back(parse_expr(Lowest));
	}

	return exprs;
}

shared_ptr<Expr> Parser::parse_call(const shared_ptr<Expr>& fun)
{
	auto expr = make_shared<CallExpr>(_current);
	expr->fun = fun;
	parse_token();

	expr->exprs = parse_exprs(Token::RParen);
	if (expect_token_type(Token::RParen)) return nullptr;
	parse_token();
	return expr;
}

shared_ptr<Expr> Parser::parse_assign(const shared_ptr<Expr>& id)
{
	auto expr = make_shared<AssignExpr>(_current);

	expr->id = id;
	parse_token();
	expr->value = parse_expr(Lowest);
	return expr;
}

shared_ptr<Expr> Parser::parse_string()
{
	auto expr = make_shared<StringExpr>(_current);
	if (_current->literal == "\n")
	{
		parse_token();
		expect_token_type(Token::DoubleQuotes);
		return nullptr;
	}

	expr->value = _current->literal;
	parse_token();
	return expr;
}

shared_ptr<Expr> Parser::parse_array()
{
	auto expr = make_shared<ArrayExpr>(_current);
	parse_token();

	expr->elements = parse_exprs(Token::RBracket);
	if (expect_token_type(Token::RBracket)) return nullptr;
	parse_token();
	return expr;
}

shared_ptr<Expr> Parser::parse_index(const shared_ptr<Expr>& left)
{
	auto expr = make_shared<IndexExpr>(_current);
	expr->left = left;
	parse_token();
	
	expr->index = parse_expr(Lowest);
	if (expect_token_type(Token::RBracket)) return nullptr;
	parse_token();

	return expr;
}


}
