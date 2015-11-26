/*====================================================
This head file contains the prototype of class 
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
