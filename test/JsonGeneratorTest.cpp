#include <gtest/gtest.h>
#include <fstream>
#include "ast/visual/JsonGenerator.hpp"
#include "lexer/Lexer.h"
#include "parser/Parser.h"

namespace li::test
{


void readTestFile(string& str)
{
	ifstream file("input.li");
	ASSERT_TRUE(file);
	str = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

void writeTestFile(const string& str)
{
	ofstream file("ouput.json");
	ASSERT_TRUE(file);
	file << str;
}

TEST(JsonGeneratorTest, generate)
{
	string input;
	readTestFile(input);
	auto lexer = make_shared<Lexer>(input);
	auto parser = make_shared<Parser>(lexer);
	auto program = parser->parseProgram();

	writeTestFile(JsonGenerator::generate(program).dump());
}


}