#include "lexer/Lexer.h"

namespace li
{


Lexer::Lexer(const string &input) :
    _input(input), _pos(0)
{
}

shared_ptr<Token> Lexer::parseToken()
{
    shared_ptr<Token> token;
	skip_whitespace();
	if (_pos >= _input.size())
	{
		token = make_shared<Token>("", Token::Eof);
		return token;
	}

	switch (_input.at(_pos))
	{

	case ',':
		token = make_shared<Token>(",", Token::Comma);
		break;

	case '(':
		token = make_shared<Token>("(", Token::LParen);
		break;

	case ')':
		token = make_shared<Token>(")", Token::RParen);
		break;

	case '{':
		token = make_shared<Token>("{", Token::LBrace);
		break;

	case '}':
		token = make_shared<Token>("}", Token::RBrace);
		break;

	case ';':
		token = make_shared<Token>(";", Token::Semicolon);
		break;

	case '=':
		if (_pos + 1 < _input.size() && _input.at(_pos + 1) == '=')
		{
			token = make_shared<Token>("==", Token::Equal);
			_pos += 2;
		}
		else
		{
			token = make_shared<Token>("=", Token::Assign);
			_pos++;
		}
		return token;

	case '!':
		if (_pos + 1 < _input.size() && _input.at(_pos + 1) == '=')
		{
			token = make_shared<Token>("!=", Token::NotEqual);
			_pos += 2;
		}
		else
		{
			token = make_shared<Token>("!", Token::LogicalNegation);
			_pos++;
		}
		return token;

	case '>':
		if (_pos + 1 < _input.size() && _input.at(_pos + 1) == '=')
		{
			token = make_shared<Token>(">=", Token::GreaterEqual);
			_pos += 2;
		}
		else
		{
			token = make_shared<Token>(">", Token::Greater);
			_pos++;
		}
		return token;

	case '<':
		if (_pos + 1 < _input.size() && _input.at(_pos + 1) == '=')
		{
			token = make_shared<Token>("<=", Token::LessEqual);
			_pos += 2;
		}
		else
		{
			token = make_shared<Token>("<", Token::Less);
			_pos++;
		}
		return token;

	case '+':
		token = make_shared<Token>("+", Token::Plus);
		break;

	case '-':
		token = make_shared<Token>("-", Token::Minus);
		break;

	case '*':
		token = make_shared<Token>("*", Token::Asterisk);
		break;

	case '/':
		token = make_shared<Token>("/", Token::Slash);
		break;

	case '"':
		token = make_shared<Token>(read_string(), Token::String);
		return token;

	case '[':
		token = make_shared<Token>("[", Token::LBracket);
		break;

	case ']':
		token = make_shared<Token>("]", Token::RBracket);
		break;

	default:
		if (isalpha(_input.at(_pos)))
		{
			// identifier or keyword
			auto id = read_identifier();
			token = make_shared<Token>(id, lookup_id(id));
		}
		else if (isdigit(_input.at(_pos)))
		{
			// number
			auto[number, type] = read_number();
			token = make_shared<Token>(number, type);
		}
		else
		{
			// illegal
			token = make_shared<Token>(string(1, _input.at(_pos)), Token::Illegal);
			_pos++;
		}
		return token;

	}

	_pos++;
	return token;
}

string Lexer::read_identifier()
{
    string identifier;
    while (_pos < _input.size() && isalnum(_input.at(_pos)))
    {
        identifier += _input.at(_pos);
        _pos++;
    }
    return identifier;
}

string Lexer::read_string()
{
    string buffer;
	_pos++;
    while (_pos < _input.size() && _input.at(_pos) != '"')
    {
		if (_input.at(_pos) == '\n')
		{
			return "\n";	// Return linebreak to decide whether expect double quotes
		}

        buffer += _input.at(_pos);
        _pos++;
    }
	_pos++;
    return buffer;
}

tuple<string, Token::Type> Lexer::read_number()
{
    string number;
	bool isFloat = false;
    while (_pos < _input.size() && (isdigit(_input.at(_pos)) || _input.at(_pos) == '.'))
    {
		if (_input.at(_pos) == '.') isFloat = true;

        number += _input.at(_pos);
        _pos++;
    }
	return { number, isFloat ? Token::Float : Token::Integer };
}

bool Lexer::is_whitespace()
{
	return _input.at(_pos) == ' ' || _input.at(_pos) == '\t' || _input.at(_pos) == '\n' || _input.at(_pos) == '\r';
}

void Lexer::skip_whitespace()
{
    while (_pos < _input.size() && is_whitespace() )
    {
        _pos++;
    }
}

Token::Type Lexer::lookup_id(const string& id)
{
    Token::Type type = Token::Identifier;
	for (const auto& it : Token::typeNames)
	{
		if (it.second == id)
		{
			type = it.first;
		}
	}

	return type;
}


}
