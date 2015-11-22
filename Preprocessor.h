/*====================================================
This head file contains the prototype of class 
Preprocessor.
Steps:
1 rune-replace: TODO
2 decomment:    remove comments and empty lines;
3 separate:     add EOL and indent seperator;
======================================================*/
#ifndef _PREPROCESSOR_H_
#define _PREPROCESSOR_H_

#include <string>
#include <vector>

class Preprocessor
{
public:
    Preprocessor();
    std::vector<std::string> preprocess(std::string const raw);
private:
    void rune_replace(std::string &raw);
    void decomment(std::string &raw);
    std::vector<std::string> separate(std::string const raw);
};

#endif // !_PREPROCESSOR_H_
