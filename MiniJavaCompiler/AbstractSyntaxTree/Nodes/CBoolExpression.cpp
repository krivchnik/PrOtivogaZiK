#include <Nodes/CBoolExpression.h>

CBoolExpression::CBoolExpression(const Location location, bool value)
        : value(value) {
    this->location = location;
}

bool CBoolExpression::GetValue() {
    return value;
}

void CBoolExpression::Accept(IVisitor* visitor) {
    visitor->Visit(this);
}

