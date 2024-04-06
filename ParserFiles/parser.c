#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below
ParserInfo classDeclar();
ParserInfo memberDeclar();
ParserInfo classVarDeclar();
ParserInfo type();
ParserInfo subroutineDeclar();
ParserInfo paramList();
ParserInfo subroutineBody();
ParserInfo statement();
ParserInfo varDeclarstatement();
ParserInfo letStatement();
ParserInfo ifStatement();
ParserInfo whileStatement();
ParserInfo doStatement();
ParserInfo subroutineCall();
ParserInfo expressionList();
ParserInfo returnstatement();
ParserInfo expression();
ParserInfo relationalExpression();
ParserInfo ArithmeticExpression();
ParserInfo term();
ParserInfo factor();
ParserInfo operand();


// classDeclar -> class identifier { {memberDeclar } }
ParserInfo classDeclar(){
	ParserInfo returnVal;
	// Check for class
	Token token = GetNextToken();
	if (token.tp == RESWORD && !strcmp(token.lx, "class")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = classExpected;
	}

	// Check for identifier
	token = GetNextToken();
	if(token.tp == ID){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = idExpected;
	}

	// Check for open bracket
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "{")){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = openBraceExpected;
	}

	// zero or many of memberdeclare
	token = PeekNextToken();
	while(token.tp != EOFile && strcmp(token.lx,"}")){
		memberDeclar();
		token = PeekNextToken();
	}

	// Check for close bracket
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "}")){
		returnVal.er = none;
	}
	else {
		returnVal.tk = token;
		returnVal.er = closeBraceExpected;
	}
}

// memberDeclar -> classVarDeclar | subroutineDeclar
ParserInfo memberDeclar(){
	ParserInfo returnVal;
	Token token = PeekNextToken();
	if( token.tp == RESWORD && (!strcmp(token.lx, "static" )) || (!strcmp(token.lx, "field" ))) {
		returnVal = classVarDeclar();
	}
	else if( token.tp == RESWORD && (!strcmp(token.lx, "constructor" )) || (!strcmp(token.lx, "function" )) || (!strcmp(token.lx, "method" ))){
		returnVal = subroutineDeclar();
	}
	else {
		returnVal.tk = token;
		returnVal.er = memberDeclarErr;
	}
	return returnVal;
}

// classVarDeclar -> (static | field) type identifier {, identifier};
ParserInfo classVarDeclar(){
	ParserInfo returnVal;
	// Check for static or field
	Token token =GetNextToken();
	if( token.tp == RESWORD && !strcmp(token.lx, "static")){
		type();
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "field")){
		type();
	}
	else{
		returnVal.tk = token;
		returnVal.er = classVarErr;
	}

	// Check for identifier
	token = GetNextToken();
	if(token.tp == ID){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = idExpected;
	}
}


// type -> int | char | boolean | identifier	
ParserInfo type(){
	ParserInfo returnVal;
	Token token = GetNextToken();
	if(token.tp == RESWORD && !strcmp(token.lx, "int")){
		returnVal.er = none;
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "char")){
		returnVal.er = none;
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "boolean")){
		returnVal.er = none;
	}
	else if(token.tp == ID){
		returnVal.er = none;
	}
	else {
		returnVal.tk = token;
		returnVal.er = illegalType;
	}
	return returnVal;
}

// subroutineDeclar -> (constrcutor | function | method) (type | void) identifier (paramList) subroutineBody
ParserInfo subroutineDeclar(){
	ParserInfo returnVal;
	Token token = GetNextToken();
	if(token.tp == RESWORD && !strcmp(token.lx, "constructor")){
		returnVal.er = none;
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "function")){
		returnVal.er = none;
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "method")){
		returnVal.er = none;
	}
	else{
		returnVal.tk = token;
		returnVal.er = subroutineDeclarErr;
		return returnVal;
	}
	token = PeekNextToken();
	if(token.tp == RESWORD && (!strcmp(token.lx, "int")) || (!strcmp(token.lx, "char")) || (!strcmp(token.lx, "boolean"))){
		returnVal=type();
	}

}



int InitParser (char* file_name)
{
	InitLexer(file_name);
	return 1;
}

ParserInfo Parse ()
{
	ParserInfo pi;

	// implement the function


	pi.er = none;
	return pi;
}

int StopParser ()
{
	return 1;
}

#ifndef TEST_PARSER
int main ()
{
	InitLexer("Ball.jack");
	printf("testing\n");
	classDeclar();
	return 1;
}
#endif
