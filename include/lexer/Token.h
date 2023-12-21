#pragma once

#include <nlohmann/json.hpp>

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
		LBrace, RBrace,

		// types
		Integer,

		// operators
		Assign, Equal, NotEqual, Greater, GreaterEqual, Less, LessEqual, LogicalNegation,
		Plus, Minus, Asterisk, Slash,

		// keywords
		Let, Fun, True, False, If, Else, Return
	};

public:
	Token(const string& literal, Token::Type type) : _literal(literal), _type(type) {}

public:
	const string& literal() const
	{
		return _literal;
	}

	void setLiteral(const string& literal)
	{
		_literal = literal;
	}

	Token::Type type() const
	{
		return _type;
	}

	void setType(Token::Type type)
	{
		_type = type;
	}

	static string typeName(Type type)
	{
		auto it = _typeNames.find(type);
		if (it == _typeNames.end())
		{
			return "";
		}

		return it->second;
	}

	string typeName() const
	{
		return typeName(_type);
	}

	static const auto& typeNames() 
	{
		return _typeNames;
	}

private:
	string _literal;
	Token::Type _type;
	static const map<Token::Type, string> _typeNames;
};


}
