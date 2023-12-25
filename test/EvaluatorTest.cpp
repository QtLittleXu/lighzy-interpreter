#include <gtest/gtest.h>
#include <memory>
#include "parser/Parser.h"
#include "evaluator/Evaluator.h"
#include "object/Integer.hpp"
#include "object/Bool.hpp"
#include "object/Error.hpp"
#include "object/Function.hpp"
#include "object/String.hpp"

namespace li::test
{


shared_ptr<Object> initEvaluator(const string& input)
{
	auto lexer = make_shared<Lexer>(input);
	auto parser = make_shared<Parser>(lexer);
	auto program = parser->parseProgram();
	auto evaluator = make_shared<Evaluator>();
	auto env = make_shared<Environment>();

	return evaluator->evaluate(program, env);
}

void testInteger(const shared_ptr<Object>& obj, int64_t value)
{
	ASSERT_EQ(obj->typeName(), "integer");

	auto cast = dynamic_pointer_cast<Integer>(obj);
	EXPECT_EQ(cast->value(), value);
}

void testBool(const shared_ptr<Object>& obj, bool value)
{
	ASSERT_EQ(obj->typeName(), "bool");

	auto cast = dynamic_pointer_cast<Bool>(obj);
	EXPECT_EQ(cast->value(), value);
}

void testString(const shared_ptr<Object>& obj, const string& value)
{
	ASSERT_EQ(obj->typeName(), "string");

	auto cast = dynamic_pointer_cast<String>(obj);
	EXPECT_EQ(cast->value(), value);
}

void testNull(const shared_ptr<Object>& obj)
{
	ASSERT_EQ(obj->typeName(), "null");

	auto cast = dynamic_pointer_cast<Null>(obj);
}

void testError(const shared_ptr<Object>& obj, const string& message)
{
	ASSERT_EQ(obj->typeName(), "error");

	auto cast = dynamic_pointer_cast<Error>(obj);
	EXPECT_EQ(cast->message(), message);
}

TEST(EvaluatorTest, evaluateInteger)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "11", 11 },
		{ "114514", 114514 },
		{ "-1", -1 },
		{ "--1", 1 },
		{ "1 + 1", 2 },
		{ "2 - 1", 1 },
		{ "12 * 12", 144 },
		{ "33 / 3", 11 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}

TEST(EvaluatorTest, evaluateBool)
{
	struct Expected
	{
		string input;
		bool value;
	} tests[] = {
		{ "true", true },
		{ "false", false },
		{ "!true", false },
		{ "!false", true },
		{ "!!false", false },
		{ "!6", false },
		{ "!0", true },
		{ "true == true", true },
		{ "true != true", false },
		{ "true == false", false },
		{ "true != false", true },
		{ "1 == 2", false },
		{ "1 > 2", false },
		{ "1 >= 2", false },
		{ "1 != 2", true },
		{ "1 < 2", true },
		{ "1 <= 2", true },
		{ "!true != false", false },
		{ "(11 != 13) == true", true }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testBool(initEvaluator(input), value);
	}
}

TEST(EvalautorTest, evaluateIf)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "if (true) { 10 }", 10 },
		{ "if (false) { 10 }", 0 },
		{ "if (1 < 2) { 1 } else { 2 }", 1 },
		{ "if (2 < 1) { 1 } else { 2 }", 2 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		
		auto obj = initEvaluator(input);
		if (value == 0)
		{
			testNull(obj);
		}
		else
		{
			testInteger(obj, value);
		}
	}
}

TEST(EvaluatorTest, evalauteReturn)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "return 1", 1 },
		{ "return 11 * 2", 22 },
		{ "283 return 2", 2 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}

TEST(EvaluatorTest, evaluateError)
{
	struct Expected
	{
		string input;
		string error;
	} tests[] = {
		{ "-true", "error - prefix operand type: -bool" },
		{ "1 + true", "error - infix operand type mismatch: integer + bool" },
		{ "true + 1; 1 + 1", "error - infix operand type mismatch: bool + integer" },
		{ "true + false", "error - unknown infix operator: bool + bool" },
		{ "if (11 > 2) { return true * false } return 1", "error - unknown infix operator: bool * bool" },
		{ "a", "error - identifier not found: a" },
		{ "1 + temp", "error - identifier not found: temp" },
		{ R"("Hello" - "world!")", "error - unknown infix operator: string - string" }
	};

	for (const auto& [input, error] : tests)
	{
		SCOPED_TRACE(input);
		testError(initEvaluator(input), error);
	}
}

TEST(EvaluatorTest, evaluateLet)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "let a = 1; a", 1 },
		{ "let a = 5 * 5; a", 25 },
		{ "let a = 2; let b = a; b", 2 },
		{ "let a = 11; let b = 22; let c = a + b - 1; c", 32 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}

TEST(EvaluatorTest, evaluateFunction)
{
	string input = "fun(x) { x + 2 }";
	auto evaluated = initEvaluator(input);

	ASSERT_EQ(evaluated->typeName(), "function");
	auto cast = dynamic_pointer_cast<Function>(evaluated);

	EXPECT_EQ(cast->args()->args().size(), 1);
	EXPECT_EQ(cast->body()->toString(), "{ (x + 2);  }");
}

TEST(EvaluatorTest, evaluateCall)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "let call = fun(x) { x }; call(5)", 5 },
		{ "let call = fun(x) { return x }; call(5)", 5 },
		{ "let double = fun(x) { x * 2 }; double(5)", 10 },
		{ "let add = fun(a, b) { a + b }; add(1, 1)", 2 },
		{ "let add = fun(a, b) { a + b }; add(add(1, 2), add(2, 3))", 8 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}

TEST(EvalautorTest, evaluateString)
{
	struct Expected
	{
		string input;
		string value;
	} tests[] = {
		{ R"("Hello world!")", "Hello world!" },
		{ R"("Hello" + " " + "world" + "!")", "Hello world!" }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testString(initEvaluator(input), value);
	}
}

TEST(EvalautorTest, evaluateAssign)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "let num = 11; num = 1", 1 },
		{ "let temp = 0; temp = 2; temp", 2 },
		{ "let n1 = 1; let n2 = 2; n2 = n1 = 4;", 4 },
		{ "let n1 = 1; let n2 = 2; n2 = n1 = 4; n1;", 4 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}


}