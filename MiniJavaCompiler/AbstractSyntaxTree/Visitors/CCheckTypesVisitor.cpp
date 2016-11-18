
#include <Visitors/CCheckTypesVisitor.h>
#include <CommonInclude.h>


void CCheckTypesVisitor::Visit(COperationExpression* expression) {

    expression->GetLeftOperand()->Accept(this);
    expression->GetRightOperand()->Accept(this);
}

void CCheckTypesVisitor::Visit(CNumExpression* expression) {
}

void CCheckTypesVisitor::Visit(CBoolExpression* expression) {
}

void CCheckTypesVisitor::Visit(CThisExpression* expression) {
}

void CCheckTypesVisitor::Visit(CIdExpression* expression) {
}

void CCheckTypesVisitor::Visit(CNotExpression* expression) {
    expression->GetExpression()->Accept(this);
}

void CCheckTypesVisitor::Visit(CLengthExpression* expression) {
    expression->getExpression()->Accept(this);
}

void CCheckTypesVisitor::Visit(CArrayConstructionExpression* expression) {
    expression->getSize()->Accept(this);

}


void CCheckTypesVisitor::Visit(CConstructClassExpression* expression) {
}

void CCheckTypesVisitor::Visit(CAssignStatement* statement) {
    statement->GetVariable()->Accept(this);
    statement->GetExpression()->Accept(this);
}

void CCheckTypesVisitor::Visit(CAssignItemStatement* statement) {
    statement->getId()->Accept(this);
    statement->getExpressionInBrackets()->Accept(this);
    statement->getAssignedExpression()->Accept(this);
}

void CCheckTypesVisitor::Visit(CPrintStatement* statement) {
    statement->GetExpression()->Accept(this);
}

void CCheckTypesVisitor::Visit(CIfElseStatement* statement) {
    statement->getExpression()->Accept(this);
    statement->getIfStatement()->Accept(this);
    statement->getElseStatement()->Accept(this);
}


void CCheckTypesVisitor::Visit(CWhileStatement* statement) {
    statement->getCondition()->Accept(this);
    statement->getBody()->Accept(this);
}

void CCheckTypesVisitor::Visit(CListStatement* statement) {
    auto statements = statement->GetStatements();
    size_t numberOfIterations = statements.size();
//    std::cout << "start analyze list of " << numberOfIterations << std::endl;

    for(size_t i = 0; i < numberOfIterations; ++i) {
        statements[i]->Accept(this);
    }
}

void CCheckTypesVisitor::Visit(CListExpression* statement) {

    auto statements = statement->GetExpressions();
    size_t numberOfIterations = statements.size();

    for(size_t i = 0; i < numberOfIterations; ++i) {
        statements[i]->Accept(this);
    }
}

void CCheckTypesVisitor::Visit(CVarDecl* decl) {
    //Проверка, что объявляемый тип существует.
    std::string typeName = decl->GetTypeName();
    if( types.find(typeName) == types.end() && classes.find(typeName) == classes.end() ) {
        std::cout << "Non existing type in variable declaration " << typeName << endl;
    }
}

void CCheckTypesVisitor::Visit(CGetItemExpression* expression) {

    expression->GetObject()->Accept(this);
    expression->GetIndex()->Accept(this);
}

void CCheckTypesVisitor::Visit( CMethod* statement ) {
    currentMethod = statement->getId()->GetName();
    statement->getId()->Accept(this);
    statement->getParameters()->Accept(this);
    statement->getListDeclarations()->Accept(this);
    statement->getListStatements()->Accept(this);
    statement->getReturnExpression()->Accept(this);
    std::string typeName = statement->getTypeName();
    if( types.find(typeName) == types.end() && classes.find(typeName) == classes.end() ) {
        std::cout << "Non existing return type" << typeName <<  "in method  "
                  << currentClass << "::" << currentMethod << endl;
    }
    currentMethod = "";
}


void CCheckTypesVisitor::Visit( CMethodCallExpression* exp) {
    exp->getObject()->Accept(this);
    exp->getMethodId()->Accept(this);
    exp->getArguments()->Accept(this);
}

void CCheckTypesVisitor::Visit( CClass* statement ) {
    currentClass = statement->getId()->GetName();
    if (statement->getBaseId().get() != nullptr) {
        statement->getBaseId()->Accept(this);
    }
    statement->getFields()->Accept(this);
    statement->getMethods()->Accept(this);
    currentClass = "";
}


void CCheckTypesVisitor::Visit( CMainClass* statement ) {
    currentClass = statement->GetClassId()->GetName();
    statement->GetClassId()->Accept(this);
    statement->GetArgId()->Accept(this);
    statement->GetStatement()->Accept(this);
    currentClass = "";
}


void CCheckTypesVisitor::Visit(CProgram *statement) {
    currentMethod = "";
    currentClass = "";
    statement->GetMainClass()->Accept(this);
    statement->GetMinorClasses()->Accept(this);
}

CCheckTypesVisitor::CCheckTypesVisitor(std::map<std::string, ClassInfo> &_classes) {
    classes = _classes;

    types["int"] = 0;
    types["intArray"] = 1;
    types["boolean"] = 2;

}

std::set<std::string> CCheckTypesVisitor::getAvailableMethod() {

    if(currentClass == "") {
        return std::set<std::string>();
    }
    std::string nextClass = currentClass;
    std::set<std::string> availMethod = classes[nextClass].getPublicMethods();
    while( classes[nextClass].HasBase() ) {
        nextClass = classes[nextClass].baseId;
        if (nextClass == currentClass) {
            break;
        }
        std::set<std::string> newMethod = classes[nextClass].getPublicMethods();
        for (auto iter = newMethod.begin(); iter != newMethod.end(); ++iter) {
            availMethod.insert(*iter);
        }
    }
    return availMethod;
}

