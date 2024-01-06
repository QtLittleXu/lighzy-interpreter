#pragma once

#include "ast/Program.hpp"
#include "object/basic/Object.h"
#include "parser/Parser.h"

namespace li::test
{


void checkParserOutputs(const shared_ptr<Parser>& parser);
void initParser(shared_ptr<Program>& program, const string& input, size_t statementSize);
shared_ptr<Object> initEvaluator(const string& input);
shared_ptr<Object> initProgram(const string& input);
void testEqual(shared_ptr<Object> obj, shared_ptr<Object> value);


}