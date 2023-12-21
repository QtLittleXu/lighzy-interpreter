#include <gtest/gtest.h>
#include "parser/Parser.h"
#include "ast/ExpressionStatement.hpp"
#include "ast/IntegerExpression.hpp"
#include "ast/PrefixExpression.hpp"
#include "ast/InfixExpression.hpp"
#include "ast/BoolExpression.hpp"
#include "ast/IfExpression.hpp"
#include "ast/FunctionExpression.hpp"
#include "ast/CallExpression.hpp"

namespace li::test
{


TEST(ParserTest, toString)
{
	auto letS = make_shared<LetStatement>(make_shared<Token>("let", Token::Let));

	auto name = make_shared<IdentifierExpression>(make_shared<Token>("testName", Token::Identifier));
	name->setValue("testName");
	letS->setName(name);
	
	{
		auto value = make_shared<PrefixExpression>(make_shared<Token>("-", Token::Minus));
		value->setOperator("-");
		auto integer = make_shared<IntegerExpression>(make_shared<Token>("1", Token::Integer));
		integer->setValue(1);
		value->setRight(integer);
		letS->setValue(value);
	}

	auto returnS = make_shared<ReturnStatement>(make_shared<Token>("return", Token::Return));
	
	{
		auto left = make_shared<IntegerExpression>(make_shared<Token>("114514", Token::Integer));
		left->setValue(114514);
		auto value = make_shared<InfixExpression>(make_shared<Token>("+", Token::Plus));
		auto right = make_shared<IntegerExpression>(make_shared<Token>("1919810", Token::Integer));
		right->setValue(1919810);
		value->setOperator("+");
		value->setLeft(left);
		value->setRight(right);
		returnS->setValue(value);
	}

	auto program = make_shared<Program>(nullptr);
	program->addStatement(letS);
	program->addStatement(returnS);

	EXPECT_EQ(program->toString(), "let testName = -1; return (114514 + 1919810); ");
}

void checkParserOutputs(const shared_ptr<Parser>& parser)
{
	for (const auto& output : parser->outputs())
	{
		cerr << output << '\n';
	}

	EXPECT_TRUE(parser->outputs().empty());
}

void initProgram(shared_ptr<Program>& program, const string& input, uint statementSize)
{
	auto lexer = make_shared<Lexer>(input);
	auto parser = make_shared<Parser>(lexer);
	program = parser->parseProgram();

	ASSERT_NO_FATAL_FAILURE(checkParserOutputs(parser));
	ASSERT_EQ(program->statements().size(), statementSize);
}

void testLetStaement(const shared_ptr<Statement>& statement, const string& name, const string& value)
{
	auto temp = dynamic_pointer_cast<LetStatement>(statement);
	ASSERT_TRUE(temp);

	EXPECT_EQ(temp->literal(), "let");
	EXPECT_EQ(temp->name()->toString(), name);
	EXPECT_EQ(temp->value()->toString(), value);
}

void testReturnStaement(const shared_ptr<Statement>& statement, const string& value)
{
	auto temp = dynamic_pointer_cast<ReturnStatement>(statement);
	ASSERT_TRUE(temp);

	EXPECT_EQ(temp->literal(), "return");
	EXPECT_EQ(temp->value()->toString(), value);
}

void testIdentifierExpr(const shared_ptr<Expression>& expr, const string& name)
{
	auto temp = dynamic_pointer_cast<IdentifierExpression>(expr);
	ASSERT_TRUE(temp);

	EXPECT_EQ(temp->toString(), name);
}

void testIntegerExpr(const shared_ptr<Expression>& expr, const string& value)
{
	auto temp = dynamic_pointer_cast<IntegerExpression>(expr);
	ASSERT_TRUE(temp);

	EXPECT_EQ(temp->toString(), value);
}

void testPrefixExpr(const shared_ptr<Expression>& expr, const string& operatorName, const string& right)
{
    auto temp = dynamic_pointer_cast<PrefixExpression>(expr);
    ASSERT_TRUE(temp);

    EXPECT_EQ(temp->operatorName(), operatorName);
	EXPECT_EQ(temp->right()->toString(), right);
}

void testInfixExpr(const shared_ptr<Expression>& expr, const string& left, const string& operatorName, const string& right)
{
    auto temp = dynamic_pointer_cast<InfixExpression>(expr);
    ASSERT_TRUE(temp);

    EXPECT_EQ(temp->left()->toString(), left);
    EXPECT_EQ(temp->operatorName(), operatorName);
    EXPECT_EQ(temp->right()->toString(), right);
}

void testBoolExpr(const shared_ptr<Expression>& expr, const string& value)
{
	auto temp = dynamic_pointer_cast<BoolExpression>(expr);
    ASSERT_TRUE(temp);

    EXPECT_EQ(temp->toString(), value);
}

void testIfExpr(const shared_ptr<Expression>& expr, const string& condition, const string& consequence, const string& alternative)
{
	auto temp = dynamic_pointer_cast<IfExpression>(expr);
    ASSERT_TRUE(temp);

	EXPECT_EQ(temp->condition()->toString(), condition);
	EXPECT_EQ(temp->consequence()->toString(), consequence);
	EXPECT_EQ(temp->alternative()->toString(), alternative);
}

void testFunctionExpr(const shared_ptr<Expression>& expr, const string& args, const string& body)
{
	auto temp = dynamic_pointer_cast<FunctionExpression>(expr);
    ASSERT_TRUE(temp);

	EXPECT_EQ(temp->body()->toString(), body);
	EXPECT_EQ(temp->args()->toString(), args);
}

void testCallExpr(const shared_ptr<Expression>& expr, const string& fun, const string& exprs)
{
	auto temp = dynamic_pointer_cast<CallExpression>(expr);
    ASSERT_TRUE(temp);

	EXPECT_EQ(temp->fun()->toString(), fun);
	EXPECT_EQ(temp->exprs()->toString(), exprs);
}

void testOperatorPrecedence(const shared_ptr<Expression>& expr, const string& result)
{
    ASSERT_TRUE(expr);
	EXPECT_EQ(expr->toString(), result);
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
		{ "3 + 4 / add(1, 2)", "(3 + (4 / add(1, 2)))" }
	};

