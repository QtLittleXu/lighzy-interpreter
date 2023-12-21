#include "evaluator/Evaluator.h"
#include "object/Bool.hpp"
#include "object/Null.hpp"
#include "object/Integer.hpp"
#include "object/Error.hpp"
#include "object/ReturnValue.hpp"
#include "ast/IntegerExpression.hpp"
#include "ast/ExpressionStatement.hpp"
#include "ast/BoolExpression.hpp"
#include "ast/PrefixExpression.hpp"
#include "ast/InfixExpression.hpp"
#include "ast/ReturnStatement.hpp"

namespace li
{


const shared_ptr<Bool> Evaluator::bool_true = make_shared<Bool>(true);
const shared_ptr<Bool> Evaluator::bool_false = make_shared<Bool>(false);
const shared_ptr<Null> Evaluator::null = make_shared<Null>();

const shared_ptr<Bool>& Evaluator::evaluate_bool(const shared_ptr<BoolExpression>& node)
{
	return node->value() ? bool_true : bool_false;
}

shared_ptr<Object> Evaluator::evaluate_program(const shared_ptr<Program>& node)
{
	shared_ptr<Object> result;
	for (const auto& statement : node->statements())
	{
		result = evaluate(statement);

		if (!result) continue;

		if (result->type() == Object::Type::ReturnValue)
		{
			return dynamic_pointer_cast<ReturnValue>(result)->value();
		}
		if (result->type() == Object::Type::Error)
		{
			return result;
		}
	}
	return result;
}

shared_ptr<Object> Evaluator::evaluate_prefix_bang(const shared_ptr<Object>& value)
{
	if (value->type() == Object::Type::Integer)
	{
		auto cast = dynamic_pointer_cast<Integer>(value);
		if (cast->value() == 0)
		{
			return bool_true;
		}
		return bool_false;
	}

	if (value == bool_false || value == null)
	{
		return bool_true;
	}
	if (value == bool_true)
	{
		return bool_false;
	}
	
	return prefix_operand_type_error("!", value->typeName());
}

shared_ptr<Object> Evaluator::evaluate_prefix_minus(const shared_ptr<Object>& value)
{
	if (value->type() != Object::Type::Integer)
	{
		return prefix_operand_type_error("-", value->typeName());
	}

	auto cast = dynamic_pointer_cast<Integer>(value);
	return make_shared<Integer>(-cast->value());
}

shared_ptr<Object> Evaluator::prefix_operand_type_error(const string& operatorName, const string& right)
{
	stringstream buffer;
	buffer << "error - prefix operand type: " << operatorName << right;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::unknown_prefix_error(const string& operatorName, const string& right)
{
	stringstream buffer;
	buffer << "error - unknown prefix operator: " << operatorName << right;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::unknown_infix_error(const string& left, const string& operatorName, const string& right)
{
	stringstream buffer;
	buffer << "error - unknown infix operator: " << left << " " << operatorName << " " << right;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::infix_operand_type_mismatch(const string& left, const string& operatorName, const string& right)
{
	stringstream buffer;
	buffer << "error - infix operand type mismatch: " << left << " " << operatorName << " " << right;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::evaluate_prefix(const string& operatorName, const shared_ptr<Object>& right)
{
	if (operatorName == "!")
	{
		return evaluate_prefix_bang(right);
	}
	if (operatorName == "-")
	{
		return evaluate_prefix_minus(right);
	}

	return unknown_prefix_error(operatorName, right->typeName());
}

const shared_ptr<Bool>& Evaluator::bool_to_object(bool value)
{
	return value ? bool_true : bool_false;
}

shared_ptr<Object> Evaluator::evaluate_infix_integer(const shared_ptr<Object>& left, const string& operatorName, const shared_ptr<Object>& right)
{
	auto leftValue = dynamic_pointer_cast<Integer>(left)->value();
	auto rightValue = dynamic_pointer_cast<Integer>(right)->value();

	if (operatorName == "+")
	{
		return make_shared<Integer>(leftValue + rightValue);
	}
	if (operatorName == "-")
	{
		return make_shared<Integer>(leftValue - rightValue);
	}
	if (operatorName == "*")
	{
		return make_shared<Integer>(leftValue * rightValue);
	}
	if (operatorName == "/")
	{
		return make_shared<Integer>(leftValue / rightValue);
	}
	if (operatorName == "==")
	{
		return bool_to_object(leftValue == rightValue);
	}
	if (operatorName == "!=")
	{
		return bool_to_object(leftValue != rightValue);
	}
	if (operatorName == ">")
	{
		return bool_to_object(leftValue > rightValue);
	}
	if (operatorName == ">=")
	{
		return bool_to_object(leftValue >= rightValue);
	}
	if (operatorName == "<")
	{
		return bool_to_object(leftValue < rightValue);
	}
	if (operatorName == "<=")
	{
		return bool_to_object(leftValue <= rightValue);
	}

	return unknown_infix_error(left->typeName(), operatorName, right->typeName());
}

bool Evaluator::is_true(const shared_ptr<Object>& obj)
{
	if (obj == null || obj == bool_false)
	{
		return false;
	}
	if (obj == bool_true)
	{
		return true;
	}
	return true;
}

shared_ptr<Object> Evaluator::evaluate_infix(const shared_ptr<Object>& left, const string& operatorName, const shared_ptr<Object>& right)
{
	if (left->type() != right->type())
	{
		return infix_operand_type_mismatch(left->typeName(), operatorName, right->typeName());
	}

	if (left->type() == Object::Type::Integer && right->type() == Object::Type::Integer)
	{
		return evaluate_infix_integer(left, operatorName, right);
	}
	if (operatorName == "==")
	{
		return bool_to_object(left == right);
	}
	if (operatorName == "!=")
	{
		return bool_to_object(left != right);
	}

	return unknown_infix_error(left->typeName(), operatorName, right->typeName());
}

shared_ptr<Object> Evaluator::evaluate_block(const shared_ptr<BlockStatement>& node)
{
	shared_ptr<Object> result;
	for (const auto& statement : node->statements())
	{
		result = evaluate(statement);

		if (!result) continue;

		if (result->type() == Object::Type::ReturnValue || result->type() == Object::Type::Error)
		{
			return result;
		}
	}
	return result;
}

shared_ptr<Object> Evaluator::evaluate_if(const shared_ptr<IfExpression>& node)
{
	if (is_true(evaluate(node->condition())))
	{
		return evaluate(node->consequence());
	}
	if (node->alternative() != nullptr)
	{
		return evaluate(node->alternative());
	}
	return null;
}

shared_ptr<Object> Evaluator::evaluate(const shared_ptr<Node>& node)
{
	switch (node->type())
	{

	case Node::Type::Program:
	{
		auto cast = dynamic_pointer_cast<Program>(node);
		return evaluate_program(cast);
	}

	case Node::Type::ExpressionStatement:
	{
		auto cast = dynamic_pointer_cast<ExpressionStatement>(node);
		return evaluate(cast->expression());
	}
	
	case Node::Type::Integer:
	{
		auto cast = dynamic_pointer_cast<IntegerExpression>(node);
		return make_shared<Integer>(cast->value());
	}

	case Node::Type::Bool:
	{
		auto cast = dynamic_pointer_cast<BoolExpression>(node);
		return evaluate_bool(cast);
	}
	case Node::Type::Prefix:
	{
		auto cast = dynamic_pointer_cast<PrefixExpression>(node);
		auto right = evaluate(cast->right());
		if (right->type() == Object::Type::Error)
		{
			return right;
		}

		return evaluate_prefix(cast->operatorName(), right);
	}

	case Node::Type::Infix:
	{
		auto cast = dynamic_pointer_cast<InfixExpression>(node);
		auto left = evaluate(cast->left());
		if (left->type() == Object::Type::Error)
		{
			return left;
		}
		auto right = evaluate(cast->right());
		if (right->type() == Object::Type::Error)
		{
			return right;
		}

		return evaluate_infix(left, cast->operatorName(), right);
	}

	case Node::Type::Block:
	{
		auto cast = dynamic_pointer_cast<BlockStatement>(node);
		return evaluate_block(cast);
	}

	case Node::Type::If:
	{
		auto cast = dynamic_pointer_cast<IfExpression>(node);
		return evaluate_if(cast);
	}

	case Node::Type::Return:
	{
		auto cast = dynamic_pointer_cast<ReturnStatement>(node);
		auto value = evaluate(cast->value());
		if (value->type() == Object::Type::Error)
		{
			return value;
		}

		return make_shared<ReturnValue>(value);
	}

	default:
		return null;

	}
}


}