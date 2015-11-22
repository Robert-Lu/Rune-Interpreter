/*====================================================
This head file contains the prototype of class Parser.
======================================================*/
#ifndef _PARSER_H_
#define _PARSER_H_

#include "tokenEnum.h"
#include "syntaxTreeEnum.h"

pSyntaxTreeNode newNode();
pSyntaxTreeNode newNode(SyntaxKind kind);
pSyntaxTreeNode newNode(SyntaxKind kind, Token tk);
pSyntaxTreeNode newNode(SyntaxKind kind, ChildrenList lst);

class Parser
{
public:
    Parser();
    std::shared_ptr<SyntaxTreeNode> parse(std::vector<Token>);
    void show_tree(std::shared_ptr<SyntaxTreeNode>, int indent = 0);

private:
    std::vector<Token>::const_iterator curr_token;
    std::vector<Token>::const_iterator end_token;
    //std::shared_ptr<SyntaxTreeNode> curr_node;
    std::shared_ptr<SyntaxTreeNode> root;

    ChildrenList program();
    ChildrenList stmt_seq();
    ChildrenList assign_stmt();
    ChildrenList def_stmt();
    ChildrenList if_stmt();
    ChildrenList loop_stmt();
    ChildrenList while_stmt();
    ChildrenList read_stmt();
    ChildrenList write_stmt();
    ChildrenList writeLn_stmt();
    ChildrenList expression();
    ChildrenList logic_exp();
    ChildrenList simple_exp();
    ChildrenList term();
    ChildrenList factor();

    void match(TokenKind);
    void match(TokenType);
};

#endif // !_PARSER_H_
