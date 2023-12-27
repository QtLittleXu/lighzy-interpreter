#pragma once

#include "Token.h"
#include <memory>

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
    bool is_whitespace();
    string read_identifier();
    string read_string();
    tuple<string, Token::Type> read_number();
    void skip_whitespace();
    static Token::Type lookup_id(const string& id);

private:
    string _input;
    uint64_t _pos;
};


}
