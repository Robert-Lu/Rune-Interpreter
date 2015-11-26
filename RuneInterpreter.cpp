/*====================================================
This cpp file contain the main() function.
All of the Preprocessor, Scanner, Parser, Interpreter
are created an instance there and excuted.
======================================================*/
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
    // Show help information.
    std::cout << "Usage: rune [Options] filename\n";
    std::cout << "Options: \n";
    std::cout << "  -token:\tShow the tokens of Scanning procedure.\n";
    std::cout << "  -ast:  \tShow the Abstract Parsing Tree of Parsing procedure.\n";
}

void show_tokens(std::vector<Token> tokens)
{
    // Show tokens with format TOKEN_NAME /TOKEN_TYPE/
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
    using namespace std;
    bool print_tokens = false, print_ast = false; 
    // Refer whether tokens and ast need to be printed. 
    int name_count = 0;
    // Count the filename inputed.
    string filename;
    // Save the name of file.
    string raw;

    if (argc < 2)
    {
        // A correct call of rune must given 2 or more args;
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
            // if more than 1 filenames were inputed, show help and exit.
            show_help();
            return -2;
        }
    }
    raw = read_all_from_file(filename);
    if (raw.size() == 0)
    {
        // **There is a pitfall.
        // **I'm not sure how to get known the existance of file.
        // **So, deal as same when file not exist and empty.
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
        // print Tokens if has option "-tokens".
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
