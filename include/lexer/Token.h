#pragma once

#include <string>
#include <map>

namespace li
{

using namespace std;


class Token
{
public:
	enum Type
	{
		// special chars
		Illegal, Eof, Identifier, Semicolon, Comma,
		LParen, RParen,
		LBrace, RBrace, DoubleQuotes,
		LBracket, RBracket,

		// types
		Integer, Float, String,

		// operators
		Assign, Equal, NotEqual, Greater, GreaterEqual, Less, LessEqual, LogicalNegation,
		Plus, Minus, Asterisk, Slash,

		// keywords
		Let, Fun, True, False, If, Else, Return, While
	};

public:
	Token(const string& literal, Token::Type type) : literal(literal), type(type) {}

public:
	static string typeName(Type type)
	{
		auto it = typeNames.find(type);
		if (it == typeNames.end())
		{
			return "";
		}

		return it->second;
	}

	string typeName() const
	{
		return typeName(type);
	}

public:
	string literal;
	Token::Type type;
	static const map<Token::Type, string> typeNames;
};


}
