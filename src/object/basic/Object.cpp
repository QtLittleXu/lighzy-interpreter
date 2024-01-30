#include "object/basic/Object.h"

namespace li
{

const map<Object::Type, string> Object::typeNames = {
	{ Type::Null,		"null" },
	{ Type::Integer,	"integer" },
	{ Type::Float,		"float" },
	{ Type::Bool,		"bool" },
	{ Type::ReturnValue,"return_value" },
	{ Type::Error,		"error" },
	{ Type::Function,	"function" },
	{ Type::String,		"string" },
	{ Type::BuiltinFun,	"builtin_fun" },
	{ Type::Array,		"array" }
};


}