#include <gtest/gtest.h>
#include <memory>
#include "object/Integer.hpp"
#include "object/Error.hpp"
#include "object/Function.hpp"
#include "object/String.hpp"
#include "object/Float.hpp"
#include "object/Array.hpp"
#include "initialization.h"
#include "evaluator/Evaluator.h"

namespace li::test
{


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
		testEqual(initEvaluator(input), make_shared<Integer>(value));
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
		testEqual(initEvaluator(input), make_shared<Float>(value));
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
		testEqual(initEvaluator(input), Evaluator::bool_to_object(value));
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
		if (value == 0)
		{
			testEqual(initEvaluator(input), Evaluator::null);
		}
		else
		{
			testEqual(initEvaluator(input), make_shared<Integer>(value));
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
		testEqual(initEvaluator(input), make_shared<Integer>(value));
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
		{ R"(_builtin_(1, 12))", "error - invalid arguments: unknown function for argument type integer" },
		{ R"(let a = 11; let a = 2)", "error - repeat declaration: a" }
	};

	for (const auto& [input, error] : tests)
	{
		SCOPED_TRACE(input);
		testEqual(initEvaluator(input), make_shared<Error>(error));
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
		testEqual(initEvaluator(input), make_shared<Integer>(value));
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
		testEqual(initEvaluator(input), make_shared<Integer>(value));
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
		testEqual(initEvaluator(input), make_shared<String>(value));
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
		testEqual(initEvaluator(input), make_shared<Integer>(value));
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
		testEqual(initEvaluator(input), make_shared<Integer>(value));
	}
}

TEST(EvaluatorTest, evaluateArray)
{
	string input = R"([1, "Hello world!", 12 / 3])";
	auto value = make_shared<Array>(Array({
		make_shared<Integer>(1),
		make_shared<String>("Hello world!"),
		make_shared<Integer>(4)}));

	testEqual(initEvaluator(input), value);
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
		{ "let a = [2, 4, 6]; a[2] += 12; a[2]", 18 },
		{ "let a = [1]; let change = fun(array) { array[0] = 11 }(a); a[0]", 1 }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testEqual(initEvaluator(input), make_shared<Integer>(value));
	}

	// Out of range will return null
	testEqual(initEvaluator("[2, 4, 6][3]"), Evaluator::null);
	testEqual(initEvaluator("[2, 4, 6][-1]"), Evaluator::null);
}

TEST(EvaluatorTest, evaluateWhile)
{
	string input = "let sum = 0; let index = 1; while (index <= 100) { sum = sum + index; index = index + 1 }; sum";
	testEqual(initEvaluator(input), make_shared<Integer>(5050));
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
		testEqual(initEvaluator(input), make_shared<Integer>(value));
	}
}


}