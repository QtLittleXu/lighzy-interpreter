#include <gtest/gtest.h>
#include <memory>
#include "parser/Parser.h"
#include "evaluator/Evaluator.h"
#include "object/Integer.hpp"
#include "object/Bool.hpp"
#include "object/Error.hpp"
#include "object/Function.hpp"
#include "object/String.hpp"
#include "object/Float.hpp"
#include "object/Array.hpp"

namespace li::test
{


void checkParserOutputs(const shared_ptr<Parser>& parser);

shared_ptr<Object> initEvaluator(const string& input)
{
	auto lexer = make_shared<Lexer>(input);
	auto parser = make_shared<Parser>(lexer);
	auto program = parser->parseProgram();
	checkParserOutputs(parser);

	auto evaluator = make_shared<Evaluator>();
	auto env = make_shared<Environment>();

	auto obj = evaluator->evaluate(program, env);
	if (obj->type == Object::Type::Error)
	{
		cerr << obj->inspect() << '\n';
	}
	return obj;
}

void testInteger(const shared_ptr<Object>& obj, int64_t value)
{
	ASSERT_EQ(obj->typeName(), "integer");

	auto cast = dynamic_pointer_cast<Integer>(obj);
	EXPECT_EQ(cast->value, value);
}

void testFloat(const shared_ptr<Object>& obj, double value)
{
	ASSERT_EQ(obj->typeName(), "float");

	auto cast = dynamic_pointer_cast<Float>(obj);
	EXPECT_EQ(cast->value, value);
}

void testBool(const shared_ptr<Object>& obj, bool value)
{
	ASSERT_EQ(obj->typeName(), "bool");

	auto cast = dynamic_pointer_cast<Bool>(obj);
	EXPECT_EQ(cast->value, value);
}

void testString(const shared_ptr<Object>& obj, const string& value)
{
	ASSERT_EQ(obj->typeName(), "string");

	auto cast = dynamic_pointer_cast<String>(obj);
	EXPECT_EQ(cast->value, value);
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
	EXPECT_EQ(cast->message, message);
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
		{ "1 + 1", 2 },
		{ "2 - 1", 1 },
		{ "12 * 12", 144 },
		{ "33 / 3", 11 },
		{ "12 % 5", 2 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}

TEST(EvaluatorTest, evaluateFloat)
{
	struct Expected
	{
		string input;
		double value;
	} tests[] = {
		{ "1.2", 1.2 },
		{ "0114.5140", 114.514 },
		{ "-21.2", -21.2 },
		{ "0.3 + 11.2", 0.3 + 11.2 },
		{ "1.2 - 0.3", 1.2 - 0.3 },
		{ "1.2 * 3", 1.2 * 3 },
		{ "1.44 / 1.2", 1.44 / 1.2 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testFloat(initEvaluator(input), value);
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

TEST(EvaluatorTest, evaluateIf)
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

TEST(EvaluatorTest, evaluateReturn)
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
		{ R"("Hello" - "world!")", "error - unknown infix operator: string - string" },
		{ R"(let add = fun(a, b) { a + b }; add(1))", "error - invalid arguments: expected the number of them to be 2, but got 1" },
		{ R"(_builtin_(1, "hello", "world!"))", "error - invalid arguments: expected the number of them to be 1, but got 2" },
		{ R"(_builtin_(1, 12))", "error - invalid arguments: unknown function for argument type integer" }
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

	EXPECT_EQ(cast->args->args.size(), 1);
	EXPECT_EQ(cast->body->toString(), "{ (x + 2);  }");
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

TEST(EvaluatorTest, evaluateString)
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

TEST(EvaluatorTest, evaluateAssign)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "let num = 11; num = 1", 1 },
		{ "let temp = 0; temp = 2; temp", 2 },
		{ "let n1 = 1; let n2 = 2; n2 = n1 = 4;", 4 },
		{ "let n1 = 1; let n2 = 2; n2 = n1 = 4; n1;", 4 },
		{ "let dontChange = 12; fun(dontChange) { dontChange = 1 }(0); dontChange", 12 },
		{ "let a = 12; a += 2", 14 },
		{ "let a = 12; a += 2; a", 14 },
		{ "let a = 12; a -= 2", 10 },
		{ "let a = 12; a -= 2; a", 10 },
		{ "let a = 12; a *= 2", 24 },
		{ "let a = 12; a *= 2; a", 24 },
		{ "let a = 12; a /= 2", 6 },
		{ "let a = 12; a /= 2; a", 6 },
		{ "let a = 12; a %= 5", 2 },
		{ "let a = 12; a %= 5; a", 2 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}

TEST(EvaluatorTest, evaluateBuiltInFun)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ R"(_builtin_(1, "Hello world!"))", 12 },
		{ R"(let str = "abcd efg"; _builtin_(1, str))", 8 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}

TEST(EvaluatorTest, evaluateArray)
{
	string input = R"([1, "Hello world!", 12 / 3])";
	auto evaluated = initEvaluator(input);

	ASSERT_EQ(evaluated->typeName(), "array");
	auto cast = dynamic_pointer_cast<Array>(evaluated);
	ASSERT_EQ(cast->elements.size(), 3);

	testInteger(cast->elements.at(0), 1);
	testString(cast->elements.at(1), "Hello world!");
	testInteger(cast->elements.at(2), 4);
}

TEST(EvaluatorTest, evaluateIndex)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "[2, 4, 6][0]", 2 },
		{ "[2, 4, 6][1]", 4 },
		{ "[2, 4, 6][2]", 6 },
		{ "let array = [2, 4, 6]; array[2]", 6 },
		{ "let array = [2, 4, 6]; let sum = array[1] + array[2]; sum", 10 },
		{ "let a = [1, 2, 3]; a[1] = 11", 11 },
		{ "let a = [1, 2, 3]; a[1] = 11; a[1]", 11 },
		{ "let a = [2, 4, 6]; a[2] += 12", 18 },
		{ "let a = [2, 4, 6]; a[2] += 12; a[2]", 18 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}

	// Out of range will return null
	testNull(initEvaluator("[2, 4, 6][3]"));
	testNull(initEvaluator("[2, 4, 6][-1]"));
}

TEST(EvaluatorTest, evaluateWhile)
{
	string input = "let sum = 0; let index = 1; while (index <= 100) { sum = sum + index; index = index + 1 }; sum";
	auto evaluated = initEvaluator(input);
	ASSERT_EQ(evaluated->typeName(), "integer");

	testInteger(evaluated, 5050);
}

TEST(EvaluatorTest, evaluateInDecrement)
{
	struct Expected
	{
		string input;
		int64_t value;
	} tests[] = {
		{ "let a = 12; ++a", 13 },
		{ "let a = 3; --a", 2 },
		{ "let a = 19; ++a; a", 20 },
		{ "let a = 6; --a; a", 5 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testInteger(initEvaluator(input), value);
	}
}


}