#include <Visitors/CPrintVisitor.h>

#include <Nodes/CAssignStatement.h>
#include <Nodes/CAssignItemStatement.h>
#include <Nodes/CCompoundStatement.h>
#include <Nodes/CPrintStatement.h>
#include <Nodes/CIfElseStatement.h>
#include <Nodes/CWhileStatement.h>
#include <Nodes/CListStatement.h>

#include <Nodes/CVarDecl.h>
#include <Nodes/CListVarDecl.h>
#include <Nodes/CMethod.h>
#include <Nodes/CListMethod.h>

#include <Nodes/CIdExpression.h>
#include <Nodes/CBoolExpression.h>
#include <Nodes/CNumExpression.h>
#include <Nodes/CNotExpression.h>
#include <Nodes/COperationExpression.h>
#include <Nodes/CLengthExpression.h>
#include <Nodes/CArrayConstructionExpression.h>
#include <Nodes/CConstructClassExpression.h>

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

	file << GetOperationSign(expression->GetOperationType()) << delim << currentExpressionId << getArrow();
	expression->GetLeftOperand()->Accept(this);
	file << getEndLine();
	file << GetOperationSign(expression->GetOperationType()) << delim << currentExpressionId << getArrow();
	expression->GetRightOperand()->Accept(this);
}

void CPrintVisitor::Visit(CNumExpression* expression) {
	++expressionId;
	std::string newId = std::string("Number") + std::to_string(expression->GetNumber()) + delim + std::to_string(expressionId);
	file << newId;
    idsOfTokenWithBoxShape.push_back(newId);
}

void CPrintVisitor::Visit(CBoolExpression* expression) {
	++expressionId;
	std::string newId = std::string("Bool") + std::to_string(expression->GetValue()) + delim + std::to_string(expressionId);
	file << newId;
	idsOfTokenWithBoxShape.push_back(newId);
}

void CPrintVisitor::Visit(CIdExpression* expression) {
	++expressionId;
	std::string newId = expression->GetName() + delim + std::to_string(expressionId);
	file << newId;
	idsOfTokenWithBoxShape.push_back(newId);
}

void CPrintVisitor::Visit(CNotExpression* expression) {
    ++expressionId;
    file << "not" << expressionId << getArrow();
    expression->GetExpression()->Accept(this);
}

void CPrintVisitor::Visit(CLengthExpression* expression) {
	++expressionId;
	expression->getExpression()->Accept(this);
	file << getArrow() << "getLength" + delim + std::to_string(expressionId);
}

void CPrintVisitor::Visit(CArrayConstructionExpression* expression) {
    ++expressionId;
    file << "newIntArray" + delim + std::to_string(expressionId) + getArrow() +
            "withSize" + std::to_string(expressionId) + getArrow();
    expression->getSize()->Accept(this);

}


void CPrintVisitor::Visit(CConstructClassExpression* expression) {
    ++expressionId;
    file << "constructClass" + delim + std::to_string(expressionId) + getArrow() +
             expression->getClassID()->GetName() + std::to_string(expressionId);
}

void CPrintVisitor::Visit(CAssignStatement* statement) {
	int currentExpressionId = ++expressionId;
	file << "assignment" << delim << currentExpressionId << getArrow();
	statement->GetVariable()->Accept(this);
	file << getEndLine();
	file << "assignment" << delim << currentExpressionId << getArrow();
	statement->GetExpression()->Accept(this);
}


void CPrintVisitor::Visit(CAssignItemStatement* statement) {
    int currentExpressionId = ++expressionId;
    file << "assignmentItem" << delim << currentExpressionId << getArrow();
    statement->getId()->Accept(this);
    file << "[";
    statement->getExpressionInBrackets()->Accept(this);
    file << "]";
    file << getEndLine();
    file << "assignmentItem" << delim << currentExpressionId << getArrow();
    statement->getAssignedExpression()->Accept(this);
}

void CPrintVisitor::Visit(CPrintStatement* statement) {
	++expressionId;
	file << "println" << expressionId <<  getArrow();
	statement->GetExpression()->Accept(this);
	//file << getEndLine();
}

void CPrintVisitor::Visit(CCompoundStatement* statement) {
	int currentExpressionId = ++expressionId;
	file << "compound" << delim << currentExpressionId << getArrow();
	statement->GetLeftOperand()->Accept(this);
	file << getEndLine();
	file << "compound" << delim << currentExpressionId << getArrow();
	statement->GetRightOperand()->Accept(this);
}


