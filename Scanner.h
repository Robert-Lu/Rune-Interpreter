/*====================================================
This head file contains the prototype of class Scanner.
Scan a token vector from lines
======================================================*/
#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <string>
#include <vector>

#include "tokenEnum.h"  

bool isNat(std::string);
bool isReal(std::string);
bool isString(std::string);

class Scanner
{
public:
    Scanner();
    std::vector<Token> scan(std::vector<std::string> const lines);
    
private:
    std::string::const_iterator strit;
    std::string::const_iterator strend;
    std::vector<std::string>::const_iterator curr_line;
    int lineno, colno;

    void skip_spaces();
    std::string get_word();
    std::string get_string();
    TokenType get_type(std::string);
    TokenKind get_kind(std::string, TokenType);
};


#endif // !_SCANNER_H_
