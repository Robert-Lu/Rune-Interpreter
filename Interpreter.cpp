/*==================================================== 
This cpp file contains the implementation of class 
Interpreter. 
======================================================*/ 
#include "Interpreter.h"
#include <iostream>
#include <map>
#include <sstream>
#include <cmath>

Interpreter::Interpreter()
{

}

void Interpreter::execute(pSyntaxTreeNode root)
{
    try
    {
        program(root);
    }
    catch (string msg)
    {
        throw (string)"Runetime Error:\n" + msg;
    }
}

void Interpreter::newID(string id, DataUnion du)
{
    // if the id has not occured before;
    if (IdentifierList.find(id) == IdentifierList.end())
        IdentifierList[id] = du;
    else
        throw (string)"Conflicting type of \"" + id + "\".";
}

void Interpreter::changeID(string id, DataUnion du)
{
    // if the id has occured before;
    if (IdentifierList.find(id) != IdentifierList.end())
        IdentifierList[id] = du;
    else
        throw (string)"Assign \"" + id + "\" before defining it.";
}

DataUnion Interpreter::getValue(string id)
{
    // if the id has occured before;
    if (IdentifierList.find(id) != IdentifierList.end())
        return IdentifierList[id];
    else
        throw (string)"Use \"" + id + "\" before defining it.";
}

DataUnion Interpreter::std_read(TokenKind kind)
{
    DataUnion du;
    switch (kind)
    {
    case Nat:
        du.type = Nat;
        std::cin >> du.nat;
        break;
    case Real:
        du.type = Real;
        std::cin >> du.real;
        break;
    case String:
        du.type = String;
        std::cin >> du.str;
        break;
    default:
        throw (string)"BUG: bad token into std_read().";
    }
    du.hasInit = true;
    return du;
}

void Interpreter::std_write(DataUnion du)
{
    switch (du.type)
    {
    case Nat:
        std::cout << du.nat;
        break;
    case Real:
        std::cout << du.real;
        break;
    case String:
        std::cout << du.str;
        break;
    default:
        throw (string)"BUG: bad token into std_write().";
    }
}

void Interpreter::std_newline()
{
    std::cout << std::endl;
}

void Interpreter::program(pSyntaxTreeNode node)
{
    stmt_seq(node->children[0]);
}

void Interpreter::stmt_seq(pSyntaxTreeNode node)
{
    for (auto stmt : node->children)
        switch (stmt->kind)
    {
        case Assign_stmt:
            assign_stmt(stmt);
            break;
        case Def_stmt:
            def_stmt(stmt);
            break;
        case If_stmt:
            if_stmt(stmt);
            break;
        case Loop_stmt:
            loop_stmt(stmt);
            break;
        case While_stmt:
            while_stmt(stmt);
            break;
        case Read_stmt:
            read_stmt(stmt);
            break;
        case Write_stmt:
            write_stmt(stmt);
            break;
        case WriteLn_stmt:
            writeLn_stmt(stmt);
            break;
        default:
            throw (string)"BUG: undealable node into stmt_seq().";
    }
}

void Interpreter::assign_stmt(pSyntaxTreeNode node)
{
    auto tkID = node->children[0];
    auto tkExp = node->children[1];
    DataUnion exp_value = expression(tkExp);
    changeID(tkID->token.value, exp_value);
}

void Interpreter::def_stmt(pSyntaxTreeNode node)
{
    auto tkType = node->children[0];
    auto tkID = node->children[1];
    DataUnion du;
    du.hasInit = false;
    du.type = tkType->token.kind;
    newID(tkID->token.value, du);
}

void Interpreter::if_stmt(pSyntaxTreeNode node)
{
    bool has_else_part = false;
    pSyntaxTreeNode nd_exp = node->children[0];
    pSyntaxTreeNode nd_if_part = node->children[1];
    pSyntaxTreeNode nd_else_part;
    if (node->children.size() == 3)
    {
        has_else_part = true;
        nd_else_part = node->children[2];
    }
    if (expression(nd_exp).nat != 0)
        stmt_seq(nd_if_part);
    else if (has_else_part)
        stmt_seq(nd_else_part);
}

void Interpreter::loop_stmt(pSyntaxTreeNode node)
{
    pSyntaxTreeNode nd_loop_part = node->children[0];
    pSyntaxTreeNode nd_until_exp = node->children[1];
    do
    {
        stmt_seq(nd_loop_part);
    } while (expression(nd_until_exp).nat != 1);
}

