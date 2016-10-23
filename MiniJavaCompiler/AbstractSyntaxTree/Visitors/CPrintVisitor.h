#pragma once

#include <Visitors/IVisitor.h>
#include <Nodes/INode.h>
#include <vector>

#include <fstream>
#include <Nodes/CIfElseStatement.h>

class CPrintVisitor: public IVisitor {
public:
	void StartVisit(INode* startNode, std::string filename);

	void Visit(CAssignStatement*);
	void Visit(CCompoundStatement*);
	void Visit(CPrintStatement*);

	void Visit(CIdExpression*);
	void Visit(CNumExpression*);
	void Visit(COperationExpression*);
	void Visit(CIfElseStatement*);

private:

	int expressionId;
    std::vector<std::string> idsOfTokenWithBoxShape;
	std::string delim = "A";

	std::ofstream file;

	std::string getArrow();
	std::string getEndLine();

};
