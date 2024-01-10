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
	shared_ptr<Object> evaluate(shared_ptr<Node> node, shared_ptr<Environment> env);

public:
	bool is_true(shared_ptr<Object> obj);

	static shared_ptr<Object> unknown_prefix(const string& operatorName, const string& right);
	static shared_ptr<Object> unknown_infix(const string& left, const string& operatorName, const string& right);
	static shared_ptr<Object> infix_operand_type_mismatch(const string& left, const string& operatorName, const string& right);
	static shared_ptr<Object> operand_type_error(const string& type, const string& operatorName, const string& right);
	static shared_ptr<Object> identifier_not_found(const string& name);
	static shared_ptr<Object> not_function(const string& type);
	static shared_ptr<Object> invalid_arguments(const string& msg);
	static shared_ptr<Object> index_operand_type(const string& left, const string& index);
	static shared_ptr<Bool> bool_to_object(bool value);

private:
	shared_ptr<Bool> evaluate_bool(shared_ptr<BoolExpr> node);
	tuple<shared_ptr<Object>, shared_ptr<Environment>> bind_fun_args_to_objects(shared_ptr<Function> fun, const vector<shared_ptr<Object>>& objects);

	shared_ptr<Object> evaluate_prefix(const string& operatorName, shared_ptr<Object> right);
	shared_ptr<Object> evaluate_prefix_bang(shared_ptr<Object> value);
	shared_ptr<Object> evaluate_prefix_minus(shared_ptr<Object> value);
	shared_ptr<Object> evaluate_infix(shared_ptr<Object> left, const string& operatorName, shared_ptr<Object> right);
	shared_ptr<Object> evaluate_block(shared_ptr<BlockStat> node, shared_ptr<Environment> env);
	shared_ptr<Object> evaluate_if(shared_ptr<IfExpr> node, shared_ptr<Environment> env);
	shared_ptr<Object> evaluate_program(shared_ptr<Program> node, shared_ptr<Environment> env);
	shared_ptr<Object> evaluate_id(shared_ptr<IdentifierExpr> id, shared_ptr<Environment> env);
	shared_ptr<Object> evaluate_fun(shared_ptr<Object> fun, const vector<shared_ptr<Object>>& args);
	shared_ptr<Object> evaluate_infix_string(shared_ptr<Object> left, const string& operatorName, shared_ptr<Object> right);
	shared_ptr<Object> evaluate_infix_number(shared_ptr<Object> left, const string& operatorName, shared_ptr<Object> right);
	vector<shared_ptr<Object>> evaluate_exprs(shared_ptr<ExpressionsStat> exprs, shared_ptr<Environment> env);
	shared_ptr<Object> evaluate_index(shared_ptr<Object> left, shared_ptr<Object> index);
	shared_ptr<Object> evaluate_index_array(shared_ptr<Array> array, shared_ptr<Integer> index);
	shared_ptr<Object> evaluate_in_decrement(shared_ptr<Object> id, const string& operatorName, shared_ptr<Environment> env);
	shared_ptr<Object> evaluate_assign(shared_ptr<Object> id, const string& operatorName, shared_ptr<Object> value, shared_ptr<Environment> env);

public:
	static const shared_ptr<Bool> bool_true;
	static const shared_ptr<Bool> bool_false;
	static const shared_ptr<Null> null;
	static const map<string, shared_ptr<BuiltinFun>> builtinFuns;
};


}