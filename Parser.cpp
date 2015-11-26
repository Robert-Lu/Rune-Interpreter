#include "Parser.h"

#include <set>
#include <map>
#include <iostream>
#include <sstream>

using std::vector;
using std::string;
using std::shared_ptr;


static Token NullToken{ NullType, NullKind, "" };
static std::vector<std::shared_ptr<SyntaxTreeNode>> NullVector;
static std::set<TokenKind> setLogicOperator{ LogicAnd, LogicOr };
static std::set<TokenKind> setCompOperator{ Greater, GreaterEqual, Less, LessEqual, Equal, NotEqual };
static std::set<TokenKind> setMultiOperator{ Multiply, Divide, Mod };
static std::set<TokenKind> setAddOperator{ Plus, Minus };

std::map<int, string> dictTokenType{
    { 0, "ReservedWord" },
    { 1, "Operator" },
    { 2, "TypeName" },
    { 3, "Identifier" },
    { 4, "Const" },
    { 5, "Seperator" },
    { 6, "NullType" },
};

std::map<int, string> dictKind{
    { 0, "Program" },
    { 1, "Stmt_sequence" },
    { 2, "Statement" },
    { 3, "Read_stmt" },
    { 4, "Write_stmt" },
    { 5, "WriteLn_stmt" },
    { 6, "If_stmt" },
    { 7, "Loop_stmt" },
    { 8, "While_stmt" },
    { 9, "Def_stmt" },
    { 10, "Assign_stmt" },
    { 11, "Expression" },
    { 12, "Logic_exp" },
    { 13, "Compare_exp" },
    { 14, "Simple_exp" },
    { 15, "Term" },
    { 16, "Factor" },
    { 17, "Terminal" },
};

std::map<int, string> dictTokenKind{
    { 0, "If" },
    { 1, "Else" },
    { 2, "While" },
    { 3, "Loop" },
    { 4, "Until" },
    { 5, "Read" },
    { 6, "Write" },
    { 7, "WriteLine" },
    { 8, "Assign" },
    { 9, "Equal" },
    { 10, "NotEqual" },
    { 11, "Greater" },
    { 12, "Less" },
    { 13, "GreaterEqual" },
    { 14, "LessEqual" },
    { 15, "Plus" },
    { 16, "Minus" },
    { 17, "Multiply" },
    { 18, "Divide" },
    { 19, "Mod" },
    { 20, "LeftParen" },
    { 21, "RightParen" },
    { 22, "//ParenmetersLogicAnd" },
    { 23, "LogicOr" },
    { 24, "LogicNot" },
    { 25, "Nat" },
    { 26, "Real" },
    { 27, "String" },
    { 28, "EOL" },
    { 29, "IndentInc" },
    { 30, "IndentDec" },
    { 31, "ConstNat" },
    { 32, "ConstReal" },
    { 33, "ConstString" },
    { 34, "IDkind" },
    { 35, "NullKind" },
};

Parser::Parser()
{
    root = std::shared_ptr<SyntaxTreeNode>(new SyntaxTreeNode());
    root->kind = Program;
    root->token = NullToken;
    root->children = NullVector;
    //curr_node = root;
}

pSyntaxTreeNode Parser::parse(std::vector<Token> tokens)
{
    curr_token = tokens.begin();
    end_token = tokens.end();
    try
    {
        root->children = program();
    }
    catch (string msg)
    {
        string strline, strcol;
        std::stringstream(strline) << curr_token->pos.lineno;
        std::stringstream(strcol) << curr_token->pos.colno;
        throw "Parser Error Occured. Compile FAIL.\n" +
            strline + " : " + strcol + "\t" + msg;
    }
    return root;
}

void Parser::match(TokenKind kind)
{
    if (curr_token != end_token && curr_token->kind == kind)
    {
        curr_token++;
    }
    else
    {
        throw "Expect token kind " + dictTokenKind[kind] + " but found "
            + dictTokenKind[curr_token->kind] + " " + curr_token->value;
    }
}

void Parser::match(TokenType type)
{
    if (curr_token != end_token && curr_token->type == type)
    {
        curr_token++;
    }
    else
    {
        throw "Expect token type " + dictTokenKind[type] + " but found "
            + dictTokenKind[curr_token->type] + " " + curr_token->value;
    }
}

