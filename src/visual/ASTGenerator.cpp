#include "visual/ASTGenerator.h"
#include "ast/ExpressionStat.hpp"
#include "ast/InfixExpr.hpp"
#include "ast/IntegerExpr.hpp"
#include "ast/LetStat.hpp"
#include "ast/PrefixExpr.hpp"
#include "ast/FunctionExpr.hpp"
#include "ast/Program.hpp"
#include "ast/BoolExpr.hpp"
#include "ast/CallExpr.hpp"
#include "ast/IfExpr.hpp"
#include "ast/ReturnStat.hpp"
#include "ast/BlockStat.hpp"
#include "ast/ExpressionsStat.hpp"
#include "ast/ArgumentsStat.hpp"

namespace li::visual
{


json ASTGenerator::generateJson(const shared_ptr<Node>& node)
{
	switch (node->type)
	{
	
	case Node::Type::Let:
	{
		auto cast = dynamic_pointer_cast<LetStat>(node);
		return {{ "type", "LetStat" },
				{ "name", generateJson(cast->name) },
				{ "value", generateJson(cast->value) }};
	}

	case Node::Type::Return:
	{
		auto cast = dynamic_pointer_cast<ReturnStat>(node);
		return {{ "type", "ReturnStat" },
				{ "value", generateJson(cast->value) }};
	}

	case Node::Type::ExprStat:
	{
		auto cast = dynamic_pointer_cast<ExpressionStat>(node);
		return {{ "type", "ExprStat" },
				{ "Expr", generateJson(cast->expression) }};
	}

	case Node::Type::Integer:
	{
		auto cast = dynamic_pointer_cast<IntegerExpr>(node);
		return cast->value;
	}

	case Node::Type::Prefix:
	{
		auto cast = dynamic_pointer_cast<PrefixExpr>(node);
		return {{ "type", "PrefixExpr" },
				{ "operator", cast->operatorName },
				{ "right", generateJson(cast->right) }};
	}

	case Node::Type::Infix:
	{
		auto cast = dynamic_pointer_cast<InfixExpr>(node);
		return {{ "type", "InfixExpr" },
				{ "operator", cast->operatorName },
				{ "left", generateJson(cast->left) },
				{ "right", generateJson(cast->right) }};
	}

	case Node::Type::Program:
	{
		auto cast = dynamic_pointer_cast<Program>(node);
		json Stats;
		for (const auto& Stat : cast->statements)
		{
			Stats.push_back(generateJson(Stat));
		}

		return {{ "type", "Program" },
				{ "Stats", Stats }};
	}

	case Node::Type::Identifier:
	{
		auto cast = dynamic_pointer_cast<IdentifierExpr>(node);
		return cast->value;
	}

	case Node::Type::Bool:
    {
        auto cast = dynamic_pointer_cast<BoolExpr>(node);
		return cast->value;
    }

	case Node::Type::Block:
	{
        auto cast = dynamic_pointer_cast<BlockStat>(node);
        json Stats;
		for (const auto& Stat : cast->statements)
		{
			Stats.push_back(generateJson(Stat));
		}
		return Stats;
    }

	case Node::Type::Arguments:
	{
		auto cast = dynamic_pointer_cast<ArgumentsStat>(node);
		json args;
		for (const auto& arg : cast->args)
		{
			args.push_back(generateJson(arg));
		}
		return args;
	}

	case Node::Type::Expr:
	{
		auto cast = dynamic_pointer_cast<ExpressionsStat>(node);
		json exprs;
		for (const auto& expr : cast->expressions)
		{
			exprs.push_back(generateJson(expr));
		}
		return exprs;
	}

	case Node::Type::Function:
	{
        auto cast = dynamic_pointer_cast<FunctionExpr>(node);
        return {{ "type", "FunctionExpr" },
				{ "args", generateJson(cast->args) },
				{ "body", generateJson(cast->body) }};
    }

	{
        auto cast = dynamic_pointer_cast<CallExpr>(node);
        return {{ "type", "CallExpr" },
				{ "fun", generateJson(cast->fun) },
				{ "exprs", generateJson(cast->exprs) }};
    }

	{
        auto cast = dynamic_pointer_cast<IfExpr>(node);
        return {{ "type", "IfExpr" },
				{ "condition", generateJson(cast->condition) },
				{ "consequence", generateJson(cast->consequence) },
				{ "alternative", generateJson(cast->alternative) }};
    }

	default:
    	return {};

	}
}


}