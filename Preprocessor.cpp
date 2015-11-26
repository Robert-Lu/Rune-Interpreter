/*====================================================
This cpp file contains the implementation of class 
Preprocessor.
Steps:
1 rune-replace: **planed to do another thing but now 
                **just do some stripping and replacing.
2 decomment:    //remove comments and empty lines;
                **now it remove comments but let them 
                **become empty lines and let the empty
                **lines remain.
3 separate:     add EOL and indent seperator;
======================================================*/
#include "Preprocessor.h"

using std::string;
using std::vector;

Preprocessor::Preprocessor()
{
    //nothing to do with init.
}

vector<string> Preprocessor::preprocess(std::string const raw)
{
    string copy_raw = raw;
    rune_replace(copy_raw);
    if (copy_raw.back() != '\n')
        copy_raw.push_back('\n');
    decomment(copy_raw);
    vector<string> lines;
    lines = separate(copy_raw);
    return lines;
}

void Preprocessor::decomment(string &raw)
{
    //remove comments, erase from a '#' to next '\n'
    size_t begin_pos;
    while ((begin_pos = raw.find('#')) != string::npos)
    {
        auto end_pos = raw.find('\n', begin_pos);
        raw.erase(begin_pos, end_pos - begin_pos);
    }
}

vector<string> Preprocessor::separate(string const raw)
{
    //separate raw string with '\n', return vector of string
    vector<string> lines;
    string tmp;
    string::size_type pos_begin = 0;
    string::size_type new_line_pos = 0;
    while (pos_begin != string::npos)
    {
        new_line_pos = raw.find('\n', pos_begin);
        if (new_line_pos != string::npos)
        {
            tmp = raw.substr(pos_begin, new_line_pos - pos_begin);
            pos_begin = new_line_pos + 1;
        }
        else
        {
            tmp = raw.substr(pos_begin);
            pos_begin = new_line_pos;
        }

        if (!tmp.empty() || 1)
        {
            lines.push_back(tmp);
            tmp.clear();
        }
    }
    return lines;
}

void Preprocessor::rune_replace(string &raw)
{
    //Up to now, rune_replace() will do following things:
    //1. add space before ')' and after '(';
    auto strit = raw.begin();
    while (strit != raw.end())
    {
        if (*strit == '(')
        {
            *strit = ' ';
            strit = raw.insert(strit, '(');
            strit++;
        }
        if (*strit == ')')
        {
            strit = raw.insert(strit, ' ');
            strit++;
        }

        strit++;
    }
}