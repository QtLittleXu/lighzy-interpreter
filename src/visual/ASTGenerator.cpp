#include "visual/ASTGenerator.h"
#include "ast/ExpressionStatement.hpp"
#include "ast/InfixExpression.hpp"
#include "ast/IntegerExpression.hpp"
#include "ast/LetStatement.hpp"
#include "ast/PrefixExpression.hpp"
#include "ast/FunctionExpression.hpp"
#include "ast/Program.hpp"
#include "ast/BoolExpression.hpp"
#include "ast/CallExpression.hpp"
#include "ast/IfExpression.hpp"
#include "ast/ReturnStatement.hpp"
#include "ast/BlockStatement.hpp"
#include "ast/ExpressionsStatement.hpp"
#include "ast/ArgumentsStatement.hpp"

namespace li::visual
{


json ASTGenerator::generateJson(const shared_ptr<Node>& node)
{
	switch (node->type())
	{
	
	case Node::Type::Let:
	{
		auto cast = dynamic_pointer_cast<LetStatement>(node);
		return {{ "type", "LetStatement" },
				{ "name", generateJson(cast->name()) },
				{ "value", generateJson(cast->value()) }};
	}

	case Node::Type::Return:
	{
		auto cast = dynamic_pointer_cast<ReturnStatement>(node);
		return {{ "type", "ReturnStatement" },
				{ "value", generateJson(cast->value()) }};
	}

	case Node::Type::ExpressionStatement:
	{
		auto cast = dynamic_pointer_cast<ExpressionStatement>(node);
		return {{ "type", "ExpressionStatement" },
				{ "expression", generateJson(cast->expression()) }};
	}

	case Node::Type::Integer:
	{
		auto cast = dynamic_pointer_cast<IntegerExpression>(node);
		return cast->value();
	}

	case Node::Type::Prefix:
	{
		auto cast = dynamic_pointer_cast<PrefixExpression>(node);
		return {{ "type", "PrefixExpression" },
				{ "operator", cast->operatorName() },
				{ "right", generateJson(cast->right()) }};
	}

	case Node::Type::Infix:
	{
		auto cast = dynamic_pointer_cast<InfixExpression>(node);
		return {{ "type", "InfixExpression" },
				{ "operator", cast->operatorName() },
				{ "left", generateJson(cast->left()) },
				{ "right", generateJson(cast->right()) }};
	}

	case Node::Type::Program:
	{
		auto cast = dynamic_pointer_cast<Program>(node);
		json statements;
		for (const auto& statement : cast->statements())
		{
			statements.push_back(generateJson(statement));
		}

		return {{ "type", "Program" },
				{ "statements", statements }};
	}

	case Node::Type::Identifier:
	{
		auto cast = dynamic_pointer_cast<IdentifierExpression>(node);
		return cast->value();
	}

	case Node::Type::Bool:
    {
        auto cast = dynamic_pointer_cast<BoolExpression>(node);
		return cast->value();
    }

	case Node::Type::Block:
	{
        auto cast = dynamic_pointer_cast<BlockStatement>(node);
        json statements;
		for (const auto& statement : cast->statements())
		{
			statements.push_back(generateJson(statement));
		}
		return statements;
    }

	case Node::Type::Arguments:
	{
		auto cast = dynamic_pointer_cast<ArgumentsStatement>(node);
		json args;
		for (const auto& arg : cast->args())
		{
			args.push_back(generateJson(arg));
		}
		return args;
	}

	case Node::Type::Expression:
	{
		auto cast = dynamic_pointer_cast<ExpressionsStatement>(node);
		json exprs;
		for (const auto& expr : cast->expressions())
		{
			exprs.push_back(generateJson(expr));
		}
		return exprs;
	}

	case Node::Type::Function:
	{
        auto cast = dynamic_pointer_cast<FunctionExpression>(node);
        return {{ "type", "FunctionExpression" },
				{ "args", generateJson(cast->args()) },
				{ "body", generateJson(cast->body()) }};
    }

	{
        auto cast = dynamic_pointer_cast<CallExpression>(node);
        return {{ "type", "CallExpression" },
				{ "fun", generateJson(cast->fun()) },
				{ "exprs", generateJson(cast->exprs()) }};
    }

	{
        auto cast = dynamic_pointer_cast<IfExpression>(node);
        return {{ "type", "IfExpression" },
				{ "condition", generateJson(cast->condition()) },
				{ "consequence", generateJson(cast->consequence()) },
				{ "alternative", generateJson(cast->alternative()) }};
    }

	default:
    	return {};

	}
}


}