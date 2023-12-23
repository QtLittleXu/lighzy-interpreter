#include "object/basic/Object.h"

namespace li
{

const map<Object::Type, string> Object::_typeNames = {
	{ Type::Null, "null" },
	{ Type::Integer, "integer" },
	{ Type::Bool, "bool" },
	{ Type::ReturnValue, "return_value" },
	{ Type::Error, "error" },
	{ Type::Function, "function" }
};


}