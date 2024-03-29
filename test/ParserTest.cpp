#include <gtest/gtest.h>
#include "parser/Parser.h"
#include "ast/ExpressionStat.hpp"
#include "ast/IntegerExpr.hpp"
#include "ast/PrefixExpr.hpp"
#include "ast/InfixExpr.hpp"
#include "ast/BoolExpr.hpp"
#include "ast/IfExpr.hpp"
#include "ast/FunctionExpr.hpp"
#include "ast/CallExpr.hpp"
#include "ast/StringExpr.hpp"
#include "ast/AssignExpr.hpp"
#include "ast/FloatExpr.hpp"
#include "ast/ArrayExpr.hpp"
#include "ast/IndexExpr.hpp"
#include "ast/WhileStat.hpp"
#include "ast/VarStat.hpp"
#include "ast/InDecrementExpr.hpp"
#include "initialization.h"

namespace li::test
{


TEST(ParserTest, toString)
{
	auto letS = make_shared<LetStat>(make_shared<Token>("let", Token::Let));

	auto name = make_shared<IdentifierExpr>(make_shared<Token>("testName", Token::Identifier));
	name->value = "testName";
	letS->name = name;
	
	{
		auto value = make_shared<PrefixExpr>(make_shared<Token>("-", Token::Minus));
		value->operatorName = "-";
		auto integer = make_shared<IntegerExpr>(make_shared<Token>("1", Token::Integer));
		integer->value = 1;
		value->right = integer;
		letS->value = value;
	}

	auto returnS = make_shared<ReturnStat>(make_shared<Token>("return", Token::Return));
	
	{
		auto left = make_shared<IntegerExpr>(make_shared<Token>("114514", Token::Integer));
		left->value = 114514;
		auto value = make_shared<InfixExpr>(make_shared<Token>("+", Token::Plus));
		auto right = make_shared<IntegerExpr>(make_shared<Token>("1919810", Token::Integer));
		right->value = 1919810;
		value->operatorName = "+";
		value->left = left;
		value->right = right;
		returnS->value = value;
	}

	auto program = make_shared<Program>(nullptr);
	program->statements.push_back(letS);
	program->statements.push_back(returnS);

	EXPECT_EQ(program->toString(), "let testName = -1; return (114514 + 1919810); ");
}

void testLetStatement(shared_ptr<Stat> statement, const string& name, const string& value)
{
	auto temp = dynamic_pointer_cast<LetStat>(statement);
	ASSERT_TRUE(temp);

	EXPECT_EQ(temp->literal(), "let");
	EXPECT_EQ(temp->name->toString(), name);
	EXPECT_EQ(temp->value->toString(), value);
}

void testReturnStatement(shared_ptr<Stat> statement, const string& value)
{
	auto temp = dynamic_pointer_cast<ReturnStat>(statement);
	ASSERT_TRUE(temp);

	EXPECT_EQ(temp->literal(), "return");
	EXPECT_EQ(temp->value->toString(), value);
}

void testIdentifierExpr(shared_ptr<Expr> expr, const string& name)
{
	auto temp = dynamic_pointer_cast<IdentifierExpr>(expr);
	ASSERT_TRUE(temp);

	EXPECT_EQ(temp->toString(), name);
}

void testIntegerExpr(shared_ptr<Expr> expr, const string& value)
{
	auto temp = dynamic_pointer_cast<IntegerExpr>(expr);
	ASSERT_TRUE(temp);

	EXPECT_EQ(temp->toString(), value);
}

void testFloatExpr(shared_ptr<Expr> expr, const string& value)
{
	ASSERT_EQ(expr->type, Node::Type::Float);
	auto cast = dynamic_pointer_cast<FloatExpr>(expr);

	EXPECT_EQ(cast->toString(), value);
}

void testPrefixExpr(shared_ptr<Expr> expr, const string& operatorName, const string& right)
{
    auto temp = dynamic_pointer_cast<PrefixExpr>(expr);
    ASSERT_TRUE(temp);

    EXPECT_EQ(temp->operatorName, operatorName);
	EXPECT_EQ(temp->right->toString(), right);
}

void testInfixExpr(shared_ptr<Expr> expr, const string& left, const string& operatorName, const string& right)
{
    auto temp = dynamic_pointer_cast<InfixExpr>(expr);
    ASSERT_TRUE(temp);

    EXPECT_EQ(temp->left->toString(), left);
    EXPECT_EQ(temp->operatorName, operatorName);
    EXPECT_EQ(temp->right->toString(), right);
}

void testBoolExpr(shared_ptr<Expr> expr, const string& value)
{
	auto temp = dynamic_pointer_cast<BoolExpr>(expr);
    ASSERT_TRUE(temp);

    EXPECT_EQ(temp->toString(), value);
}

void testIfExpr(shared_ptr<Expr> expr, const string& condition, const string& consequence, const string& alternative)
{
	auto temp = dynamic_pointer_cast<IfExpr>(expr);
    ASSERT_TRUE(temp);

	EXPECT_EQ(temp->condition->toString(), condition);
	EXPECT_EQ(temp->consequence->toString(), consequence);
	EXPECT_EQ(temp->alternative->toString(), alternative);
}

void testFunctionExpr(shared_ptr<Expr> expr, const string& args, const string& body)
{
	auto temp = dynamic_pointer_cast<FunctionExpr>(expr);
    ASSERT_TRUE(temp);

	EXPECT_EQ(temp->body->toString(), body);
	EXPECT_EQ(temp->args->toString(), args);
}

void testCallExpr(shared_ptr<Expr> expr, const string& fun, const string& exprs)
{
	auto temp = dynamic_pointer_cast<CallExpr>(expr);
    ASSERT_TRUE(temp);

	EXPECT_EQ(temp->fun->toString(), fun);
	EXPECT_EQ(temp->exprs->toString(), exprs);
}

void testOperatorPrecedence(shared_ptr<Expr> expr, const string& result)
{
    ASSERT_TRUE(expr);
	EXPECT_EQ(expr->toString(), result);
}

void testStringExpr(shared_ptr<Expr> expr, const string& value)
{
	ASSERT_EQ(expr->type, Node::Type::String);
	auto cast = dynamic_pointer_cast<StringExpr>(expr);

	EXPECT_EQ(cast->value, value);
}

void testAssignExpr(shared_ptr<Expr> expr, const string& id, const string& operatorName, const string& value)
{
	ASSERT_EQ(expr->type, Node::Type::Assign);
	auto cast = dynamic_pointer_cast<AssignExpr>(expr);

	EXPECT_EQ(cast->id->toString(), id);
	EXPECT_EQ(cast->operatorName, operatorName);
	EXPECT_EQ(cast->value->toString(), value);
}

void testInDecrementExpr(shared_ptr<Expr> expr, const string& id, const string& operatorName)
{
	ASSERT_EQ(expr->type, Node::Type::InDecrement);
	auto cast = dynamic_pointer_cast<InDecrementExpr>(expr);

	EXPECT_EQ(cast->id->toString(), id);
	EXPECT_EQ(cast->operatorName, operatorName);
}

TEST(ParserTest, operatorPrecedence)
{
	struct Expected
	{
		string input;
		string result;
	} tests[] = {
		{ "1 + 1", "(1 + 1)" },
		{ "1 + 2 + 3", "((1 + 2) + 3)" },
		{ "1 - 2 * 3 + 4", "((1 - (2 * 3)) + 4)" },
		{ "1 + if (a > b) { a } else { b }", "(1 + if ((a > b)) { a;  } else { b;  })" },
		{ "add(1, 2) + 3", "(add(1, 2) + 3)"},
		{ "3 + 4 / add(1, 2)", "(3 + (4 / add(1, 2)))" },
		{ "1 + [a, b, c][2] * 2", "(1 + ([a, b, c][2] * 2))" },
		{ "testFun(12)[1]",  "testFun(12)[1]"}
	};

	for (const auto& [input, result] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testOperatorPrecedence(statement->expression, result));
	}
}

