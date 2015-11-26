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

void show_help()
{
    std::cout << "Usage: rune [Options] filename\n";
    std::cout << "Options: \n";
    std::cout << "  -token:\tShow the tokens of Scanning procedure.\n";
    std::cout << "  -ast:  \tShow the Abstract Parsing Tree of Parsing procedure.\n";
}

void show_tokens(std::vector<Token> tokens)
{
    using namespace std;
    cout << "TOKENS:\n";
    map<TokenType, string> dict = 
    {
        { ReservedWord, "rw" },
        { Operator, "op" },
        { TypeName, "type" },
        { Identifier, "id" },
        { Const, "const" },
        { Seperator, "" },
    };
    for (Token t : tokens)
    {
        cout << t.value;
        if (t.type != Seperator)
        {
            cout << " /" << dict[t.type] << "/   ";
        }
    }
    cout << "\n\n\n";
}

int main(int argc, char *argv[])
{
    bool print_tokens = false, print_ast = false;
    int name_count = 0;
    string filename;
    using namespace std;
    string raw;

    if (argc < 2)
    {
        show_help();
        return -2;
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            if (argv[i][0] == '-')
            {
                switch(argv[i][1])
                {
                case 't':
                    print_tokens = true;
                    break;
                case 'a':
                    print_ast = true;
                    break;
                default:
                    show_help();
                    return -2;
                }
            }
            else
            {
                filename = argv[i];
                name_count++;
            }
        }
        if (name_count != 1)
        {
            show_help();
            return -2;
        }
    }
    raw = read_all_from_file(filename);
    if (raw.size() == 0)
    {
        cout << "file not exist or nothing contained in file.";
        return -3;
    }
    try
    {
        Preprocessor pre;
        vector<string> lines = pre.preprocess(raw);
        // Raw string preprocessed to decommented lines.
        Scanner scn;
        vector<Token> tokens = scn.scan(lines);
        // Scan the lines to a vector of Tokens.
        // Token is defined in "tokenEnum.h".
        if (print_tokens)
        {
            show_tokens(tokens);
        }
        Parser psr;
        auto ast = psr.parse(tokens);
        // Parse the Tokens to an Abstract Syntax Tree.
        // AST is defined in "syntexTreeEnum.h".
        if (print_ast)
        {
            cout << "SYNTAX TREE:\n";
            psr.show_tree(ast);
        }
        // print AST if has option "-ast".
        Interpreter itp;
        itp.execute(ast);
        // Interprete the AST.
    } 
    catch (string msg)
    {
        cout << msg << endl;
        return -1;
        // print error message.
    }
	return 0;
}
