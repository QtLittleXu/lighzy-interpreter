#pragma once

#include "ast/BoolExpression.hpp"
#include "ast/IfExpression.hpp"
#include "ast/IdentifierExpression.hpp"
#include "ast/ExpressionsStatement.hpp"
#include "ast/Program.hpp"
#include "ast/BlockStatement.hpp"
#include "object/basic/Object.h"
#include "object/Bool.hpp"
#include "object/Null.hpp"
#include "object/Function.hpp"
#include "object/Environment.hpp"

namespace li
{


class Evaluator
{
public:
	shared_ptr<Object> evaluate(const shared_ptr<Node>& node, const shared_ptr<Environment>& env);

private:
	bool is_true(const shared_ptr<Object>& obj);

	shared_ptr<Object> unknown_prefix(const string& operatorName, const string& right);
	shared_ptr<Object> unknown_infix(const string& left, const string& operatorName, const string& right);
	shared_ptr<Object> infix_operand_type_mismatch(const string& left, const string& operatorName, const string& right);
	shared_ptr<Object> prefix_operand_type(const string& operatorName, const string& right);
	shared_ptr<Object> identifier_not_found(const string& name);
	shared_ptr<Object> not_function(const string& type);

private:
	const shared_ptr<Bool>& evaluate_bool(const shared_ptr<BoolExpression>& node);
	const shared_ptr<Bool>& bool_to_object(bool value);
	const shared_ptr<Environment> bind_fun_args_to_objects(const shared_ptr<Function>& fun, const vector<shared_ptr<Object>>& objects);

	shared_ptr<Object> evaluate_prefix(const string& operatorName, const shared_ptr<Object>& right);
	shared_ptr<Object> evaluate_prefix_bang(const shared_ptr<Object>& value);
	shared_ptr<Object> evaluate_prefix_minus(const shared_ptr<Object>& value);
	shared_ptr<Object> evaluate_infix(const shared_ptr<Object>& left, const string& operatorName, const shared_ptr<Object>& right);
	shared_ptr<Object> evaluate_block(const shared_ptr<BlockStatement>& node, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_if(const shared_ptr<IfExpression>& node, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_program(const shared_ptr<Program>& node, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_id(const shared_ptr<IdentifierExpression>& id, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_fun(const shared_ptr<Function>& fun, const vector<shared_ptr<Object>>& args);
	shared_ptr<Object> evaluate_infix_integer(const shared_ptr<Object>& left, const string& operatorName, const shared_ptr<Object>& right);
	vector<shared_ptr<Object>> evaluate_exprs(const shared_ptr<ExpressionsStatement>& exprs, const shared_ptr<Environment>& env);

public:
	static const shared_ptr<Bool> bool_true;
	static const shared_ptr<Bool> bool_false;
	static const shared_ptr<Null> null;
};


}