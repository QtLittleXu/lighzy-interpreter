#pragma once

#include "ast/BoolExpression.hpp"
#include "ast/IfExpression.hpp"
#include "ast/Program.hpp"
#include "ast/BlockStatement.hpp"
#include "object/basic/Object.h"
#include "object/Bool.hpp"
#include "object/Null.hpp"

namespace li
{


class Evaluator
{
public:
	shared_ptr<Object> evaluate(const shared_ptr<Node>& node);

private:
	bool is_true(const shared_ptr<Object>& obj);

	shared_ptr<Object> unknown_prefix_error(const string& operatorName, const string& right);
	shared_ptr<Object> unknown_infix_error(const string& left, const string& operatorName, const string& right);
	shared_ptr<Object> infix_operand_type_mismatch(const string& left, const string& operatorName, const string& right);
	shared_ptr<Object> prefix_operand_type_error(const string& operatorName, const string& right);

private:
	const shared_ptr<Bool>& evaluate_bool(const shared_ptr<BoolExpression>& node);
	const shared_ptr<Bool>& bool_to_object(bool value);

	shared_ptr<Object> evaluate_prefix(const string& operatorName, const shared_ptr<Object>& right);
	shared_ptr<Object> evaluate_prefix_bang(const shared_ptr<Object>& value);
	shared_ptr<Object> evaluate_prefix_minus(const shared_ptr<Object>& value);
	shared_ptr<Object> evaluate_infix(const shared_ptr<Object>& left, const string& operatorName, const shared_ptr<Object>& right);
	shared_ptr<Object> evaluate_block(const shared_ptr<BlockStatement>& node);
	shared_ptr<Object> evaluate_if(const shared_ptr<IfExpression>& node);
	shared_ptr<Object> evaluate_program(const shared_ptr<Program>& node);
	shared_ptr<Object> evaluate_infix_integer(
		const shared_ptr<Object>& left,
		const string& operatorName,
		const shared_ptr<Object>& right);

public:
	static const shared_ptr<Bool> bool_true;
	static const shared_ptr<Bool> bool_false;
	static const shared_ptr<Null> null;
};


}