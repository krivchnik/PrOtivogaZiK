#include <Visitors/CPrintVisitor.h>

#include <CommonInclude.h>

std::string GetOperationSign(int index) {
	std::vector<std::string> signs = {"addition", "subtraction", "multiplication", "mod", "and", "or", "less"};
	return signs[index];
}

void CPrintVisitor::StartVisit(INode* startNode, std::string filename) {

	expressionId = 0;

	file.open(filename);
	
	file << "digraph task1 {\n\t";

	startNode->Accept(this);
    //распечатает id тех кто будет квадратным
    for(std::string name : idsOfTokenWithBoxShape){
    	file << getEndLine() << name << " [shape=box]";
    }
	file << ";\n}";
	file.close();	
}

void CPrintVisitor::Visit(COperationExpression* expression) {

	int currentExpressionId = ++expressionId;

	file << getNodeNameWithLabel(GetOperationSign(expression->GetOperationType()), currentExpressionId,
                                 GetOperationSign(expression->GetOperationType()))  << getArrow();
	expression->GetLeftOperand()->Accept(this);
	file << getEndLine();
    file << getNodeNameWithLabel(GetOperationSign(expression->GetOperationType()), currentExpressionId,
                                 GetOperationSign(expression->GetOperationType()))  << getArrow();
    expression->GetRightOperand()->Accept(this);
}

void CPrintVisitor::Visit(CNumExpression* expression) {
	++expressionId;
	std::string newId = getNodeNameWithLabel(std::to_string(expression->GetNumber()), expressionId, "Number");
	file << newId;
    idsOfTokenWithBoxShape.push_back(newId);
}

void CPrintVisitor::Visit(CBoolExpression* expression) {
	++expressionId;
	std::string newId = getNodeNameWithLabel(std::to_string(expression->GetValue()), expressionId, "Bool");
	file << newId;
	idsOfTokenWithBoxShape.push_back(newId);
}

void CPrintVisitor::Visit(CThisExpression* expression) {
    ++expressionId;
    std::string newId = getNodeNameWithLabel("This", expressionId, "This");
    file << newId;
    idsOfTokenWithBoxShape.push_back(newId);
}

void CPrintVisitor::Visit(CIdExpression* expression) {
	++expressionId;
	std::string newId = getNodeNameWithLabel(expression->GetName(), expressionId, "");
	file << newId;
	idsOfTokenWithBoxShape.push_back(newId);
}

void CPrintVisitor::Visit(CNotExpression* expression) {
    ++expressionId;
    file << getNodeNameWithLabel("not", expressionId, "not") << getArrow();
    expression->GetExpression()->Accept(this);
}

void CPrintVisitor::Visit(CLengthExpression* expression) {
	++expressionId;
	expression->getExpression()->Accept(this);
	file << getArrow() << "getLength" + delim + std::to_string(expressionId);
}

void CPrintVisitor::Visit(CArrayConstructionExpression* expression) {
    ++expressionId;
    file << getNodeNameWithLabel("newIntArray", expressionId, "newIntArray") + getArrow() +
            getNodeNameWithLabel("withSize", expressionId, "withSize")  + getArrow();
    expression->getSize()->Accept(this);

}


void CPrintVisitor::Visit(CConstructClassExpression* expression) {
    ++expressionId;
    file << getNodeNameWithLabel("constructClass", expressionId, "constructClass") + getArrow() +
            getNodeNameWithLabel(expression->getClassID()->GetName(), expressionId, expression->getClassID()->GetName());
}

void CPrintVisitor::Visit(CAssignStatement* statement) {
	int currentExpressionId = ++expressionId;
    file << getNodeNameWithLabel("assignment", currentExpressionId, "assignment") << getArrow();
	statement->GetVariable()->Accept(this);
	file << getEndLine();
	file << getNodeNameWithLabel("assignment", currentExpressionId, "assignment") << getArrow();
	statement->GetExpression()->Accept(this);
}

void CPrintVisitor::Visit(CAssignItemStatement* statement) {
    int currentExpressionId = ++expressionId;
    file << getNodeNameWithLabel("assignmentItem", currentExpressionId, "assignmentItem") << getArrow();
    statement->getId()->Accept(this);
    file << getArrow();

    file << getNodeNameWithLabel("indexItem", currentExpressionId, "indexItem") << getArrow();
    statement->getExpressionInBrackets()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("assignmentItem", currentExpressionId, "assignmentItem") << getArrow();
    statement->getAssignedExpression()->Accept(this);
}

void CPrintVisitor::Visit(CPrintStatement* statement) {
	++expressionId;
	file << getNodeNameWithLabel("println", expressionId, "println") <<  getArrow();
	statement->GetExpression()->Accept(this);
}