TEST(ParserTest, LetStat)
{
	struct Expected
	{
		string input;
		string name;
		string value;
	} tests[] = {
		{ "let a = 12", "a", "12" },
		{ "let b = a + 1", "b", "(a + 1)" }
	};

	for (const auto& [input, expectedName, expectedValue] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<LetStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testLetStatement(statement, expectedName, expectedValue));
	}
}

TEST(ParserTest, VarTest)
{
	struct Expected
	{
		string input;
		string name;
		string value;
	} tests[] = {
		{ "var a = 12", "a", "12" },
		{ "var b = a + 1", "b", "(a + 1)" }
	};

	for (const auto& [input, expectedName, expectedValue] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto stat = dynamic_pointer_cast<VarStat>(program->statements.at(0));
		ASSERT_TRUE(stat);

		EXPECT_EQ(stat->name->value, expectedName);
		EXPECT_EQ(stat->value->toString(), expectedValue);
	}
}

TEST(ParserTest, ReturnStat)
{
	struct Expected
	{
		string input;
		string value;
	} tests[] = {
		{ "return 11 - 2", "(11 - 2)" },
		{ "return a - b", "(a - b)" }
	};

	for (const auto& [input, expectedValue] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		testReturnStatement(program->statements.at(0), expectedValue);
	}
}