	for (const auto& [input, result] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initProgram(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testOperatorPrecedence(statement->expression(), result));
	}
}

TEST(ParserTest, letStatement)
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
		ASSERT_NO_FATAL_FAILURE(initProgram(program, input, 1));

		auto statement = dynamic_pointer_cast<LetStatement>(program->statements().at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testLetStaement(statement, expectedName, expectedValue));
	}
}

TEST(ParserTest, returnStatement)
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
		ASSERT_NO_FATAL_FAILURE(initProgram(program, input, 1));

		testReturnStaement(program->statements().at(0), expectedValue);
	}
}

TEST(ParserTest, identifierExpression)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initProgram(program, "myabc", 1));

    auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testIdentifierExpr(statement->expression(), "myabc"));
}

TEST(ParserTest, integerExpression)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initProgram(program, "114514", 1));

	auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testIntegerExpr(statement->expression(), "114514"));
}

TEST(ParserTest, prefixExpression)
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
		ASSERT_NO_FATAL_FAILURE(initProgram(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testPrefixExpr(statement->expression(), expectedOperator, expectedRight));
	}
}

TEST(ParserTest, infixExpression)
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
		{ "1 == 2", "1", "==", "2" },
		{ "1 != 2", "1", "!=", "2" },
		{ "1 >= 2", "1", ">=", "2" },
		{ "1 <= 2", "1", "<=", "2" }
	};

	for (const auto& [input, left, operatorName, right] : tests)
	{
		SCOPED_TRACE(input);

		shared_ptr<Program> program;
		ASSERT_NO_FATAL_FAILURE(initProgram(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testInfixExpr(statement->expression(), left, operatorName, right));
	}
}

TEST(ParserTest, boolExpression)
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
		ASSERT_NO_FATAL_FAILURE(initProgram(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testBoolExpr(statement->expression(), value));
	}
}

TEST(ParserTest, ifExpression)
{
	shared_ptr<Program> program;
	ASSERT_NO_FATAL_FAILURE(initProgram(program, "if (x == y) { x } else { y }", 1));

    auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
	ASSERT_TRUE(statement);

	ASSERT_NO_FATAL_FAILURE(testIfExpr(statement->expression(), "(x == y)", "{ x;  }", "{ y;  }"));
}

TEST(ParserTest, functionExpression)
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
		ASSERT_NO_FATAL_FAILURE(initProgram(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testFunctionExpr(statement->expression(), args, body));
	}
}

TEST(ParserTest, callExpression)
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
		ASSERT_NO_FATAL_FAILURE(initProgram(program, input, 1));

		auto statement = dynamic_pointer_cast<ExpressionStatement>(program->statements().at(0));
		ASSERT_TRUE(statement);

		ASSERT_NO_FATAL_FAILURE(testCallExpr(statement->expression(), fun, exprs));
	}
}


} // namespace test