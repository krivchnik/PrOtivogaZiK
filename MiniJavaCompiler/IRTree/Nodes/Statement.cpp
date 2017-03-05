#include <IRTree/Nodes/Statement.h>

#include <IRTree/Nodes/Expression.h>
#include <assert.h>
#include <IRTree/Canonization.h>

using namespace IRTree;


CStatement::~CStatement() {}

CMoveStatement::CMoveStatement( const CExpression* _destination, const CExpression* _source )
    : destination( _destination ), source( _source ) {}
CMoveStatement::CMoveStatement( std::shared_ptr<const CExpression> _destination, std::shared_ptr<const CExpression> _source )
    : destination( _destination ), source( _source ) {}
CMoveStatement::~CMoveStatement() {}

std::shared_ptr<const CStatement> CMoveStatement::Clone() const {
    return std::shared_ptr<const CStatement>( new CMoveStatement(
            destination->Clone(), source->Clone()
    ));
}

std::shared_ptr<const CStatement> CMoveStatement::Canonize() {
    std::shared_ptr<const CExpression> destinationCanon = destination->Canonize();
    std::shared_ptr<const CExpression> sourceCanon = source->Canonize();

    std::shared_ptr<const CStatement> result;
    const CEseqExpression* sourceCanonEseq = CastToEseqExpression( sourceCanon.get() );
    if ( sourceCanonEseq ) {
        result = std::shared_ptr<const CStatement>(
                new CSeqStatement(
                        sourceCanonEseq->Statement()->Clone(),
                        std::shared_ptr<const CStatement>(
                                new CMoveStatement(
                                        destinationCanon,
                                        sourceCanonEseq->Expression()->Clone()
                                )
                        ) )
                );
    } else {
        result = std::shared_ptr<const CStatement>(
                new CMoveStatement(
                        destinationCanon,
                        sourceCanon
                )
        );
    }
    return result;
}

CExpStatement::CExpStatement( const CExpression* _expression ) : expression( _expression ) {}
CExpStatement::CExpStatement( std::shared_ptr<const CExpression> _expression )
    : expression( _expression ) {}
CExpStatement::~CExpStatement() {}

std::shared_ptr<const CStatement> CExpStatement::Canonize() {
    std::shared_ptr<const CExpression> expressionCanon = expression->Canonize();
    std::shared_ptr<const CStatement> result;
    const CEseqExpression* eseqExpression = CastToEseqExpression( expressionCanon.get() );
    if ( eseqExpression ) {
                result = std::shared_ptr<const CStatement>(
                            new CSeqStatement(
                                        eseqExpression->Statement()->Clone(),
                            std::shared_ptr<const CStatement>(
                                                    new CExpStatement(
                                                                eseqExpression->Expression()->Clone()
                                                            )
                                                ) )
                                    );
            } else {
                result = std::shared_ptr<const CStatement>(
                            new CExpStatement( expressionCanon ) );
            }
    return result;
}

std::shared_ptr<const CStatement> CExpStatement::Clone() const {
    return std::shared_ptr<const CStatement>( new CExpStatement(expression->Clone()));
}

CJumpStatement::CJumpStatement( CLabel _target )
    : target( _target ) {}
CJumpStatement::~CJumpStatement() {}

std::shared_ptr<const CStatement> CJumpStatement::Canonize() {
    assert(false);
    return CStatement::Canonize();
}

std::shared_ptr<const CStatement> CJumpStatement::Clone() const {
    return std::shared_ptr<const CStatement>( new CJumpStatement(target));
}

CJumpConditionalStatement::CJumpConditionalStatement( TLogicOperatorType _operation,
    const CExpression* left, const CExpression* right,
    CLabel _labelTrue, CLabel _labelFalse )
        : leftOperand( left ), rightOperand( right ),
        labelTrue( _labelTrue ), labelFalse( _labelFalse ), operation( _operation ) {}
CJumpConditionalStatement::CJumpConditionalStatement( TLogicOperatorType _operation,
    std::shared_ptr<const CExpression> left, std::shared_ptr<const CExpression> right,
    CLabel _labelTrue, CLabel _labelFalse )
        : leftOperand( left ), rightOperand( right ),
        labelTrue( _labelTrue ), labelFalse( _labelFalse ), operation( _operation ) {}
CJumpConditionalStatement::~CJumpConditionalStatement() {}

