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
#include "ast/FunctionExpression.hpp"
#include "ast/LetStatement.hpp"
#include "ast/CallExpression.hpp"

namespace li
{


const shared_ptr<Bool> Evaluator::bool_true = make_shared<Bool>(true);
const shared_ptr<Bool> Evaluator::bool_false = make_shared<Bool>(false);
const shared_ptr<Null> Evaluator::null = make_shared<Null>();

const shared_ptr<Bool>& Evaluator::evaluate_bool(const shared_ptr<BoolExpression>& node)
{
	return node->value() ? bool_true : bool_false;
}

shared_ptr<Object> Evaluator::evaluate_program(const shared_ptr<Program>& node, const shared_ptr<Environment>& env)
{
	shared_ptr<Object> result;
	for (const auto& statement : node->statements())
	{
		result = evaluate(statement, env);

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
	
	return prefix_operand_type("!", value->typeName());
}

shared_ptr<Object> Evaluator::evaluate_prefix_minus(const shared_ptr<Object>& value)
{
	if (value->type() != Object::Type::Integer)
	{
		return prefix_operand_type("-", value->typeName());
	}

	auto cast = dynamic_pointer_cast<Integer>(value);
	return make_shared<Integer>(-cast->value());
}

shared_ptr<Object> Evaluator::prefix_operand_type(const string& operatorName, const string& right)
{
	stringstream buffer;
	buffer << "error - prefix operand type: " << operatorName << right;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::unknown_prefix(const string& operatorName, const string& right)
{
	stringstream buffer;
	buffer << "error - unknown prefix operator: " << operatorName << right;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::unknown_infix(const string& left, const string& operatorName, const string& right)
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

shared_ptr<Object> Evaluator::identifier_not_found(const string& name)
{
	stringstream buffer;
	buffer << "error - identifier not found: " << name;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::not_function(const string& type)
{
	stringstream buffer;
	buffer << "error - expected function: " << type;
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

	return unknown_prefix(operatorName, right->typeName());
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

	return unknown_infix(left->typeName(), operatorName, right->typeName());
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

	return unknown_infix(left->typeName(), operatorName, right->typeName());
}

shared_ptr<Object> Evaluator::evaluate_block(const shared_ptr<BlockStatement>& node, const shared_ptr<Environment>& env)
{
	shared_ptr<Object> result;
	for (const auto& statement : node->statements())
	{
		result = evaluate(statement, env);

		if (!result) continue;

		if (result->type() == Object::Type::ReturnValue || result->type() == Object::Type::Error)
		{
			return result;
		}
	}
	return result;
}

shared_ptr<Object> Evaluator::evaluate_if(const shared_ptr<IfExpression>& node, const shared_ptr<Environment>& env)
{
	if (is_true(evaluate(node->condition(), env)))
	{
		return evaluate(node->consequence(), env);
	}
	if (node->alternative() != nullptr)
	{
		return evaluate(node->alternative(), env);
	}
	return null;
}

shared_ptr<Object> Evaluator::evaluate_id(const shared_ptr<IdentifierExpression>& id, const shared_ptr<Environment>& env)
{
	auto value = env->get(id->value());
	if (!value)
	{
		return identifier_not_found(id->value());
	}
	return value;
}

vector<shared_ptr<Object>> Evaluator::evaluate_exprs(const shared_ptr<ExpressionsStatement>& exprs, const shared_ptr<Environment>& env)
{
	vector<shared_ptr<Object>> result;
	for (const auto& expr : exprs->expressions())
	{
		auto evaluated = evaluate(expr, env);
		if (evaluated->type() == Object::Type::Error)
		{
			return { evaluated };
		}
		result.push_back(evaluated);
	}
	return result;
}

const shared_ptr<Environment> Evaluator::bind_fun_args_to_objects(const shared_ptr<Function>& fun, const vector<shared_ptr<Object>>& objects)
{
	auto env = make_shared<Environment>(fun->env());
	for (int i = 0; i < fun->args()->args().size(); i++)
	{
		auto id = fun->args()->args().at(i);
		env->set(id->value(), objects.at(i));
	}
	return env;
}

shared_ptr<Object> Evaluator::evaluate_fun(const shared_ptr<Function>& fun, const vector<shared_ptr<Object>>& args)
{
	auto innerEnv = bind_fun_args_to_objects(fun, args);
	auto evaluated = evaluate(fun->body(), innerEnv);

	if (evaluated->type() == Object::Type::ReturnValue)
	{
		return dynamic_pointer_cast<ReturnValue>(evaluated)->value();
	}
	return evaluated;
}

shared_ptr<Object> Evaluator::evaluate(const shared_ptr<Node>& node, const shared_ptr<Environment>& env)
{
	switch (node->type())
	{

	case Node::Type::Program:
	{
		auto cast = dynamic_pointer_cast<Program>(node);
		return evaluate_program(cast, env);
	}

	case Node::Type::ExpressionStatement:
	{
		auto cast = dynamic_pointer_cast<ExpressionStatement>(node);
		return evaluate(cast->expression(), env);
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
		auto right = evaluate(cast->right(), env);
		if (right->type() == Object::Type::Error)
		{
			return right;
		}

		return evaluate_prefix(cast->operatorName(), right);
	}

	case Node::Type::Infix:
	{
		auto cast = dynamic_pointer_cast<InfixExpression>(node);
		auto left = evaluate(cast->left(), env);
		if (left->type() == Object::Type::Error)
		{
			return left;
		}
		auto right = evaluate(cast->right(), env);
		if (right->type() == Object::Type::Error)
		{
			return right;
		}

		return evaluate_infix(left, cast->operatorName(), right);
	}

	case Node::Type::Block:
	{
		auto cast = dynamic_pointer_cast<BlockStatement>(node);
		return evaluate_block(cast, env);
	}

	case Node::Type::If:
	{
		auto cast = dynamic_pointer_cast<IfExpression>(node);
		return evaluate_if(cast, env);
	}

	case Node::Type::Return:
	{
		auto cast = dynamic_pointer_cast<ReturnStatement>(node);
		auto value = evaluate(cast->value(), env);
		if (value->type() == Object::Type::Error)
		{
			return value;
		}

		return make_shared<ReturnValue>(value);
	}

	case Node::Type::Let:
	{
		auto cast = dynamic_pointer_cast<LetStatement>(node);
		auto value = evaluate(cast->value(), env);
		if (value->type() == Object::Type::Error)
		{
			return value;
		}

		env->set(cast->name()->value(), value);
		return null;
	}

	case Node::Type::Identifier:
	{
		auto cast = dynamic_pointer_cast<IdentifierExpression>(node);
		return evaluate_id(cast, env);
	}

	case Node::Type::Function:
	{
		auto cast = dynamic_pointer_cast<FunctionExpression>(node);
		return make_shared<Function>(cast->args(), cast->body(), env);
	}

	case Node::Type::Call:
	{
		auto cast = dynamic_pointer_cast<CallExpression>(node);
		auto fun = evaluate(cast->fun(), env);
		if (fun->type() == Object::Type::Error)
		{
			return fun;
		}
		if (fun->type() != Object::Type::Function)
		{
			return not_function(fun->typeName());
		}

		auto args = evaluate_exprs(cast->exprs(), env);
		if (args.size() == 1 && args.at(0)->type() == Object::Type::Error)
		{
			return args.at(0);
		}
		
		return evaluate_fun(dynamic_pointer_cast<Function>(fun), args);
	}

	default:
		return null;

	}
}


}