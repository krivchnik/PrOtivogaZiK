#pragma once

#include <IRTree/Nodes/NodeNames.h>
#include <IRTree/Visitors/Visitor.h>

#include <IRTree/Nodes/Expression.h>
#include <IRTree/Nodes/ExpressionList.h>
#include <IRTree/Nodes/Statement.h>
#include <IRTree/Nodes/StatementList.h>

namespace IRTree {

class CDoubleCallEliminationVisitor : public CVisitor {
public:
    CDoubleCallEliminationVisitor( bool _verbose = false ) : CVisitor( _verbose ) {}
    ~CDoubleCallEliminationVisitor() {}

    std::shared_ptr<const CStatement> ResultTree();

    // Visitors for different node types.
    void Visit( const CConstExpression* expression ) override;
    void Visit( const CNameExpression* expression ) override;
    void Visit( const CTempExpression* expression ) override;
    void Visit( const CBinaryExpression* expression ) override;
    void Visit( const CMemExpression* expression ) override;
    void Visit( const CCallExpression* expression ) override;
    void Visit( const CEseqExpression* expression ) override;

    void Visit( const CExpStatement* expression ) override;
    void Visit( const CJumpConditionalStatement* expression ) override;
    void Visit( const CJumpStatement* expression ) override;
    void Visit( const CLabelStatement* expression ) override;
    void Visit( const CMoveStatement* expression ) override;
    void Visit( const CSeqStatement* expression ) override;

    void Visit( const CExpressionList* list ) override;
    void Visit( const CStatementList* list ) override;

private:
    void updateLastExpression( const CExpression* newLastExpression );
    void updateLastExpression( std::shared_ptr<const CExpression> newLastExpression );

    void updateLastExpressionList( const CExpressionList* newLastExpressionList );
    void updateLastExpressionList( std::shared_ptr<const CExpressionList> newLastExpressionList );

    void updateLastStatement( const CStatement* newLastStatement );
    void updateLastStatement( std::shared_ptr<const CStatement> newLastStatement );

    void updateLastStatementList( const CStatementList* newLastStatementList );
    void updateLastStatementList( std::shared_ptr<const CStatementList> newLastStatementList );

    std::shared_ptr<const CExpression> lastExpression;
    std::shared_ptr<const CStatement> lastStatement;
    std::shared_ptr<const CExpressionList> lastExpressionList;
    std::shared_ptr<const CStatementList> lastStatementList;
};

}