void CPrintVisitor::Visit(CIfElseStatement* statement) {
	int currentExpressionId = ++expressionId;
	file << getNodeNameWithLabel("if", currentExpressionId, "if") << getArrow() <<
         getNodeNameWithLabel("condition", currentExpressionId, "condition") << getArrow();
	statement->getExpression()->Accept(this);
	file << getEndLine();
    file << getNodeNameWithLabel("if", currentExpressionId, "if") << getArrow() <<
         getNodeNameWithLabel("ifStatement", currentExpressionId, "ifStatement") << getArrow();
    statement->getIfStatement()->Accept(this);
    file << getEndLine();
    file << getNodeNameWithLabel("if", currentExpressionId, "if") << getArrow() <<
         getNodeNameWithLabel("elseStatement", currentExpressionId, "elseStatement") << getArrow();
    statement->getElseStatement()->Accept(this);
}


void CPrintVisitor::Visit(CWhileStatement* statement) {
	int currentExpressionId = ++expressionId;
	file << getNodeNameWithLabel("While", currentExpressionId, "While") << getArrow() <<
         getNodeNameWithLabel("condition", currentExpressionId, "condition") << getArrow();
	statement->getCondition()->Accept(this);
	file << getEndLine();
	file << getNodeNameWithLabel("While", currentExpressionId, "While") << getArrow() <<
         getNodeNameWithLabel("body", currentExpressionId, "body") << getArrow();
	statement->getBody()->Accept(this);
}

void CPrintVisitor::Visit(CListStatement* statement) {
    int currentExpressionId = ++expressionId;

    auto statements = statement->GetStatements();
    size_t numberOfIterations = statements.size();

    for(size_t i = 0; i < numberOfIterations; ++i) {
        if (i != numberOfIterations - 1) {
            file << getNodeNameWithLabel("List" + statement->GetStatementType(), currentExpressionId, "List") << getArrow();
            statements[i]->Accept(this);
            file << getEndLine();
        } else {
            file << getNodeNameWithLabel("List" + statement->GetStatementType(), currentExpressionId, "List") << getArrow();
            statements[numberOfIterations - 1]->Accept(this);
        }
    }
    if (numberOfIterations == 0) {
        file << getNodeNameWithLabel("Empty", currentExpressionId, "Empty");
    }
}

void CPrintVisitor::Visit(CListExpression* statement) {
    int currentExpressionId = ++expressionId;

    auto statements = statement->GetExpressions();
    size_t numberOfIterations = statements.size();

    for(size_t i = 0; i < numberOfIterations; ++i) {
        if (i != numberOfIterations - 1) {
            file << getNodeNameWithLabel("List" + statement->GetExpressionType(), currentExpressionId, "List") << getArrow();
            statements[i]->Accept(this);
            file << getEndLine();
        } else {
            file << getNodeNameWithLabel("List" + statement->GetExpressionType(), currentExpressionId, "List") << getArrow();
            statements[numberOfIterations - 1]->Accept(this);
        }
    }
    if (numberOfIterations == 0) {
        file << getNodeNameWithLabel("Empty", currentExpressionId, "Empty");
    }
}

void CPrintVisitor::Visit(CVarDecl* decl) {
    int currentExpressionId = ++expressionId;
    file << getNodeNameWithLabel("VariableDeclaration", currentExpressionId, "VariableDeclaration") << getArrow();
    file << getNodeNameWithLabel(decl->GetTypeName(), ++expressionId, decl->GetTypeName()) << getEndLine();
    file << getNodeNameWithLabel("VariableDeclaration", currentExpressionId, "VariableDeclaration") << getArrow();
    file << getNodeNameWithLabel(decl->GetVariableName(), ++expressionId, decl->GetVariableName());
}

void CPrintVisitor::Visit(CGetItemExpression* expression) {

    int currentExpressionId = ++expressionId;

    file << getNodeNameWithLabel("GetItem", currentExpressionId, "GetItem")  << getArrow();
    expression->GetObject()->Accept(this);
    file << getEndLine();
    file << getNodeNameWithLabel("GetItem", currentExpressionId, "GetItem")  << getArrow();
    expression->GetIndex()->Accept(this);
}

