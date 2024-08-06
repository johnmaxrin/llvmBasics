#ifndef PACEVISITOR
#define PACEVISITOR

#include "ast.h"

class Expression;

class Visitor
{
    public:
        virtual void VisitExpr(const Expression* expr) = 0;
};

#endif