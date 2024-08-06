#ifndef PACEAST
#define PACEAST

#include "visitor.h"

class AST
{
public:
    virtual ~AST() {}
    virtual void Accept(Visitor *visitor) const = 0;
};

class Expression : public AST
{

public:
    virtual void Accept(Visitor *visitor) const override
    {
        visitor->VisitExpr(this);
    }

    int val;
    char op;
    Expression *lhs;
    Expression *rhs;

    Expression(int value) : val(value), op('\0'), lhs(nullptr), rhs(nullptr) {}
    Expression(char argop, Expression *arglhs, Expression *argrhs) : val(0), op(argop), lhs(arglhs), rhs(argrhs) {}
};

#endif