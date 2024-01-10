#include <gtest/gtest.h>
#include "evaluator/Evaluator.h"
#include "initialization.h"

namespace li::test
{


TEST(stdlibTest, utils)
{
	struct Expected
	{
		string input;
		shared_ptr<Object> value;
	} tests[] = {
		{ R"(print("Hello world!"))", Evaluator::null },
		{ R"(println("Hello world!"))", Evaluator::null },
		{ R"(len("12345"))", make_shared<Integer>(5) },
		{ R"(len([1, 2, 3, 4, 5]))", make_shared<Integer>(5) }
	};

	for (const auto& [input, value] : tests)
	{
		SCOPED_TRACE(input);
		testEqual(initProgram(input), value);
	}
}


}