void CPrintVisitor::Visit(CIfElseStatement* statement) {
	int currentExpressionId = ++expressionId;
	file << "if" << delim << currentExpressionId << getArrow() << "condition" << currentExpressionId << getArrow();
	statement->getExpression()->Accept(this);
	file << getEndLine();
    file << "if" << delim << currentExpressionId << getArrow() << "ifStatement" << currentExpressionId << getArrow();
    statement->getIfStatement()->Accept(this);
    file << getEndLine();
    file << "if" << delim << currentExpressionId << getArrow() << "elseStatement" << currentExpressionId << getArrow();
    statement->getElseStatement()->Accept(this);
}


void CPrintVisitor::Visit(CWhileStatement* statement) {
	int currentExpressionId = ++expressionId;
	file << "While" << delim << currentExpressionId << getArrow() << "condition" << currentExpressionId << getArrow();
	statement->getCondition()->Accept(this);
	file << getEndLine();
	file << "While" << delim << currentExpressionId << getArrow() << "body" << currentExpressionId << getArrow();
	statement->getBody()->Accept(this);
}

void CPrintVisitor::Visit(CListStatement* statement) {
    int currentExpressionId = ++expressionId;
    for(size_t i = 0; i < statement->GetStatements().size() - 1; ++i){
        file << "ListStatement" << delim << currentExpressionId << getArrow();
        statement->GetStatements()[i]->Accept(this);
        file << getEndLine();
    }
	file << "ListStatement" << delim << currentExpressionId << getArrow();
	statement->GetStatements()[statement->GetStatements().size() - 1]->Accept(this);
}

void CPrintVisitor::Visit(CVarDecl* decl) {
    int currentExpressionId = ++expressionId;
    file << "VariableDeclaration" << delim << currentExpressionId << getArrow();
    file << decl->GetTypeName() << delim << ++expressionId << getEndLine();
    file << "VariableDeclaration" << delim << currentExpressionId << getArrow();
    file << decl->GetVariableName() << delim << ++expressionId;
}

void CPrintVisitor::Visit(CListVarDecl* declarationList) {
    int currentExpressionId = ++expressionId;
    auto declarations = declarationList->GetDeclarations();
    for(size_t i = 0; i < declarations.size() - 1; ++i){
        file << "ListVarDecl" << delim << currentExpressionId << getArrow();
        declarations[i]->Accept(this);
        file << getEndLine();
    }
    file << "ListVarDecl" << delim << currentExpressionId << getArrow();
    declarations[declarations.size() - 1]->Accept(this);
}


void CPrintVisitor::Visit( CMethod* statement ) {
    int currentExpressionId = ++expressionId;
    file << "CMethod" << delim << currentExpressionId << getArrow() << "Visibility" << currentExpressionId << getArrow();
    file << statement->getVisibility() << delim << currentExpressionId;
    file << getEndLine();

    file << "CMethod" << delim << currentExpressionId << getArrow() << "TypeName" << currentExpressionId << getArrow();
    file << statement->getTypeName() << delim << currentExpressionId ;
    file << getEndLine();

    file << "CMethod" << delim << currentExpressionId << getArrow() << "ID" << currentExpressionId << getArrow();
    statement->getId()->Accept(this);
    file << getEndLine();


    file << "CMethod" << delim << currentExpressionId << getArrow() << "Params" << currentExpressionId << getArrow();
    statement->getParameters()->Accept(this);
    file << getEndLine();


    file << "CMethod" << delim << currentExpressionId << getArrow() << "VariablesDeclarations" << currentExpressionId << getArrow();
    statement->getListDeclarations()->Accept(this);
    file << getEndLine();


    file << "CMethod" << delim << currentExpressionId << getArrow() << "ListStatementsInMethod" << currentExpressionId << getArrow();
    statement->getListStatements()->Accept(this);
    file << getEndLine();


    file << "CMethod" << delim << currentExpressionId << getArrow() << "ReturnExpression" << currentExpressionId << getArrow();
    statement->getReturnExpression()->Accept(this);


}


void CPrintVisitor::Visit(CListMethod *statement) {
    int currentExpressionId = ++expressionId;
    auto methods = statement->GetMethods();
    for(size_t i = 0; i < methods.size() - 1; ++i){
        file << "ListMethod" << delim << currentExpressionId << getArrow();
        methods[i]->Accept(this);
        file << getEndLine();
    }
    file << "ListMethod" << delim << currentExpressionId << getArrow();
    methods[methods.size() - 1]->Accept(this);
}

std::string CPrintVisitor::getArrow() const {
	return " -> ";
}

std::string CPrintVisitor::getEndLine() const {
	return ";\n\t";
}






