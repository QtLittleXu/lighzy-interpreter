#pragma once

#include "ast/BoolExpr.hpp"
#include "ast/IfExpr.hpp"
#include "ast/IdentifierExpr.hpp"
#include "ast/ExpressionsStat.hpp"
#include "ast/Program.hpp"
#include "ast/BlockStat.hpp"
#include "ast/IndexExpr.hpp"
#include "ast/AssignExpr.hpp"
#include "object/basic/Object.h"
#include "object/Bool.hpp"
#include "object/Null.hpp"
#include "object/Function.hpp"
#include "object/Environment.hpp"
#include "object/BuiltinFun.hpp"
#include "object/Array.hpp"
#include "object/Integer.hpp"

namespace li
{


class Evaluator
{
public:
	shared_ptr<Object> evaluate(const shared_ptr<Node>& node, const shared_ptr<Environment>& env);

public:
	bool is_true(const shared_ptr<Object>& obj);

	static shared_ptr<Object> unknown_prefix(const string& operatorName, const string& right);
	static shared_ptr<Object> unknown_infix(const string& left, const string& operatorName, const string& right);
	static shared_ptr<Object> infix_operand_type_mismatch(const string& left, const string& operatorName, const string& right);
	static shared_ptr<Object> operand_type_error(const string& type, const string& operatorName, const string& right);
	static shared_ptr<Object> identifier_not_found(const string& name);
	static shared_ptr<Object> not_function(const string& type);
	static shared_ptr<Object> invalid_arguments(const string& msg);
	static shared_ptr<Object> index_operand_type(const string& left, const string& index);

private:
	const shared_ptr<Bool>& evaluate_bool(const shared_ptr<BoolExpr>& node);
	const shared_ptr<Bool>& bool_to_object(bool value);
	tuple<shared_ptr<Object>, shared_ptr<Environment>> bind_fun_args_to_objects(const shared_ptr<Function>& fun, const vector<shared_ptr<Object>>& objects);

	shared_ptr<Object> evaluate_prefix(const string& operatorName, const shared_ptr<Object>& right);
	shared_ptr<Object> evaluate_prefix_bang(const shared_ptr<Object>& value);
	shared_ptr<Object> evaluate_prefix_minus(const shared_ptr<Object>& value);
	shared_ptr<Object> evaluate_infix(const shared_ptr<Object>& left, const string& operatorName, const shared_ptr<Object>& right);
	shared_ptr<Object> evaluate_block(const shared_ptr<BlockStat>& node, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_if(const shared_ptr<IfExpr>& node, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_program(const shared_ptr<Program>& node, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_id(const shared_ptr<IdentifierExpr>& id, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_fun(const shared_ptr<Object>& fun, const vector<shared_ptr<Object>>& args);
	shared_ptr<Object> evaluate_infix_string(const shared_ptr<Object>& left, const string& operatorName, const shared_ptr<Object>& right);
	shared_ptr<Object> evaluate_infix_number(const shared_ptr<Object>& left, const string& operatorName, const shared_ptr<Object>& right);
	vector<shared_ptr<Object>> evaluate_exprs(const shared_ptr<ExpressionsStat>& exprs, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_index(const shared_ptr<Object>& left, const shared_ptr<Object>& index);
	shared_ptr<Object> evaluate_index_array(const shared_ptr<Array>& array, const shared_ptr<Integer>& index);
	shared_ptr<Object> evaluate_in_decrement(const string& id, const string& operatorName, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_assign(const shared_ptr<AssignExpr>& expr, const shared_ptr<Object>& value, const shared_ptr<Environment>& env);
	shared_ptr<Object> evaluate_assign_index(const shared_ptr<IndexExpr>& expr, const shared_ptr<Object>& value, const string& operatorName, const shared_ptr<Environment>& env);

public:
	static const shared_ptr<Bool> bool_true;
	static const shared_ptr<Bool> bool_false;
	static const shared_ptr<Null> null;
	static const map<string, shared_ptr<BuiltinFun>> builtinFuns;
};


}