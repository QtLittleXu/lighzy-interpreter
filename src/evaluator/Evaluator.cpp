#include "evaluator/Evaluator.h"
#include "object/Bool.hpp"
#include "object/Null.hpp"
#include "object/Integer.hpp"
#include "object/Error.hpp"
#include "object/ReturnValue.hpp"
#include "object/String.hpp"
#include "object/Float.hpp"
#include "object/Array.hpp"
#include "ast/IntegerExpr.hpp"
#include "ast/BoolExpr.hpp"
#include "ast/PrefixExpr.hpp"
#include "ast/InfixExpr.hpp"
#include "ast/ReturnStat.hpp"
#include "ast/FunctionExpr.hpp"
#include "ast/LetStat.hpp"
#include "ast/CallExpr.hpp"
#include "ast/StringExpr.hpp"
#include "ast/AssignExpr.hpp"
#include "ast/ExpressionStat.hpp"
#include "ast/FloatExpr.hpp"
#include "ast/ArrayExpr.hpp"
#include "ast/IndexExpr.hpp"
#include "ast/WhileStat.hpp"
#include "ast/VarStat.hpp"
#include "ast/InDecrementExpr.hpp"

namespace li
{


const shared_ptr<Bool> Evaluator::bool_true = make_shared<Bool>(true);
const shared_ptr<Bool> Evaluator::bool_false = make_shared<Bool>(false);
const shared_ptr<Null> Evaluator::null = make_shared<Null>();

shared_ptr<Bool> Evaluator::evaluate_bool(shared_ptr<BoolExpr> node)
{
	return node->value ? bool_true : bool_false;
}

shared_ptr<Object> Evaluator::evaluate_program(shared_ptr<Program> node, shared_ptr<Environment> env)
{
	shared_ptr<Object> result;
	for (const auto& statement : node->statements)
	{
		result = evaluate(statement, env);

		if (!result) continue;

		if (result->type == Object::Type::ReturnValue)
		{
			return dynamic_pointer_cast<ReturnValue>(result)->value;
		}
		if (result->type == Object::Type::Error)
		{
			return result;
		}
	}
	return result;
}

shared_ptr<Object> Evaluator::evaluate_prefix_bang(shared_ptr<Object> value)
{
	if (value->type == Object::Type::Integer)
	{
		auto cast = dynamic_pointer_cast<Integer>(value);
		if (cast->value == 0)
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
	
	return operand_type_error("prefix", "!", value->typeName());
}

shared_ptr<Object> Evaluator::evaluate_prefix_minus(shared_ptr<Object> value)
{
	switch (value->type)
	{
	
	case Object::Type::Integer:
	{
		auto cast = dynamic_pointer_cast<Integer>(value);
		return make_shared<Integer>(-cast->value);
	}

	case Object::Type::Float:
	{
		auto cast = dynamic_pointer_cast<Float>(value);
		return make_shared<Float>(-cast->value);
	}

	default:
		return operand_type_error("prefix", "-", value->typeName());

	}
}

shared_ptr<Object> Evaluator::operand_type_error(const string& type, const string& operatorName, const string& right)
{
	stringstream buffer;
	buffer << "error - " << type << " operand type: " << operatorName << right;
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

shared_ptr<Object> Evaluator::invalid_arguments(const string& msg)
{
	stringstream buffer;
	buffer << "error - invalid arguments: " << msg;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::index_operand_type(const string& left, const string& index)
{
	stringstream buffer;
	buffer << "error - index operand type: " << left << "[" << index << "]";
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::repeat_declaration(const string& name)
{
	stringstream buffer;
	buffer << "error - repeat declaration: " << name;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::access_immutable_var(const string &name)
{
	stringstream buffer;
	buffer << "error - cannot access immutable variable: " << name;
	return make_shared<Error>(buffer.str());
}

shared_ptr<Object> Evaluator::evaluate_prefix(const string& operatorName, shared_ptr<Object> right)
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

shared_ptr<Bool> Evaluator::bool_to_object(bool value)
{
	return value ? bool_true : bool_false;
}

shared_ptr<Object> Evaluator::evaluate_infix_number(shared_ptr<Object> left, const string& operatorName, shared_ptr<Object> right)
{
	double leftValue = 0;
	double rightValue = 0;

	switch (left->type)
	{
	case Object::Type::Integer:
		leftValue = dynamic_pointer_cast<Integer>(left)->value;
		break;

	case Object::Type::Float:
		leftValue = dynamic_pointer_cast<Float>(left)->value;
		break;

	default:
		return infix_operand_type_mismatch(left->typeName(), operatorName, right->typeName());
	}

	switch (right->type)
	{
	case Object::Type::Integer:
		rightValue = dynamic_pointer_cast<Integer>(right)->value;
		break;

	case Object::Type::Float:
		rightValue = dynamic_pointer_cast<Float>(right)->value;
		break;

	default:
		return infix_operand_type_mismatch(left->typeName(), operatorName, right->typeName());
	}

	if (operatorName == "+")
	{
		if (left->type == Object::Type::Float || right->type == Object::Type::Float)
		{
			return make_shared<Float>(leftValue + rightValue);
		}

		return make_shared<Integer>(leftValue + rightValue);
	}
	if (operatorName == "-")
	{
		if (left->type == Object::Type::Float || right->type == Object::Type::Float)
		{
			return make_shared<Float>(leftValue - rightValue);
		}

		return make_shared<Integer>(leftValue - rightValue);
	}
	if (operatorName == "*")
	{
		if (left->type == Object::Type::Float || right->type == Object::Type::Float)
		{
			return make_shared<Float>(leftValue * rightValue);
		}

		return make_shared<Integer>(leftValue * rightValue);
	}
	if (operatorName == "/")
	{
		if (left->type == Object::Type::Float || right->type == Object::Type::Float)
		{
			return make_shared<Float>(leftValue / rightValue);
		}

		return make_shared<Integer>(leftValue / rightValue);
	}
	if (operatorName == "%")
	{
		if (left->type == Object::Type::Integer || right->type == Object::Type::Integer)
		{
			return make_shared<Integer>(static_cast<int64_t>(leftValue) % static_cast<int64_t>(rightValue));
		}
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

bool Evaluator::is_true(shared_ptr<Object> obj)
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

shared_ptr<Object> Evaluator::evaluate_infix_string(shared_ptr<Object> left, const string& operatorName, shared_ptr<Object> right)
{
	auto leftValue = dynamic_pointer_cast<String>(left)->value;
	auto rightValue = dynamic_pointer_cast<String>(right)->value;

	if (operatorName == "+")
	{
		return make_shared<String>(leftValue + rightValue);
	}
	if (operatorName == "==")
	{
		bool_to_object(leftValue == rightValue);
	}
	if (operatorName == "!=")
	{
		bool_to_object(leftValue != rightValue);
	}

	return unknown_infix(left->typeName(), operatorName, right->typeName());
}

shared_ptr<Object> Evaluator::evaluate_infix(shared_ptr<Object> left, const string& operatorName, shared_ptr<Object> right)
{
	if (left->type == Object::Type::Integer || left->type == Object::Type::Float)
	{
		return evaluate_infix_number(left, operatorName, right);
	}
	if (left->type == Object::Type::String)
	{
		return evaluate_infix_string(left, operatorName, right);
	}

	if (left->type != right->type)
	{
		return infix_operand_type_mismatch(left->typeName(), operatorName, right->typeName());
	}
	// Compare address directly
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

shared_ptr<Object> Evaluator::evaluate_block(shared_ptr<BlockStat> node, shared_ptr<Environment> env)
{
	shared_ptr<Object> result;
	for (const auto& statement : node->statements)
	{
		result = evaluate(statement, env);

		if (!result) continue;

		if (result->type == Object::Type::ReturnValue || result->type == Object::Type::Error)
		{
			return result;
		}
	}
	return result;
}

shared_ptr<Object> Evaluator::evaluate_if(shared_ptr<IfExpr> node, shared_ptr<Environment> env)
{
	if (is_true(evaluate(node->condition, env)))
	{
		return evaluate(node->consequence, env);
	}
	if (node->alternative != nullptr)
	{
		return evaluate(node->alternative, env);
	}
	return null;
}

shared_ptr<Object> Evaluator::evaluate_id(shared_ptr<IdentifierExpr> id, shared_ptr<Environment> env)
{
	auto* value = env->get(id->value);
	if (value != nullptr)
	{
		return *value;
	}
	
	auto it = builtinFuns.find(id->value);
	if (it != builtinFuns.end())
	{
		return it->second;
	}

	return identifier_not_found(id->value);
}

vector<shared_ptr<Object>> Evaluator::evaluate_exprs(shared_ptr<ExpressionsStat> exprs, shared_ptr<Environment> env)
{
	vector<shared_ptr<Object>> result;
	for (const auto& expr : exprs->expressions)
	{
		auto evaluated = evaluate(expr, env);
		if (evaluated->type == Object::Type::Error)
		{
			return { evaluated };
		}
		result.push_back(evaluated);
	}
	return result;
}

tuple<shared_ptr<Object>, shared_ptr<Environment>> Evaluator::bind_fun_args_to_objects(shared_ptr<Function> fun, const vector<shared_ptr<Object>>& objects)
{
	if (fun->args->args.size() != objects.size())
	{
		return { invalid_arguments(string("expected the number of them to be 2, but got ") + to_string(objects.size())), nullptr};
	}

	auto env = make_shared<Environment>(fun->env);
	for (int i = 0; i < fun->args->args.size(); i++)
	{
		auto id = fun->args->args.at(i);
		auto copied = objects.at(i)->copy();
		copied->setMutable(true);
		env->add(id->value, copied);
	}
	return { nullptr, env };
}

shared_ptr<Object> Evaluator::evaluate_fun(shared_ptr<Object> fun, const vector<shared_ptr<Object>>& args)
{
	switch (fun->type)
	{

	case Object::Type::Function:
	{
		auto cast = dynamic_pointer_cast<Function>(fun);

		auto [error, innerEnv] = bind_fun_args_to_objects(cast, args);
		if (error) return error;

		auto evaluated = evaluate(cast->body, innerEnv);

		if (evaluated->type == Object::Type::ReturnValue)
		{
			return dynamic_pointer_cast<ReturnValue>(evaluated)->value;
		}
		return evaluated;
	}

	case Object::Type::BuiltinFun:
	{
		auto cast = dynamic_pointer_cast<BuiltinFun>(fun);
		return cast->fun(args);
	}

	default:
		return not_function(fun->typeName());
	}
}

shared_ptr<Object> Evaluator::evaluate_index(shared_ptr<Object> left, shared_ptr<Object> index)
{
	if (left->type == Object::Type::Array && index->type == Object::Type::Integer)
	{
		return evaluate_index_array(dynamic_pointer_cast<Array>(left), dynamic_pointer_cast<Integer>(index));
	}

	return index_operand_type(left->typeName(), index->typeName());
}

shared_ptr<Object> Evaluator::evaluate_index_array(shared_ptr<Array> array, shared_ptr<Integer> index)
{
	size_t size = array->elements.size();
	if (index->value >= size || index->value < 0)
	{
		return null;
	}
	return array->elements.at(index->value);
}

shared_ptr<Object> Evaluator::evaluate_in_decrement(shared_ptr<Object> id, const string& operatorName, shared_ptr<Environment> env)
{
	if (operatorName == "++")
	{
		switch (id->type)
		{

		case Object::Type::Integer:
		{
			auto cast = dynamic_pointer_cast<Integer>(id);
			cast->value++;
			return cast;
		}

		default:
			return operand_type_error("increment or decrement", operatorName, id->typeName());
		}
	}

	if (operatorName == "--")
	{
		switch (id->type)
		{

		case Object::Type::Integer:
		{
			auto cast = dynamic_pointer_cast<Integer>(id);
			cast->value--;
			return cast;
		}

		default:
			return operand_type_error("increment or decrement", operatorName, id->typeName());
		}
	}

	return unknown_prefix(operatorName, id->typeName());
}

shared_ptr<Object> Evaluator::evaluate_assign(shared_ptr<Object> id, const string& operatorName, shared_ptr<Object> value, shared_ptr<Environment> env)
{
	if (operatorName == "=")
	{
		bool origin = id->isMutable;
		id->assign(value);
		id->isMutable = origin;
		return value;
	}
	
	if (operatorName.size() != 2)
	{
		return unknown_infix(id->typeName(), operatorName, value->typeName());
	}

	auto result = evaluate_infix(id, string(1, operatorName.at(0)), value);

	bool origin = id->isMutable;
	id->assign(result);
	id->isMutable = origin;
	return result;
}

shared_ptr<Object> Evaluator::evaluate(shared_ptr<Node> node, shared_ptr<Environment> env)
{
	switch (node->type)
	{

	case Node::Type::Program:
	{
		auto cast = dynamic_pointer_cast<Program>(node);
		return evaluate_program(cast, env);
	}

	case Node::Type::ExprStat:
	{
		auto cast = dynamic_pointer_cast<ExpressionStat>(node);
		return evaluate(cast->expression, env);
	}
	
	case Node::Type::Integer:
	{
		auto cast = dynamic_pointer_cast<IntegerExpr>(node);
		return make_shared<Integer>(cast->value);
	}

	case Node::Type::Float:
	{
		auto cast = dynamic_pointer_cast<FloatExpr>(node);
		return make_shared<Float>(cast->value);
	}

	case Node::Type::Bool:
	{
		auto cast = dynamic_pointer_cast<BoolExpr>(node);
		return evaluate_bool(cast);
	}
	case Node::Type::Prefix:
	{
		auto cast = dynamic_pointer_cast<PrefixExpr>(node);
		auto right = evaluate(cast->right, env);
		if (right->type == Object::Type::Error)
		{
			return right;
		}

		return evaluate_prefix(cast->operatorName, right);
	}

	case Node::Type::Infix:
	{
		auto cast = dynamic_pointer_cast<InfixExpr>(node);
		auto left = evaluate(cast->left, env);
		if (left->type == Object::Type::Error)
		{
			return left;
		}
		auto right = evaluate(cast->right, env);
		if (right->type == Object::Type::Error)
		{
			return right;
		}

		return evaluate_infix(left, cast->operatorName, right);
	}

	case Node::Type::Block:
	{
		auto cast = dynamic_pointer_cast<BlockStat>(node);
		return evaluate_block(cast, env);
	}

	case Node::Type::If:
	{
		auto cast = dynamic_pointer_cast<IfExpr>(node);
		return evaluate_if(cast, env);
	}

	case Node::Type::Return:
	{
		auto cast = dynamic_pointer_cast<ReturnStat>(node);
		auto value = evaluate(cast->value, env);
		if (value->type == Object::Type::Error)
		{
			return value;
		}

		return make_shared<ReturnValue>(value);
	}

	case Node::Type::Let:
	{
		auto cast = dynamic_pointer_cast<LetStat>(node);
		auto value = evaluate(cast->value, env);
		if (value->type == Object::Type::Error)
		{
			return value;
		}

		string name = cast->name->value;
		auto found = env->store.find(name);
		if (found != env->store.end())
		{
			return repeat_declaration(name);
		}

		value->setMutable(false);
		env->add(name, value);
		return null;
	}

	case Node::Type::Var:
	{
		auto cast = dynamic_pointer_cast<VarStat>(node);
		auto value = evaluate(cast->value, env);
		if (value->type == Object::Type::Error)
		{
			return value;
		}

		string name = cast->name->value;
		auto found = env->store.find(name);
		if (found != env->store.end())
		{
			return repeat_declaration(name);
		}

		value->setMutable(true);
		env->add(name, value);
		return null;
	}

	case Node::Type::Identifier:
	{
		auto cast = dynamic_pointer_cast<IdentifierExpr>(node);
		return evaluate_id(cast, env);
	}

	case Node::Type::Function:
	{
		auto cast = dynamic_pointer_cast<FunctionExpr>(node);
		return make_shared<Function>(cast->args, cast->body, env);
	}

	case Node::Type::Call:
	{
		auto cast = dynamic_pointer_cast<CallExpr>(node);
		auto fun = evaluate(cast->fun, env);
		if (fun->type == Object::Type::Error)
		{
			return fun;
		}

		auto args = evaluate_exprs(cast->exprs, env);
		if (args.size() == 1 && args.at(0)->type == Object::Type::Error)
		{
			return args.at(0);
		}
		
		return evaluate_fun(fun, args);
	}

	case Node::Type::String:
	{
		auto cast = dynamic_pointer_cast<StringExpr>(node);
		return make_shared<String>(cast->value);
	}

	case Node::Type::Assign:
	{
		auto cast = dynamic_pointer_cast<AssignExpr>(node);
		auto id = evaluate(cast->id, env);		// This code is only for detecting whether identifier is found
		auto value = evaluate(cast->value, env);
		if (id->type == Object::Type::Error)
		{
			return id;
		}
		if (value->type == Object::Type::Error)
		{
			return value;
		}
		if (!id->isMutable)
		{
			return access_immutable_var(cast->id->literal());
		}

		return evaluate_assign(id, cast->operatorName, value, env);
	}

	case Node::Type::Array:
	{
		auto cast = dynamic_pointer_cast<ArrayExpr>(node);
		auto elements = evaluate_exprs(cast->elements, env);
		if (elements.size() == 1 && elements.at(0)->type == Object::Type::Error)
		{
			return elements.at(0);
		}

		return make_shared<Array>(elements);
	}

	case Node::Type::Index:
	{
		auto cast = dynamic_pointer_cast<IndexExpr>(node);
		auto left = evaluate(cast->left, env);
		if (left->type == Object::Type::Error)
		{
			return left;
		}

		auto index = evaluate(cast->index, env);
		if (index->type == Object::Type::Error)
		{
			return index;
		}

		return evaluate_index(left, index);
	}

	case Node::Type::While:
	{
		auto cast = dynamic_pointer_cast<WhileStat>(node);
		while (is_true(evaluate(cast->condition, env)))
		{
			evaluate(cast->body, make_shared<Environment>(env));
		}
		return null;
	}

	case Node::Type::InDecrement:
	{
		auto cast = dynamic_pointer_cast<InDecrementExpr>(node);
		auto id = evaluate(cast->id, env);
		if (id->type == Object::Type::Error)
		{
			return id;
		}

		return evaluate_in_decrement(id, cast->operatorName, env);
	}

	default:
		return null;

	}
}


}