std::shared_ptr<const CStatement> CJumpConditionalStatement::Canonize() {
    assert(false);
    std::shared_ptr<const CExpression> expressionLeftCanon = leftOperand->Canonize();
    std::shared_ptr<const CExpression> expressionRightCanon = rightOperand->Canonize();

    std::shared_ptr<const CStatement> result;
    const CEseqExpression* eseqExpressionLeftCanon = CastToEseqExpression( expressionLeftCanon.get() );
    const CEseqExpression* eseqExpressionRightCanon = CastToEseqExpression( expressionRightCanon.get() );
    if ( eseqExpressionLeftCanon ) {
            result = std::shared_ptr<const CStatement>(
                    new CSeqStatement(
                            eseqExpressionLeftCanon->Statement()->Clone(),
                            std::shared_ptr<const CStatement>(
                                    new CJumpConditionalStatement(
                                            operation,
                                            eseqExpressionLeftCanon->Expression()->Clone(),
                                            expressionRightCanon,
                                            labelTrue,
                                            labelFalse
                                    )
                            ) )

            );
    } else if ( eseqExpressionRightCanon ) {
        if ( AreCommuting( eseqExpressionRightCanon->Statement(), expressionLeftCanon.get() )  ) {
            result = std::shared_ptr<const CStatement>(
                    new CSeqStatement(
                            eseqExpressionRightCanon->Statement()->Clone() ,
                    std::shared_ptr<const CStatement>(
                                    new CJumpConditionalStatement(
                                            operation,
                                            expressionLeftCanon,
                                            eseqExpressionRightCanon->Expression()->Clone(),
                                            labelTrue,
                                            labelFalse
                                    )
                            ) )
                    );
        } else {
            CTemp temp;
            result = std::shared_ptr<const CStatement>(
                    new CSeqStatement(
                            new CMoveStatement(
                                    std::shared_ptr<const CExpression>(
                                            new CTempExpression( temp )),
                                    expressionLeftCanon
                            ),
                            new CSeqStatement(
                                    eseqExpressionRightCanon->Statement()->Clone(),
                                    std::shared_ptr<const CStatement>(
                                            new CJumpConditionalStatement(
                                                    operation,
                                                    std::shared_ptr<const CExpression>(
                                                            new CTempExpression( temp )
                                                    ),
                                                    eseqExpressionRightCanon->Expression()->Clone(),
                                                    labelTrue,
                                                    labelFalse
                                            )
                                    ) )
                            )
                    );
        }
    } else {
        result = std::shared_ptr<const CStatement>(
                new CJumpConditionalStatement(
                        operation,
                        expressionLeftCanon,
                        expressionRightCanon,
                        labelTrue,
                        labelFalse
                )
        );
    }
    return result;
}

std::shared_ptr<const CStatement> CJumpConditionalStatement::Clone() const {
    return std::shared_ptr<const CStatement>( new CJumpConditionalStatement(
            operation, leftOperand->Clone(), rightOperand->Clone(),
            labelTrue, labelFalse
    ));
}

CSeqStatement::CSeqStatement(const CStatement* _left, const CStatement* _right )
    : leftStatement( _left ), rightStatement( _right ) {}
CSeqStatement::CSeqStatement( std::shared_ptr<const CStatement> _left, std::shared_ptr<const CStatement> _right )
    : leftStatement( _left ), rightStatement( _right ) {}

std::shared_ptr<const CStatement> CSeqStatement::Canonize() {
    std::shared_ptr<const CStatement> statementLeftCanon = leftStatement->Canonize();
    std::shared_ptr<const CStatement> statementRightCanon = rightStatement->Canonize();
            return std::shared_ptr<const CStatement>(
                    new CSeqStatement(
                                statementLeftCanon,
                                statementRightCanon
                            )) ;
}

std::shared_ptr<const CStatement> CSeqStatement::Clone() const {
    return std::shared_ptr<const CStatement>( new CSeqStatement(
            leftStatement->Clone(), rightStatement->Clone()
    ));
}

CLabelStatement::CLabelStatement( CLabel _label ) : label( _label ) {}
CLabelStatement::~CLabelStatement() {}

std::shared_ptr<const CStatement> CLabelStatement::Canonize() {
    return Clone();
}

std::shared_ptr<const CStatement> CLabelStatement::Clone() const {
    return std::shared_ptr<const CStatement>( new CLabelStatement(label));
}
