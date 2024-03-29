#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "initialization.h"
#include "evaluator/Evaluator.h"
#include "parser/Parser.h"
#include "object/Float.hpp"
#include "object/Integer.hpp"
#include "object/Bool.hpp"
#include "object/Error.hpp"
#include "object/String.hpp"
#include "object/Array.hpp"
#include "object/Null.hpp"
#include "program/initialization.h"
#include "config.h"

namespace li::test
{


void checkParserOutputs(shared_ptr<Parser> parser)
{
	for (const auto& output : parser->outputs())
	{
		cerr << output << '\n';
	}

	EXPECT_TRUE(parser->outputs().empty());
}

void initParser(shared_ptr<Program>& program, const string& input, size_t statementSize)
{
	auto lexer = make_shared<Lexer>(input);
	auto parser = make_shared<Parser>(lexer);
	program = parser->parseProgram();

	ASSERT_NO_FATAL_FAILURE(checkParserOutputs(parser));
	ASSERT_EQ(program->statements.size(), statementSize);
}

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

string read_file(const string& fileName)
{
	string output;
	string buffer;
	ifstream in(fileName, ios::in);
	if (!in)
	{
		cerr << "File \"" << fileName << "\" read failed! \n";
		return "";
	}

	while (getline(in, buffer))
	{
		output += buffer + '\n';
	}
	return output;
}

string read_folder_sources(const filesystem::path& folder)
{
	if (!filesystem::is_directory(folder))
	{
		cerr << "cannot read standard library! " << '\n';
		return "";
	}

	string sources;
	for (const auto& entry : filesystem::directory_iterator(folder))
	{
		if (filesystem::is_directory(entry))
        {
            sources += read_folder_sources(entry.path());
            continue;
        }

		string fileName = entry.path().string();
		sources += read_file(fileName);
	}
	return sources;
}

shared_ptr<Object> initProgram(const string& input)
{
	string sources = read_folder_sources(program::executable_path().parent_path().parent_path().parent_path() / PREREAD_SOURCES_DIR);
	return initEvaluator(sources + input);
}

void testEqual(shared_ptr<Object> obj, shared_ptr<Object> value)
{
	switch (obj->type)
	{

	case Object::Type::Null:
	{
		ASSERT_EQ(obj->typeName(), value->typeName());
		EXPECT_EQ(dynamic_pointer_cast<Null>(obj), dynamic_pointer_cast<Null>(value));
		break;
	}

	case Object::Type::Integer:
	{
		ASSERT_EQ(obj->typeName(), value->typeName());
		EXPECT_EQ(dynamic_pointer_cast<Integer>(obj)->value, dynamic_pointer_cast<Integer>(value)->value);
		break;
	}

	case Object::Type::Array:
	{
		ASSERT_EQ(obj->typeName(), value->typeName());
		auto cast = dynamic_pointer_cast<Array>(obj);
		auto castValue = dynamic_pointer_cast<Array>(value);

		ASSERT_EQ(cast->elements.size(), castValue->elements.size());
		for (int i = 0; i < cast->elements.size(); i++)
		{
			testEqual(cast->elements.at(i), castValue->elements.at(i));
		}
		break;
	}

	case Object::Type::Float:
	{
		ASSERT_EQ(obj->typeName(), value->typeName());
		EXPECT_EQ(dynamic_pointer_cast<Float>(obj)->value, dynamic_pointer_cast<Float>(value)->value);
		break;
	}

	case Object::Type::String:
	{
		ASSERT_EQ(obj->typeName(), value->typeName());
		EXPECT_EQ(dynamic_pointer_cast<String>(obj)->value, dynamic_pointer_cast<String>(value)->value);
		break;
	}

	case Object::Type::Bool:
	{
		ASSERT_EQ(obj->typeName(), value->typeName());
		EXPECT_EQ(dynamic_pointer_cast<Bool>(obj)->value, dynamic_pointer_cast<Bool>(value)->value);
		break;
	}

	case Object::Type::Error:
	{
		ASSERT_EQ(obj->typeName(), value->typeName());
		EXPECT_EQ(dynamic_pointer_cast<Error>(obj)->message, dynamic_pointer_cast<Error>(value)->message);
		break;
	}
	
	default:
		ASSERT_TRUE(false) << "unknown object type in testEqual: " << obj->typeName();
		break;

	}
}


}