ChildrenList Parser::program()
{
    ChildrenList pro;
    pro.push_back(newNode(Stmt_sequence, stmt_seq()));
    return pro;
}

ChildrenList Parser::stmt_seq()
{
    ChildrenList stmt_seq;
    while (curr_token != end_token)
    {
        //show_tree(root, 0);
        pSyntaxTreeNode node;
        Token temp = *curr_token;
        switch (temp.type)
        {
        case TypeName:          //Def
            node = newNode(Def_stmt);
            node->children = def_stmt();
            stmt_seq.push_back(node);
            break;

        case Identifier:        //Assign
            node = newNode(Assign_stmt);
            node->children = assign_stmt();
            stmt_seq.push_back(node);
            break;
        case ReservedWord:
            switch (temp.kind)
            {
            case While:         //While
                node = newNode(While_stmt);
                node->children = while_stmt();
                stmt_seq.push_back(node);
                break;
            case Loop:          //Loop
                node = newNode(Loop_stmt);
                node->children = loop_stmt();
                stmt_seq.push_back(node);
                break;
            case Read:          //Read
                node = newNode(Read_stmt);
                node->children = read_stmt();
                stmt_seq.push_back(node);
                break;
            case Write:         //Write
                node = newNode(Write_stmt);
                node->children = write_stmt();
                stmt_seq.push_back(node);
                break;
            case WriteLine:     //WriteLine
                node = newNode(WriteLn_stmt);
                node->children = writeLn_stmt();
                stmt_seq.push_back(node);
                break;
            case If:            //If
                node = newNode(If_stmt);
                node->children = if_stmt();
                stmt_seq.push_back(node);
                break;
            default:
                throw "A statement cannot starts with Keyword " + curr_token->value;
            }
            break;
        case Seperator: //When stmt-seq ends, next token should be IdentDec.
            return stmt_seq;
        default:
            throw "A statement cannot starts with " + curr_token->value;;
        }
    }
    return stmt_seq;
}

ChildrenList Parser::def_stmt()
{   
    ChildrenList def_stmt;
    Token temp;
    temp = *curr_token;
    match(TypeName);
    def_stmt.push_back(newNode(Terminal, temp));
    temp = *curr_token;
    match(Identifier);
    def_stmt.push_back(newNode(Terminal, temp));
    match(EOL);
    return def_stmt;
}

ChildrenList Parser::assign_stmt()
{

    ChildrenList assign_stmt;
    Token temp;
    temp = *curr_token;
    match(Identifier);
    assign_stmt.push_back(newNode(Terminal, temp));
    match(Assign);
    assign_stmt.push_back(newNode(Expression, expression()));
    match(EOL);
    return assign_stmt;
}

ChildrenList Parser::if_stmt()
{
    ChildrenList if_stmt;
    match(If);
    if_stmt.push_back(newNode(Expression, expression()));
    match(EOL);
    match(IndentInc);
    if_stmt.push_back(newNode(Stmt_sequence, stmt_seq()));
    match(IndentDec);
    if (curr_token->kind == Else)
    {
        match(Else);
        match(EOL);
        match(IndentInc);
        if_stmt.push_back(newNode(Stmt_sequence, stmt_seq()));
        match(IndentDec);
    }
    
    return if_stmt;
}

ChildrenList Parser::loop_stmt()
{
    ChildrenList loop_stmt;
    match(Loop);
    match(EOL);
    match(IndentInc);
    loop_stmt.push_back(newNode(Stmt_sequence, stmt_seq()));
    match(IndentDec);
    match(Until);
    loop_stmt.push_back(newNode(Expression, expression()));
    match(EOL);
    return loop_stmt;
}

ChildrenList Parser::while_stmt()
{
    ChildrenList while_stmt;
    match(While);
    while_stmt.push_back(newNode(Expression, expression()));
    match(EOL);
    match(IndentInc);
    while_stmt.push_back(newNode(Stmt_sequence, stmt_seq()));
    match(IndentDec);
    return while_stmt;
}