void Interpreter::while_stmt(pSyntaxTreeNode node)
{
    pSyntaxTreeNode nd_while_exp = node->children[0];
    pSyntaxTreeNode nd_while_part = node->children[1];
    while (expression(nd_while_exp).nat != 0)
    {
        stmt_seq(nd_while_part);
    }
}

void Interpreter::read_stmt(pSyntaxTreeNode node)
{
    auto tkID = node->children[0];
    auto kind = IdentifierList[tkID->token.value].type;
    auto value = std_read(kind);
    changeID(tkID->token.value, value);
}

void Interpreter::write_stmt(pSyntaxTreeNode node)
{
    DataUnion value = expression(node->children[0]);
    std_write(value);
}

void Interpreter::writeLn_stmt(pSyntaxTreeNode node)
{
    DataUnion value = expression(node->children[0]);
    std_write(value);
    std_newline();
}

DataUnion Interpreter::expression(pSyntaxTreeNode node)
{
    DataUnion temp = logic_exp(node->children[0]);
    int len = node->children.size(), i = 1;
    while (i < len)
    {
        TokenKind kind = node->children[i++]->token.kind;
        DataUnion temp2 = logic_exp(node->children[i++]);
        switch (kind)
        {
        case LogicAnd:
            temp = temp && temp2;
            break;
        case LogicOr:
            temp = temp || temp2;
            break;
        default:
            throw (string)"BUG: undealable node into expression().";
        }
    }
    return temp;
}

DataUnion Interpreter::logic_exp(pSyntaxTreeNode node)
{
    DataUnion temp = simple_exp(node->children[0]);
    int len = node->children.size(), i = 1;
    while (i < len)
    {
        TokenKind kind = node->children[i++]->token.kind;
        DataUnion temp2 = simple_exp(node->children[i++]);
        switch (kind)
        {
        case Greater:
            temp = temp > temp2;
            break;
        case Less:
            temp = temp < temp2;
            break;
        case GreaterEqual:
            temp = temp >= temp2;
            break;
        case LessEqual:
            temp = temp <= temp2;
            break;
        case Equal:
            temp = temp == temp2;
            break;
        case NotEqual:
            temp = temp != temp2;
            break;
        default:
            throw (string)"BUG: undealable node into expression().";
        }
    }
    return temp;
}

DataUnion Interpreter::simple_exp(pSyntaxTreeNode node)
{
    DataUnion temp = term(node->children[0]);
    int len = node->children.size(), i = 1;
    while (i < len)
    {
        TokenKind kind = node->children[i++]->token.kind;
        DataUnion temp2 = term(node->children[i++]);
        switch (kind)
        {
        case Plus:
            temp = temp + temp2;
            break;
        case Minus:
            temp = temp - temp2;
            break;
        default:
            throw (string)"BUG: undealable node into expression().";
        }
    }
    return temp;
}

DataUnion Interpreter::term(pSyntaxTreeNode node)
{
    DataUnion temp = factor(node->children[0]);
    int len = node->children.size(), i = 1;
    while (i < len)
    {
        TokenKind kind = node->children[i++]->token.kind;
        DataUnion temp2 = factor(node->children[i++]);
        switch (kind)
        {
        case Multiply:
            temp = temp * temp2;
            break;
        case Divide:
            temp = temp / temp2;
            break;
        case Mod:
            temp = temp % temp2;
            break;
        default:
            throw (string)"BUG: undealable node into expression().";
        }
    }
    return temp;
}

DataUnion Interpreter::factor(pSyntaxTreeNode node)
{
    DataUnion temp;
    auto factor_node = *node->children[0];
    if (factor_node.kind == Expression)
        temp = expression(node->children[0]);
    else if (factor_node.kind == Terminal)
    {
        if (factor_node.token.type == Const)
        {
            temp.hasInit = true;
            switch (factor_node.token.kind)
            {
            case ConstNat:
                std::stringstream(factor_node.token.value) >> temp.nat;
                temp.type = Nat;
                break;
            case ConstReal:
                std::stringstream(factor_node.token.value) >> temp.real;
                temp.type = Real;
                break;
            case ConstString:
                temp.str = strip(factor_node.token.value);
                temp.type = String;
                break;
            }
        }
        else if (factor_node.token.type == Identifier)
        {
            if (IdentifierList.find(factor_node.token.value) == IdentifierList.end())
            {
                throw (string)"Undefined variable " + factor_node.token.value + " .";
            }
            auto that = IdentifierList[factor_node.token.value];
            temp.hasInit = that.hasInit;
            switch (that.type)
            {
            case Nat:
                temp.nat = that.nat;
                temp.type = Nat;
                break;
            case Real:
                temp.real = that.real;
                temp.type = Real;
                break;
            case String:
                temp.type = String;
                break;
            }
        }
    }
    return temp;
}

