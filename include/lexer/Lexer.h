#pragma once

#include "Token.h"

namespace li
{


class Lexer
{
public:
    Lexer(const string& input);
    shared_ptr<Token> parseToken();

public:
    uint64_t pos() const
    {
        return _pos;
    }

private:
    string read_identifier();
    string read_string();
    string read_number();
    void skip_whitespace();
    static Token::Type lookup_id(const string& id);

private:
    string _input;
    uint64_t _pos;
};


}
