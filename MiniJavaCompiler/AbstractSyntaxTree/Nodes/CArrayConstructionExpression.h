#pragma once

#include <Nodes/IExpression.h>

class CArrayConstructionExpression : public IExpression {
public:
    CArrayConstructionExpression(const Location location, const std::shared_ptr<IExpression> &size);
    const std::shared_ptr<IExpression> &getSize() const;
    void Accept(IVisitor*);

private:
    std::shared_ptr<IExpression> size;
};


