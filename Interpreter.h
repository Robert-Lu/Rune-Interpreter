/*==================================================== 
This head file contains the prototype of class Interp-
reter. 
======================================================*/ 

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "syntaxTreeEnum.h"
#include "tokenEnum.h"
#include <map>
#include <memory>

#define DELTA 1.0e-9

using std::string;

typedef struct dataunion
{
    TokenKind type;
    bool hasInit;
    int nat;
    double real;
    string str;
    struct dataunion operator&&(struct dataunion);
    struct dataunion operator||(struct dataunion);
    struct dataunion operator+(struct dataunion);
    struct dataunion operator-(struct dataunion);
    struct dataunion operator*(struct dataunion);
    struct dataunion operator/(struct dataunion);
    struct dataunion operator%(struct dataunion);
    struct dataunion operator>(struct dataunion);
    struct dataunion operator<(struct dataunion);
    struct dataunion operator>=(struct dataunion);
    struct dataunion operator<=(struct dataunion);
    struct dataunion operator==(struct dataunion);
    struct dataunion operator!=(struct dataunion);
} DataUnion;

void check_init(DataUnion du);
string strip(string);

static DataUnion Zero = { Nat, true, 0, 0, "" };

class Interpreter
{
public:
    Interpreter();
    void execute(pSyntaxTreeNode);

private:
    std::map<string, DataUnion> IdentifierList;
    void newID(string, DataUnion);
    void changeID(string, DataUnion);
    DataUnion getValue(string);
    DataUnion std_read(TokenKind);
    void std_write(DataUnion);
    void std_newline();
    void program(pSyntaxTreeNode);
    void stmt_seq(pSyntaxTreeNode);
    void assign_stmt(pSyntaxTreeNode);
    void def_stmt(pSyntaxTreeNode);
    void if_stmt(pSyntaxTreeNode);
    void loop_stmt(pSyntaxTreeNode);
    void while_stmt(pSyntaxTreeNode);
    void read_stmt(pSyntaxTreeNode);
    void write_stmt(pSyntaxTreeNode);
    void writeLn_stmt(pSyntaxTreeNode);
    DataUnion expression(pSyntaxTreeNode);
    DataUnion logic_exp(pSyntaxTreeNode);
    DataUnion simple_exp(pSyntaxTreeNode);
    DataUnion term(pSyntaxTreeNode);
    DataUnion factor(pSyntaxTreeNode);
};

#endif // !_INTERPRETER_H_