void CPrintVisitor::Visit( CMethod* statement ) {
    int currentExpressionId = ++expressionId;
    file << getNodeNameWithLabel("CMethod",currentExpressionId,"CMethod") << getArrow() <<
             getNodeNameWithLabel("Visibility",currentExpressionId,"Visibility") << getArrow();
    file << getNodeNameWithLabel(statement->getVisibility(), currentExpressionId, statement->getVisibility());
    file << getEndLine();

    file << getNodeNameWithLabel("CMethod",currentExpressionId,"CMethod") << getArrow() <<
         getNodeNameWithLabel("TypeName",currentExpressionId,"TypeName") << getArrow();
    file << getNodeNameWithLabel(statement->getTypeName(), currentExpressionId, statement->getTypeName());
    file << getEndLine();

    file << getNodeNameWithLabel("CMethod",currentExpressionId,"CMethod") << getArrow() <<
         getNodeNameWithLabel("ID",currentExpressionId,"ID") << getArrow();
    statement->getId()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("CMethod",currentExpressionId,"CMethod") << getArrow() <<
         getNodeNameWithLabel("Params",currentExpressionId,"Params") << getArrow();
    statement->getParameters()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("CMethod", currentExpressionId, "CMethod") << getArrow()
         << getNodeNameWithLabel("VariablesDeclarations", currentExpressionId, "VariablesDeclarations")
         << getArrow();
    statement->getListDeclarations()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("CMethod",currentExpressionId,"CMethod") << getArrow() <<
         getNodeNameWithLabel("ListStatementsInMethod",currentExpressionId,"ListStatementsInMethod") << getArrow();
    statement->getListStatements()->Accept(this);

    if (statement->getReturnExpression() != nullptr) {
        file << getEndLine();
        file << getNodeNameWithLabel("CMethod", currentExpressionId, "CMethod") << getArrow() <<
             getNodeNameWithLabel("ReturnExpression", currentExpressionId, "ReturnExpression") << getArrow();
        statement->getReturnExpression()->Accept(this);
    }
}


void CPrintVisitor::Visit( CMethodCallExpression* exp) {
    int currentExpressionId = ++expressionId;
    file << getNodeNameWithLabel("CMethodCallExpression",currentExpressionId,"CMethodCallExpression") << getArrow() <<
         getNodeNameWithLabel("Object",currentExpressionId,"Object") << getArrow();
    exp->getObject()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("CMethodCallExpression",currentExpressionId,"CMethodCallExpression") << getArrow() <<
         getNodeNameWithLabel("MethodId",currentExpressionId,"MethodId") << getArrow();
    exp->getMethodId()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("CMethodCallExpression",currentExpressionId,"CMethodCallExpression") << getArrow() <<
         getNodeNameWithLabel("Arguments",currentExpressionId,"Arguments") << getArrow();
    exp->getArguments()->Accept(this);
}

void CPrintVisitor::Visit( CClass* statement ) {
    int currentExpressionId = ++expressionId;

    file << getNodeNameWithLabel("CClass",currentExpressionId,"CClass") << getArrow() <<
         getNodeNameWithLabel("ID",currentExpressionId,"ID") << getArrow();
    statement->getId()->Accept(this);
    file << getEndLine();

    if (statement->getBaseId().get() != nullptr) {
        file << getNodeNameWithLabel("CClass",currentExpressionId,"CClass") << getArrow() <<
             getNodeNameWithLabel("BaseID",currentExpressionId,"BaseID") << getArrow();
        statement->getBaseId()->Accept(this);
        file << getEndLine();
    }

    file << getNodeNameWithLabel("CClass",currentExpressionId,"CClass") << getArrow() <<
         getNodeNameWithLabel("Fields",currentExpressionId,"Fields") << getArrow();
    statement->getFields()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("CClass",currentExpressionId,"CClass") << getArrow() <<
         getNodeNameWithLabel("Methods",currentExpressionId,"Methods") << getArrow();
    statement->getMethods()->Accept(this);
}


void CPrintVisitor::Visit( CMainClass* statement ) {
    int currentExpressionId = ++expressionId;

    file << getNodeNameWithLabel("CMainClass",currentExpressionId,"CMainClass") << getArrow() <<
         getNodeNameWithLabel("ID",currentExpressionId,"ID") << getArrow();
    statement->GetClassId()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("CMainClass",currentExpressionId,"CMainClass") << getArrow() <<
         getNodeNameWithLabel("MainMethod",currentExpressionId,"MainMethod") << getArrow();
    statement->GetMainMethod()->Accept(this);
}


void CPrintVisitor::Visit(CProgram *statement) {
    int currentExpressionId = ++expressionId;

    file << getNodeNameWithLabel("CProgram",currentExpressionId,"CProgram") << getArrow() <<
         getNodeNameWithLabel("MainClass",currentExpressionId,"MainClass") << getArrow();
    statement->GetMainClass()->Accept(this);
    file << getEndLine();

    file << getNodeNameWithLabel("CProgram",currentExpressionId,"CProgram") << getArrow() <<
         getNodeNameWithLabel("MinorClass",currentExpressionId,"MinorClass") << getArrow();
    statement->GetMinorClasses()->Accept(this);
}


std::string CPrintVisitor::getArrow() const {
	return " -> ";
}

std::string CPrintVisitor::getEndLine() const {
	return ";\n\t";
}

std::string CPrintVisitor::getNodeNameWithLabel(std::string label, int id, std::string nodeName ) const {
    return "{" + nodeName  + delim +
           std::to_string(id) +"[label=\"" + label + "\"]" +  "}";
}






