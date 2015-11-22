/*====================================================
This head file contains definition of tokens' type and
kind enum.
======================================================*/
#ifndef _TOKEN_ENUM_H_
#define _TOKEN_ENUM_H_

#include <string>

typedef enum tokentype
{
    ReservedWord, Operator, TypeName, Identifier, Const, Seperator, NullType
} TokenType;

typedef enum tokenkind
{
    If, Else, While, Loop, Until,    
    Read, Write, WriteLine,             //Rederved words

    Assign, Equal, NotEqual, Greater, Less, GreaterEqual, LessEqual,
    Plus, Minus, Multiply, Divide, Mod, //+-*/%
    LeftParen, RightParen,              //Parenmeters
    LogicAnd, LogicOr, LogicNot,        //Logical operator

    Nat, Real, String,                  //Type name

    EOL, IndentInc, IndentDec,          //Seperator

    ConstNat, ConstReal, ConstString,   //Const
    IDkind,

    NullKind
} TokenKind;

typedef struct posinfo
{
    int lineno;
    int colno;
} Position;


typedef struct token
{
    TokenType type;
    tokenkind kind;
    std::string value;
    Position pos;
} Token;

#endif // !_Token_ENUM_H_