TEST(ParserTest, IdentifierExpr)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initParser(program, "myabc", 1));

    auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testIdentifierExpr(statement->expression, "myabc"));
}

TEST(ParserTest, IntegerExpr)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initParser(program, "114514", 1));

	auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testIntegerExpr(statement->expression, "114514"));
}

TEST(ParserTest, FloatExpr)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initParser(program, "1919.81", 1));

	auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testFloatExpr(statement->expression, "1919.81"));
}

TEST(ParserTest, PrefixExpr)
{
	struct Expected
	{
		string input;
		string operatorName;
		string right;
	} tests[] = {
		{ "!5", "!", "5"},
		{ "!11", "!", "11"},
		{ "-120", "-", "120" }
	};

	for (const auto& [input, expectedOperator, expectedRight] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testPrefixExpr(statement->expression, expectedOperator, expectedRight));
	}
}

TEST(ParserTest, InfixExpr)
{
	struct Expected
	{
		string input;
		string left;
		string operatorName;
		string right;
	} tests[] = {
		{ "1 + 2", "1", "+", "2" },
		{ "1 - 2", "1", "-", "2" },
		{ "1 * 2", "1", "*", "2" },
		{ "1 / 2", "1", "/", "2" },
		{ "12 % 5", "12", "%", "5" },
		{ "1 == 2", "1", "==", "2" },
		{ "1 != 2", "1", "!=", "2" },
		{ "1 >= 2", "1", ">=", "2" },
		{ "1 <= 2", "1", "<=", "2" }
	};

	for (const auto& [input, left, operatorName, right] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testInfixExpr(statement->expression, left, operatorName, right));
	}
}

TEST(ParserTest, BoolExpr)
{
	struct Expected
	{
		string input;
		string value;
	} tests[] = {
		{ "true", "true"},
		{ "false", "false"}
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testBoolExpr(statement->expression, value));
	}
}

TEST(ParserTest, IfExpr)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initParser(program, "if (x == y) { x } else { y }", 1));

    auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testIfExpr(statement->expression, "(x == y)", "{ x;  }", "{ y;  }"));
}

TEST(ParserTest, FunctionExpr)
{
	struct Expected
	{
		string input;
		string args;
		string body;
	} tests[] = {
		{ "fun(a) { a }", "(a)", "{ a;  }" },
		{ "fun(first, second) { first + second }", "(first, second)", "{ (first + second);  }" }
	};

	for (const auto& [input, args, body] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testFunctionExpr(statement->expression, args, body));
	}
}

