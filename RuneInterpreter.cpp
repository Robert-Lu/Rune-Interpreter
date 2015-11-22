#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "Preprocessor.h"
#include "Scanner.h"
#include "Parser.h"
#include "Interpreter.h"

std::string read_all_from_file(std::string const file_name)
{
    using namespace std;

    std::ifstream t(file_name);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

int main(int argc, char *argv[])
{
    using namespace std;
    string raw;
    if (argc == 2)
    {
        raw = read_all_from_file(argv[1]);
    }
    else
    {
        cout << "Usage: " << argv[0] << " filename.\n";
        return -2;
    }

    try
    {
        Preprocessor pre;
        vector<string> lines = pre.preprocess(raw);
        Scanner scn;
        vector<Token> tokens = scn.scan(lines);
        Parser psr;
        auto ast = psr.parse(tokens);
        cout << "SYNTAX TREE:\n";
        psr.show_tree(ast);
        Interpreter itp;
        itp.execute(ast);
    } 
    catch (string msg)
    {
        cout << msg << endl;
        return -1;
    }
	return 0;
}

/*
#Rune !
nat x
nat fact
pi is real
pi = 3.1415925
read x
fact = 1
loop #loop
    fact = fact * x
    x = x - 1
    if (x + 1) % 5 == 0 && x >= 5
        write "So tired...\n"
until x < 1
write fact
#while
while x > 1
    fact + 1
    if fact % 2 == 0
        write "fact is even."

*/