ChildrenList Parser::read_stmt()
{
    ChildrenList read_stmt;
    Token temp;
    match(Read);
    temp = *curr_token;
    match(Identifier);
    read_stmt.push_back(newNode(Terminal, temp));
    match(EOL);
    return read_stmt;
}

ChildrenList Parser::write_stmt()
{
    ChildrenList write_stmt;
    Token temp;
    match(Write);
    write_stmt.push_back(newNode(Expression, expression()));
    match(EOL);
    return write_stmt;
}

ChildrenList Parser::writeLn_stmt()
{
    ChildrenList writeLn_stmt;
    Token temp;
    match(WriteLine);
    writeLn_stmt.push_back(newNode(Expression, expression()));
    match(EOL);
    return writeLn_stmt;
}

ChildrenList Parser::expression()
{
    ChildrenList expression;
    expression.push_back(newNode(Logic_exp, logic_exp()));
    while (setLogicOperator.find(curr_token->kind) != setLogicOperator.end() )
    {
        expression.push_back(newNode(Terminal, *curr_token++));
        expression.push_back(newNode(Logic_exp, logic_exp()));
    }
    return expression;
}

ChildrenList Parser::logic_exp()
{
    ChildrenList logic_exp;
    logic_exp.push_back(newNode(Simple_exp, simple_exp()));
    while (setCompOperator.find(curr_token->kind) != setCompOperator.end())
    {
        logic_exp.push_back(newNode(Terminal, *curr_token++));
        logic_exp.push_back(newNode(Simple_exp, simple_exp()));
    }
    return logic_exp;
}

ChildrenList Parser::simple_exp()
{
    ChildrenList simple_exp;
    simple_exp.push_back(newNode(Term, term()));
    while (setAddOperator.find(curr_token->kind) != setAddOperator.end())
    {
        simple_exp.push_back(newNode(Terminal, *curr_token++));
        simple_exp.push_back(newNode(Term, term()));
    }
    return simple_exp;
}

ChildrenList Parser::term()
{
    ChildrenList term;
    term.push_back(newNode(Factor, factor()));
    while (setMultiOperator.find(curr_token->kind) != setMultiOperator.end())
    {
        term.push_back(newNode(Terminal, *curr_token++));
        term.push_back(newNode(Factor, factor()));
    }
    return term;
}


ChildrenList Parser::factor()
{
    ChildrenList factor;
    if (curr_token->kind == LeftParen)
    {
        curr_token++;
        factor.push_back(newNode(Expression, expression()));
        match(RightParen);
    }
    else if (curr_token->type == Identifier || curr_token->type == Const)
    {
        factor.push_back(newNode(Terminal, *curr_token++));
    }
    else
    {
        throw "Expect a factor, but found " + curr_token->value;
    }
    return factor;
}

pSyntaxTreeNode newNode()
{
    return pSyntaxTreeNode(new SyntaxTreeNode());
}

pSyntaxTreeNode newNode(SyntaxKind kind)
{
    auto t = pSyntaxTreeNode(new SyntaxTreeNode());
    t->kind = kind;
    t->token = NullToken;
    t->children = NullVector;
    return t;
}

pSyntaxTreeNode newNode(SyntaxKind kind, Token tk)
{
    auto t = pSyntaxTreeNode(new SyntaxTreeNode());
    t->kind = kind;
    t->token = tk;
    t->children = NullVector;
    return t;
}

pSyntaxTreeNode newNode(SyntaxKind kind, ChildrenList lst)
{
    auto t = pSyntaxTreeNode(new SyntaxTreeNode());
    t->kind = kind;
    t->token = NullToken;
    t->children = lst;
    return t;
}

//Show Part --Testing Only

void Parser::show_tree(pSyntaxTreeNode node, int indent)
{
    using namespace std;
    for (int i = 0; i < indent; i++)
    {
        cout << "   ";
    }
    cout << dictKind[node->kind];
    if (node->kind == Terminal)
    {
        cout << "->  ";
        cout << dictTokenKind[node->token.kind] << ": " <<node->token.value;
    }
    cout << endl;
    for (auto n : node->children )
    {
        show_tree(n, indent + 1);
    }
    if (indent == 0)
    {
        cout << "\n\n\n";
    }
}