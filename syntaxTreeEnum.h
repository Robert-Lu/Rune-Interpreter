/*====================================================
This head file contains definition of SyntaxTree and
tree node type enum.
======================================================*/
#ifndef _SYNTAX_TREE_ENUM_H_
#define _SYNTAX_TREE_ENUM_H_

#include <vector>
#include <memory>
#include "tokenEnum.h"

typedef enum syntaxkind
{
    Program, Stmt_sequence, Statement, //0, 1, 2
    
    Read_stmt, Write_stmt, WriteLn_stmt, If_stmt, Loop_stmt, While_stmt,//3..8
    Def_stmt, Assign_stmt, //9..10

    Expression, Logic_exp, Compare_exp, Simple_exp, Term, Factor, //11..16

    Terminal
} SyntaxKind;

typedef struct syntaxtreenode
{
    SyntaxKind kind;
    Token token;
    std::vector<std::shared_ptr<syntaxtreenode> > children;
} SyntaxTreeNode;

typedef std::shared_ptr<SyntaxTreeNode> pSyntaxTreeNode;
typedef std::vector<std::shared_ptr<SyntaxTreeNode>> ChildrenList;

#endif // !_SYNTAX_TREE_ENUM_H_