DataUnion DataUnion::operator&&(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat && that.nat;
        temp.type = Nat;
    }
    else
    {
        throw (string)"Use operation && with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator||(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat || that.nat;
        temp.type = Nat;
    }
    else
    {
        throw (string)"Use operation || with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator>(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat > that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.nat = this->real > that.real;
        temp.type = Nat;
    }
    else
    {
        throw (string)"Use operation > with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator<(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat < that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.nat = this->real < that.real;
        temp.type = Nat;
    }
    else
    {
        throw (string)"Use operation < with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator>=(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat >= that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.nat = this->real >= that.real;
        temp.type = Nat;
    }
    else
    {
        throw (string)"Use operation >= with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator<=(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat <= that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.nat = this->real <= that.real;
        temp.type = Nat;
    }
    else
    {
        throw (string)"Use operation <= with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator==(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    temp.type = Nat;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat == that.nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.nat = std::abs(temp.real - that.real) < 1.0e-10;
    }
    else
    {
        throw (string)"Use operation == with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator!=(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    temp.type = Nat;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat != that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.nat = std::abs(this->real - that.real) < 1.0e-10;
        temp.type = Nat;
    }
    else
    {
        throw (string)"Use operation != with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator+(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat + that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.real = this->real + that.real;
        temp.type = Real;
    }
    else if (thiskind == Nat && thatkind == Real)
    {
        temp.nat = this->nat + (int)that.real;
        temp.type = Nat;
    }
    else if (thiskind == Real && thatkind == Nat)
    {
        temp.real = this->real + that.nat;
        temp.type = Real;
    }
    else
    {
        throw (string)"Use operation + with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator-(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat - that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.real = this->real - that.real;
        temp.type = Real;
    }
    else if (thiskind == Nat && thatkind == Real)
    {
        temp.nat = this->nat - (int)that.real;
        temp.type = Nat;
    }
    else if (thiskind == Real && thatkind == Nat)
    {
        temp.real = this->real - that.nat;
        temp.type = Real;
    }
    else
    {
        throw (string)"Use operation - with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator*(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat * that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.real = this->real * that.real;
        temp.type = Real;
    }
    else if (thiskind == Nat && thatkind == Real)
    {
        temp.nat = this->nat * (int)that.real;
        temp.type = Nat;
    }
    else if (thiskind == Real && thatkind == Nat)
    {
        temp.real = this->real * that.nat;
        temp.type = Real;
    }
    else
    {
        throw (string)"Use operation * with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator/(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if ((that == Zero).nat == 1)
        throw (string)"Zero division error.";
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat / that.nat;
        temp.type = Nat;
    }
    else if (thiskind == thatkind && thatkind == Real)
    {
        temp.real = this->real / that.real;
        temp.type = Real;
    }
    else if (thiskind == Nat && thatkind == Real)
    {
        temp.nat = this->nat / (int)that.real;
        temp.type = Nat;
    }
    else if (thiskind == Real && thatkind == Nat)
    {
        temp.real = this->real / that.nat;
        temp.type = Real;
    }
    else
    {
        throw (string)"Use operation / with wrong type.";
    }
    return temp;
}

DataUnion DataUnion::operator%(DataUnion that)
{
    DataUnion temp;
    temp.hasInit = true;
    check_init(*this);
    check_init(that);
    TokenKind thiskind = this->type;
    TokenKind thatkind = that.type;
    if ((that == Zero).nat == 1)
        throw (string)"Zero division error.";
    if (thiskind == thatkind && thatkind == Nat)
    {
        temp.nat = this->nat % that.nat;
        temp.type = Nat;
    }
    else
    {
        throw (string)"Use operation % with wrong type.";
    }
    return temp;
}



void check_init(DataUnion du)
{
    if (!du.hasInit)
    {
        throw (string)"Use variable before initialization.";
    }
}

string strip(string s)
{
    string temp;
    for (size_t i = 1; i < s.size() - 1; i++)
    {
        temp.push_back(s[i]);
    }
    return temp;
}
