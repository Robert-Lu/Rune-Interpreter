#include "Scanner.h"
#include <regex>
#include <map>

using std::string;
using std::vector;

std::map<std::string, TokenKind> dictReversedWord{
    { "if", If },
    { "else", Else },
    { "while", While },
    { "loop", Loop },
    { "until", Until },
    { "read", Read },
    { "write", Write },
    { "writeln", WriteLine },
};

std::map<std::string, TokenKind> dictTypeName{
    { "nat", Nat },
    { "real", Real },
    { "str", String },
};

std::map<std::string, TokenKind> dictOperator{
    { "=", Assign },
    { "==", Equal },
    { "!=", NotEqual },
    { ">", Greater },
    { "<", Less },
    { ">=", GreaterEqual },
    { "<=", LessEqual },

    { "(", LeftParen },
    { ")", RightParen },

    { "&&", LogicAnd },
    { "||", LogicOr },
    { "!", LogicNot },

    { "+", Plus },
    { "-", Minus },
    { "*", Multiply },
    { "/", Divide },
    { "%", Mod },

};

Token tokenIndentInc(Position pos)
{
    return Token{ Seperator, IndentInc, "-->\n", pos };
};

Token tokenIndentDec(Position pos)
{
    return Token{ Seperator, IndentDec, "<--\n", pos };
};

Token tokenEndOfLine(Position pos)
{
    return Token{ Seperator, EOL, "EOL\n", pos };
};


Scanner::Scanner()
{

}

vector<Token> Scanner::scan(vector<string> const lines)
{
    lineno = 1;
    curr_line = lines.cbegin();
    vector<Token> result;
    string word;
    int indent, old_indent = 0;
    bool is_empty_line;
    while (curr_line != lines.end())
    {
        //let two iterators point to the current line.
        strit = curr_line->begin();
        strend = curr_line->end();
        colno = 0;
        is_empty_line = true;

        //deal with indents, add IndentInc and IndentDec
        //when current indent is difference from old one.
        indent = 0;
        while (strit != strend && *strit == '\t')
        {
            strit++;
            colno++;
            indent += 1;
        }
        if (indent != old_indent && strit != strend)
        {
            for (int i = 0; i < indent - old_indent; i++)
            {
                Position pos{ lineno, colno };
                result.push_back(tokenIndentInc(pos));
            }
            for (int i = 0; i < old_indent - indent; i++)
            {
                Position pos{ lineno, colno };
                result.push_back(tokenIndentDec(pos));
            }
            old_indent = indent;
        }

        //scan words into token vector.
        try
        {
            while (word = get_word(), word.size() > 0)
            {
                TokenType tokentype = get_type(word);
                TokenKind tokenkind = get_kind(word, tokentype);
                Position pos = { lineno, colno };
                Token token = { tokentype, tokenkind, word, pos };
                result.push_back(token);
                is_empty_line = false;
            }

        }
        catch (std::invalid_argument e)
        {
            string msg(e.what());
            string prefix = "Scanner Error Occured. Compile FAIL.\n" +
                std::to_string(curr_line - lines.begin() + 1) + ":" +
                std::to_string(strit - curr_line->begin()) + "\t";
            msg = prefix + msg;
            throw msg;
        }
        if (!is_empty_line)
        {
            Position pos{ lineno, colno };
            result.push_back(tokenEndOfLine(pos));
        }
        curr_line++;
        lineno++;
    }
    if (old_indent != 0)
    {
        for (int i = 0; i < old_indent; i++)
        {
            Position pos{ lineno, colno };
            result.push_back(tokenIndentDec(pos));
        }
    }
    return result;
}

string Scanner::get_word()
{
    string temp;
    skip_spaces();
    if (strit != strend && *strit == '\"')
        return get_string();
    while (strit != strend && *strit != ' ')
    {
        temp.push_back(*strit++);
        colno++;
    }
    return temp;
}

void Scanner::skip_spaces()
{
    while (strit != strend && *strit == ' ')
    {
        if (strit != curr_line->begin() && *(strit - 1) != '(' && 
            strit != strend - 1 && *(strit + 1) != ')')
        {
            colno++;
        }
        strit++;
    }
}

string Scanner::get_string()
{
    strit++;
    colno++;
    string temp = "\"";
    while (strit != strend && *strit != '\"')
    {
        temp.push_back(*strit++);
        colno++;
    }
    temp.push_back('\"');
    strit++;
    colno++;
    return temp;
}

TokenType Scanner::get_type(string word)
{
    if (dictReversedWord.find(word) != dictReversedWord.end())
    {
        //if word is in the dictionary of Reserved Word.
        return ReservedWord;
    }
    if (dictTypeName.find(word) != dictTypeName.end())
    {
        //if word is in the dictionary of Type Name.
        return TypeName;
    }
    if (dictOperator.find(word) != dictOperator.end())
    {
        //if word is in the dictionary of Operator.
        return Operator;
    }
    if (::isalpha(word[0]) || word[0] == '_')
    {
        //else, if word starts with alpha or underline.
        for (char c : word)
            if (!::isalnum(c) && c != '_')
                throw std::invalid_argument("Invalid charactor in identifier \"" + word + "\".");
        return Identifier;
    }
    return Const;
}

TokenKind Scanner::get_kind(string word, TokenType type)
{
    switch (type)
    {
    case ReservedWord:
        return dictReversedWord[word];
    case TypeName:
        return dictTypeName[word];
    case Operator:
        return dictOperator[word];
    case Identifier:
        return IDkind;
    case Const:
       /* std::regex nat_pattern("[0-9]+");
        std::regex real_pattern("[0-9]+\\.[0-9]+");
        std::regex str_pattern("\\\".*\\\"");*/
        if (isNat(word))
            return ConstNat;
        if (isReal(word))
            return ConstReal;
        if (isString(word))
            return ConstString;
    }
    throw std::invalid_argument("Can\'t match any type with \"" + word + "\".");
}

bool isNat(string word)
{
    if (word[0] == '-' || word[0] == '+')
        word[0] = '0';
    for (char c : word)
        if (!isdigit(c))
            return false;
    return true;
}

bool isReal(string word)
{
    if (word[0] == '-' || word[0] == '+')
        word[0] = '0';
    int dot_count = 0;
    for (char c : word)
    {
        if (!isdigit(c) && c != '.')
            return false;
        if (c == '.')
            dot_count++;
    }
    return dot_count <= 1;
}

bool isString(string word)
{
    return word.back() == '\"' && word.front() == '\"';
}