TEST(ParserTest, CallExpr)
{
	struct Expected
	{
		string input;
		string fun;
		string exprs;
	} tests[] = {
		{ "add(1, 2)", "add", "(1, 2)" },
		{ "minus(1 * 3, 2 / a)", "minus", "((1 * 3), (2 / a))" }
	};

	for (const auto& [input, fun, exprs] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testCallExpr(statement->expression, fun, exprs));
	}
}

TEST(ParserTest, StringExpr)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initParser(program, "\"Hello world!\"", 1));

    auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testStringExpr(statement->expression, "Hello world!"));
}

TEST(ParserTest, AssignExpr)
{
	struct Expected
	{
		string input;
		string id;
		string operatorName;
		string value;
	} tests[] = {
		{ "test = 2", "test", "=", "2" },
		{ "num = 12 - 2 / 3", "num", "=", "(12 - (2 / 3))" },
		{ "test = num = 2", "test", "=", "num = 2" },
		{ "a += 2", "a", "+=", "2" },
		{ "a -= 2", "a", "-=", "2" },
		{ "a *= 2", "a", "*=", "2" },
		{ "a /= 2", "a", "/=", "2" },
		{ "a %= 2", "a", "%=", "2" }
	};

	for (const auto& [input, id, operatorName, value] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testAssignExpr(statement->expression, id, operatorName, value));
	}
}

TEST(ParserTest, errorHandling)
{
	struct Expected
	{
		string input;
		string error;
	} tests[] = {
		{ "let a 2", R"(char 7: error: expected token type to be assign("="), but got integer)" },
		{ "let = 4", R"(char 7: error: expected token type to be identifier, but got assign("="))" }
	};

	for (const auto& [input, error] : tests)
	{
		SCOPED_TRACE(input);

		auto lexer = make_shared<Lexer>(input);
		auto parser = make_shared<Parser>(lexer);
		auto program = parser->parseProgram();

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		EXPECT_EQ(parser->outputs().at(0), error);
	}
}

TEST(ParserTest, ArrayExpr)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initParser(program, R"([1, 1 * 3, "Hello world!"])", 1));

    auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
	ASSERT_TRUE(statement);

	auto exprs = dynamic_pointer_cast<ArrayExpr>(statement->expression)->elements->expressions;
	ASSERT_NO_FATAL_FAILURE(testIntegerExpr(exprs.at(0), "1"));
	ASSERT_NO_FATAL_FAILURE(testInfixExpr(exprs.at(1), "1", "*", "3"));
	ASSERT_NO_FATAL_FAILURE(testStringExpr(exprs.at(2), "Hello world!"));
}

TEST(ParserTest, IndexExpr)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initParser(program, "testArray[12]", 1));

    auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
	ASSERT_TRUE(statement);

	auto indexExpr = dynamic_pointer_cast<IndexExpr>(statement->expression);

	ASSERT_NO_FATAL_FAILURE(testIdentifierExpr(indexExpr->left, "testArray"));
	ASSERT_NO_FATAL_FAILURE(testIntegerExpr(indexExpr->index, "12"));
}

TEST(ParserTest, WhileStat)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initParser(program, "while(a < 2) { a + 1 }", 1));

    auto statement = dynamic_pointer_cast<WhileStat>(program->statements.at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testInfixExpr(statement->condition, "a", "<", "2"));
	auto expr = dynamic_pointer_cast<ExpressionStat>(statement->body->statements.at(0));
	ASSERT_NO_FATAL_FAILURE(testInfixExpr(expr->expression, "a", "+", "1"));
}

TEST(ParserTest, InDecrementExpr)
{
	struct Expected
	{
		string input;
		string id;
		string operatorName;
	} tests[] = {
		{ "++test", "test", "++" },
		{ "--num", "num", "--" }
	};

	for (const auto& [input, id, value] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initParser(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStat>(program->statements.at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testInDecrementExpr(statement->expression, id, value));
	}
}


} // namespace test