/* Mini Calculator */
/* calc.y */

%{
#include <CommonInclude.h>
int yyerror(char *s);
int yylex(void);

extern shared_ptr<IStatement> ans;
%}
	
%union{
  int                int_val;
  char*              op_val;
  INode*             node_val;
  IExpression*       expr_val;
  IStatement*        stat_val;
}

%start	input

%token				LPBRACKET RPBRACKET LFBRACKET RFBRACKET LSBRACKET RSBRACKET SEMICOLON
                    PUBLIC PRIVATE STATIC
                    INT BOOLEAN VOID STRING
                    CLASS EXTENDS
                    IF ELSE WHILE
                    RETURN PRINTLN LENGTH
                    THIS NEW
                    MAIN
%token <int_val>	INTEGER_LITERAL
%token <op_val>     ID

%type  <expr_val>	exp
%type  <stat_val>   stat

%left 	OR
%left   AND
%left	PLUS MINUS
%left	STAR MOD

%nonassoc ASSIGN
%nonassoc LESS

%%

input:	/* empty */
		| stat	{ ans = shared_ptr<IStatement>($1); return 0;}
		;

exp: 	INTEGER_LITERAL	{ $$ = new CNumExpression($1); }
		| exp PLUS exp	{ $$ = new COperationExpression(shared_ptr<IExpression>($1), shared_ptr<IExpression>($3), COperationExpression::ADDITION); }
		| exp MINUS exp	{ $$ = new COperationExpression(shared_ptr<IExpression>($1), shared_ptr<IExpression>($3), COperationExpression::SUBTRACTION); }
		| exp STAR exp	{ $$ = new COperationExpression(shared_ptr<IExpression>($1), shared_ptr<IExpression>($3), COperationExpression::MULTIPLICATION); }
		| exp MOD exp	{ $$ = new COperationExpression(shared_ptr<IExpression>($1), shared_ptr<IExpression>($3), COperationExpression::MOD); }
		| exp AND exp	{ $$ = new COperationExpression(shared_ptr<IExpression>($1), shared_ptr<IExpression>($3), COperationExpression::AND); }
		| exp OR exp	{ $$ = new COperationExpression(shared_ptr<IExpression>($1), shared_ptr<IExpression>($3), COperationExpression::OR); }
		| exp LESS exp	{ $$ = new COperationExpression(shared_ptr<IExpression>($1), shared_ptr<IExpression>($3), COperationExpression::LESS); }

		| ID 							{ $$ = new CIdExpression(std::string($1)); }
//		| exp LSBRACKET exp RSBRACKET   { $$ = new }
//		| exp POINT LENGTH 				{ $$ = new }
		;

stat 	: LFBRACKET stat RFBRACKET                  		{ $$ = $2; }
    	| stat stat                        					{ $$ = new CCompoundStatement(shared_ptr<IStatement>($1), 
    																					  shared_ptr<IStatement>($2)); }
//
//    	| IF LFBRACKET exp RFBRACKET stat ELSE stat 		{ $$ = new CIfElseStatement(shared_ptr<IExpression>($3),
//    																					shared_ptr<IStatement>($5),
//    																					shared_ptr<IStatement>($7)); }
//
//    	| WHILE LFBRACKET exp RFBRACKET stat         		{ $$ = new CWhileStatement(shared_ptr<IExpression>($3),
//    																				   shared_ptr<IStatement>($5)); }

    	| PRINTLN LPBRACKET exp RPBRACKET SEMICOLON 		{ $$ = new CPrintStatement(shared_ptr<IExpression>($3)); }

    	| ID ASSIGN exp SEMICOLON                			{ $$ = new CAssignStatement(shared_ptr<CIdExpression>(new CIdExpression(std::string($1))), 
    																					shared_ptr<IExpression>($3)); }

//    	| ID LSBRACKET exp RFBRACKET ASSIGN exp SEMICOLON 	{ $$ = new CAssignItemStatement(shared_ptr<CIdExpression>(new CIdExpression(std::string($1))),
//    																						shared_ptr<IExpression>($3),
//    																						shared_ptr<IExpression>($6)); }
;		
/*
Goal: MainClass ( ClassDeclaration )* <EOF> {$$ = new IExpr($1)}

MainClass: "class" Identifier "{" "public" "static" "void" "main" "(" "String" "[" "]" Identifier ")" "{" Statement "}" "}"

ClassDeclaration: "class" Identifier ( "extends" Identifier )? "{" ( VarDeclaration )* ( MethodDeclaration )* "}"

VarDeclaration: Type Identifier ";"

MethodDeclaration: "public" | “private” Type Identifier "(" ( Type Identifier ( "," Type Identifier )* )? ")" "{" ( VarDeclaration )*( Statement )* "return" Expression ";" "}"

Type: "int" "[" "]"
| "boolean"
| "int"
| Identifier

Statement: 
	"{" ( Statement )* "}"
	| "if" "(" Expression ")" Statement "else" Statement
	| "while" "(" Expression ")" Statement
	| "System.out.println" "(" Expression ")" ";"
	| Identifier "=" Expression ";"
	| Identifier "[" Expression "]" "=" Expression ";"

Expression:
	| Expression "[" Expression "]"
	| Expression "." "length"
	| Expression "." Identifier "(" ( Expression ( "," Expression )* )? ")"
	| <INTEGER_LITERAL>
	| "true"
	| "false"
	| Identifier
	| "this"
	| "new" "int" "[" Expression "]"
	| "new" Identifier "(" ")"
	| "!" Expression | "(" Expression ")"

Identifier ::= <IDENTIFIER>
*/

%%

int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  exit(1);
}

int yyerror(char *s)
{
  return yyerror(string(s));
}


