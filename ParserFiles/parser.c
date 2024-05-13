#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions belowcc -std=c99 lexer.h parser.h lexer.c parser.c
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
ParserInfo returnStatement();
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
		return returnVal;
	}

	// Check for identifier
	token = GetNextToken();
	if(token.tp == ID){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = idExpected;
		return returnVal;
	}

	// Check for open bracket
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "{")){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = openBraceExpected;
		return returnVal;
	}

	// zero or many of memberdeclare
	token = PeekNextToken();
	while(token.tp != EOFile && strcmp(token.lx,"}")){
		returnVal = memberDeclar();
		if(returnVal.er != none){
			return returnVal;
		}
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
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
}

// memberDeclar -> classVarDeclar | subroutineDeclar
ParserInfo memberDeclar(){
	ParserInfo returnVal;
	Token token = PeekNextToken();
	if( token.tp == RESWORD && (!strcmp(token.lx, "static" )) || (!strcmp(token.lx, "field" ))) {
		returnVal = classVarDeclar();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else if( token.tp == RESWORD && (!strcmp(token.lx, "constructor" )) || (!strcmp(token.lx, "function" )) || (!strcmp(token.lx, "method" ))){
		returnVal = subroutineDeclar();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else {
		returnVal.tk = token;
		returnVal.er = memberDeclarErr;
		return returnVal;
	}
	returnVal.er = none;
	return returnVal;
}

// classVarDeclar -> (static | field) type identifier {, identifier};
ParserInfo classVarDeclar(){
	ParserInfo returnVal;
	// Check for static or field
	Token token = GetNextToken();
	if( token.tp == RESWORD && !strcmp(token.lx, "static")){
		returnVal = type();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "field")){
		returnVal = type();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else{
		returnVal.tk = token;
		returnVal.er = classVarErr;
		return returnVal;
	}

	// Check for identifier
	token = GetNextToken();
	if(token.tp == ID){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = idExpected;
		return returnVal;
	}

	// Check for , identifier
	token = PeekNextToken();
	while(token.tp != EOFile && strcmp(token.lx,";")) {
		token = GetNextToken();
		if(token.tp == SYMBOL && !strcmp(token.lx, ",")){
			;
		}
		else{
			returnVal.tk = token;
			returnVal.er = classVarErr;
			return returnVal;
		}

		token = GetNextToken();
		if(token.tp == ID){
			;
		}
		else {
			returnVal.tk = token;
			returnVal.er = idExpected;
			return returnVal;
		}
	}
	returnVal.er = none;
	return returnVal;
	
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
	// Check for constrcutor | function | method
	Token token = GetNextToken();
	if(token.tp == RESWORD && !strcmp(token.lx, "constructor")){
		;
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "function")){
		;
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "method")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = subroutineDeclarErr;
		return returnVal;
	}

	// Check for type | void
	token = PeekNextToken();
	if(token.tp == RESWORD && (!strcmp(token.lx, "int")) || (!strcmp(token.lx, "char")) || (!strcmp(token.lx, "boolean"))){
		returnVal=type();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else if(token.tp == RESWORD && !strcmp(token.lx, "void")){
		token = GetNextToken();
	}
	else {
		returnVal.er = subroutineDeclarErr;
		returnVal.tk = token;
		return returnVal;
	}

	// Check for identifier
	token = GetNextToken();
	if(token.tp == ID){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = idExpected;
		return returnVal;
	}

	// Check for ( paramList )
	token = GetNextToken();
	if(token.tp == SYMBOL && !strcmp(token.lx, "(")){
		// Check for param list
		returnVal = paramList();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else {
		returnVal.tk = token;
		returnVal.er = openParenExpected;
		return returnVal;
	}

	// Check for closing paren
	if(token.tp == SYMBOL && !strcmp(token.lx, ")")){

		// Check for subroutine body
		returnVal = subroutineBody();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else {
		returnVal.tk = token;
		returnVal.er = closeParenExpected;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
}

// paramList -> type identifier {, type identifier} | ε
ParserInfo paramList(){
	ParserInfo returnVal;
	Token token = PeekNextToken();
	if(token.tp == SYMBOL && !strcmp(token.lx, ")")){
		returnVal.er = none;
		return returnVal;
	}
	else if(token.tp == ID || (token.tp == RESWORD && (!strcmp(token.lx, "int")) || (!strcmp(token.lx, "char")) || (!strcmp(token.lx, "boolean")))){
		// Handle type
		returnVal=type();
		if(returnVal.er != none){
			return returnVal;
		}

		// Handle identifier
		token = GetNextToken();
		if(token.tp == ID){
		;
		}
		else {
			returnVal.tk = token;
			returnVal.er = idExpected;
			return returnVal;
		}

		// zero or many
		token = PeekNextToken();
		while(token.tp == SYMBOL && !strcmp(token.lx, ",")){
			GetNextToken();
			returnVal=type();
			if(returnVal.er != none){
				return returnVal;
			}

			// Handle identifier
			token = GetNextToken();
			if(token.tp == ID){
				;
			}
			else {
				returnVal.tk = token;
				returnVal.er = idExpected;
				return returnVal;
			}

		}
	}
	returnVal.er = none;
	return returnVal;

}

//subroutineBody -> { {statement} } 
ParserInfo subroutineBody(){
	ParserInfo returnVal;
	// Check for open bracket
	Token token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "{")){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = openBraceExpected;
		return returnVal;
	}

	// Check for statement
	token = PeekNextToken();
	while(token.tp = RESWORD && (!strcmp(token.lx, "var") || !strcmp(token.lx, "let") || !strcmp(token.lx, "if") || !strcmp(token.lx, "while") || !strcmp(token.lx, "do") || !strcmp(token.lx, "return"))){
		returnVal = statement();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// Check for close bracket
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "}")){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = closeBraceExpected;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
}

//statement -> varDeclarStatement | letStatemnt | ifStatement | whileStatement | doStatement | returnStatemnt
ParserInfo statement(){
	ParserInfo returnVal;
	// check for statement
	Token token = PeekNextToken();
	if(token.tp = RESWORD && !strcmp(token.lx, "var")){
		returnVal = varDeclarstatement();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else if(token.tp = RESWORD && !strcmp(token.lx, "let")){
		returnVal = letStatement();
		if(returnVal.er != none){
			return returnVal;
		}

	}
	else if(token.tp = RESWORD && !strcmp(token.lx, "if")){
		returnVal = ifStatement();
		if(returnVal.er != none){
			return returnVal;
		}

	}
	else if(token.tp = RESWORD && !strcmp(token.lx, "while")){
		returnVal = whileStatement();
		if(returnVal.er != none){
			return returnVal;
		}

	}
	else if(token.tp = RESWORD && !strcmp(token.lx, "do")){
		returnVal = doStatement();
		if(returnVal.er != none){
			return returnVal;
		}

	}
	else if(token.tp = RESWORD && !strcmp(token.lx, "return")){
		returnVal = returnStatement();
		if(returnVal.er != none){
			return returnVal;
		}

	}
	else{
		returnVal.er = syntaxError;
		returnVal.tk = token;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
}

//varDeclarStatement -> var type identifier { , identifier } ;
ParserInfo varDeclarstatement(){
	ParserInfo returnVal;

	// Check for var
	Token token = GetNextToken();
	if(token.tp = RESWORD && !strcmp(token.lx, "var")){

		// Check for type
		returnVal = type();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else{
		returnVal.tk = token;
		returnVal.er = syntaxError;
		return returnVal;
	}

	// Handle identifier
	token = GetNextToken();
	if(token.tp == ID){
	;
	}
	else {
		returnVal.tk = token;
		returnVal.er = idExpected;
		return returnVal;
	}

	// zero or many
	token = PeekNextToken();
	while(token.tp == SYMBOL && !strcmp(token.lx, ",")){
		// Eat , token
		GetNextToken();

		// Handle identifier
		token = GetNextToken();
		if(token.tp == ID){
			;
		}
		else {
			returnVal.tk = token;
			returnVal.er = idExpected;
			return returnVal;
		}

	}

	// Handle semi colon
	token = GetNextToken();
	if(token.tp == SYMBOL && !strcmp(token.lx, ",")){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = semicolonExpected;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
	
}

//letStatemnt -> let identifier [ [ expression ] ] = expression ;
ParserInfo letStatement(){
	ParserInfo returnVal;

	// Check for let
	Token token = GetNextToken();
	if(token.tp = RESWORD && !strcmp(token.lx, "let")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = syntaxError;
		return returnVal;
	}

	// Handle identifier
	token = GetNextToken();
	if(token.tp == ID){
	;
	}
	else {
		returnVal.tk = token;
		returnVal.er = idExpected;
		return returnVal;
	}

	//zero or one
	token = PeekNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "[")){
		GetNextToken();
		returnVal = expression();
		if(returnVal.er != none){
			return returnVal;
		}

		token = GetNextToken();
		if(token.tp = SYMBOL && !strcmp(token.lx, "]")){
			;
		}
		else{
			returnVal.tk = token;
			returnVal.er = closeBracketExpected;
			return returnVal;
		}

	}

	// Handle equals
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "=")){
		returnVal = expression();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else {
		returnVal.tk = token;
		returnVal.er = equalExpected;
		return returnVal;
	}

	// handle semi colon
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, ";")){
		;
	}
	else {
		returnVal.tk = token;
		returnVal.er = semicolonExpected;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
}

//ifStatement -> if ( expression ) { {statement} } [else { {statement} }]
ParserInfo ifStatement(){
	ParserInfo returnVal;

	// Check for if
	Token token = GetNextToken();
	if(token.tp = RESWORD && !strcmp(token.lx, "if")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = syntaxError;
		return returnVal;
	}

	// Check for open bracket
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "(")){
		returnVal = expression();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else{
		returnVal.tk = token;
		returnVal.er = openParenExpected;
		return returnVal;
	}

	// Check for closed bracket
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, ")")){
		returnVal = expression();
	}
	else{
		returnVal.tk = token;
		returnVal.er = closeParenExpected;
		return returnVal;
	}

	// Check for open brace
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "{")){
		returnVal = expression();
	}
	else{
		returnVal.tk = token;
		returnVal.er = openBraceExpected;
		return returnVal;
	}

	// Zero or many
	// Check for statement
	token = PeekNextToken();
	while(token.tp = RESWORD && (!strcmp(token.lx, "var") || !strcmp(token.lx, "let") || !strcmp(token.lx, "if") || !strcmp(token.lx, "while") || !strcmp(token.lx, "do") || !strcmp(token.lx, "return"))){
		returnVal = statement();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// Check for close brace
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "}")){
		returnVal = expression();
	}
	else{
		returnVal.tk = token;
		returnVal.er = closeBraceExpected;
		return returnVal;
	}

	// [else { {statement} }]
	// zero or one
	token = PeekNextToken();
	// Check for else
	if(token.tp = RESWORD && !strcmp(token.lx, "else")){
		GetNextToken();
		// Check for open brace
		token = GetNextToken();
		if(token.tp = SYMBOL && !strcmp(token.lx, "{")){
			;
		}
		else {
			returnVal.tk = token;
			returnVal.er = openBraceExpected;
			return returnVal;
		}

		// Check for statement
		token = PeekNextToken();
		while(token.tp = RESWORD && (!strcmp(token.lx, "var") || !strcmp(token.lx, "let") || !strcmp(token.lx, "if") || !strcmp(token.lx, "while") || !strcmp(token.lx, "do") || !strcmp(token.lx, "return"))){
			returnVal = statement();
			if(returnVal.er != none){
				return returnVal;
			}
		}

		// Check for close brace
		token = GetNextToken();
		if(token.tp = SYMBOL && !strcmp(token.lx, "}")){
			;
		}
		else {
			returnVal.tk = token;
			returnVal.er = closeBraceExpected;
			return returnVal;
		}
	}

	returnVal.er = none;
	return returnVal;

}

//whileStatement -> while ( expression ) { {statement} } 
ParserInfo whileStatement(){
	ParserInfo returnVal;

	// Check for while
	Token token = GetNextToken();
	if(token.tp = RESWORD && !strcmp(token.lx, "while")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = syntaxError;
		return returnVal;
	}

	// Check for open paren
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "(")){
		returnVal = expression();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else{
		returnVal.tk = token;
		returnVal.er = openParenExpected;
		return returnVal;
	}

	// Check for close paren
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, ")")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = closeParenExpected;
		return returnVal;
	}

	// Check for open brace
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "{")){
		returnVal = expression();
	}
	else{
		returnVal.tk = token;
		returnVal.er = openBraceExpected;
		return returnVal;
	}

	// Zero or many
	// Check for statement
	token = PeekNextToken();
	while(token.tp = RESWORD && (!strcmp(token.lx, "var") || !strcmp(token.lx, "let") || !strcmp(token.lx, "if") || !strcmp(token.lx, "while") || !strcmp(token.lx, "do") || !strcmp(token.lx, "return"))){
		returnVal = statement();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// Check for close brace
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, "}")){
		returnVal = expression();
	}
	else{
		returnVal.tk = token;
		returnVal.er = closeBraceExpected;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
}

//doStatement -> do subroutineCall ; 
ParserInfo doStatement(){
	ParserInfo returnVal;

	// Check for do
	Token token = GetNextToken();
	if(token.tp = RESWORD && !strcmp(token.lx, "do")){
		returnVal = subroutineCall();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else{
		returnVal.tk = token;
		returnVal.er = syntaxError;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
}

//subroutineCall -> identifier [ . identifier ] ( expressionList )
ParserInfo subroutineCall(){
	ParserInfo returnVal;

	// Handle identifier
	Token token = GetNextToken();
	if(token.tp == ID){
	;
	}
	else {
		returnVal.tk = token;
		returnVal.er = idExpected;
		return returnVal;
	}

	// Zero or one
	token = PeekNextToken();
	if(token.tp == SYMBOL && !strcmp(token.lx, ".")){
		// Eat the . token
		GetNextToken();
		// Handle identifier
		token = GetNextToken();
		if(token.tp == ID){
		;
		}
		else {
			returnVal.tk = token;
			returnVal.er = idExpected;
			return returnVal;
		}
	}

	// Hanlde open paren
	token = GetNextToken();
	if(token.tp == SYMBOL && !strcmp(token.lx, "(")){
		returnVal = expressionList();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else{
		returnVal.tk = token;
		returnVal.er = openParenExpected;
		return returnVal;
	}

	// Hanlde close paren
	token = GetNextToken();
	if(token.tp == SYMBOL && !strcmp(token.lx, ")")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = closeParenExpected;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;
}

// expressionList -> expression { , expression } | ε
ParserInfo expressionList(){
	ParserInfo returnVal;
	Token token = PeekNextToken();
	// Check for close paren indicating ε
	if(token.tp == SYMBOL && !strcmp(token.lx, ")")){
		returnVal.er = none;
		return returnVal;
	}
	// check for expression
	else if( (token.tp == SYMBOL && (!strcmp(token.lx,"-")) || (!strcmp(token.lx,"~"))) || (token.tp == INT) || (token.tp == ID)){
		returnVal = expression();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// zero or many expression
	token = PeekNextToken();
	while(token.tp == SYMBOL && (!strcmp(token.lx, ","))){
		returnVal = expression();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	returnVal.er = none;
	return returnVal;
		
}

//returnStatemnt->  return [ expression ] ;
ParserInfo returnStatement(){
	ParserInfo returnVal;

	// Check for return
	Token token = GetNextToken();
	if(token.tp = RESWORD && !strcmp(token.lx, "return")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = syntaxError;
		return returnVal;
	}
	
	// zero or one expression
	token = PeekNextToken();
	if( (token.tp == SYMBOL && (!strcmp(token.lx,"-")) || (!strcmp(token.lx,"~"))) || (token.tp == INT) || (token.tp == ID)){
		returnVal = expression();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// check for semicolon
	token = GetNextToken();
	if(token.tp = SYMBOL && !strcmp(token.lx, ";")){
		;
	}
	else{
		returnVal.tk = token;
		returnVal.er = semicolonExpected;
		return returnVal;
	}

	returnVal.er = none;
	return returnVal;


}

//expression -> relationalExpression { ( & | | ) relationalExpression }
ParserInfo expression(){
	ParserInfo returnVal;

	// check for relationExpression
	Token token = PeekNextToken();
	if( (token.tp == SYMBOL && (!strcmp(token.lx,"-")) || (!strcmp(token.lx,"~"))) || token.tp == INT || token.tp == ID || token.tp == STRING || token.tp == SYMBOL && !strcmp(token.lx, "(") || token.tp == RESWORD && (!strcmp(token.lx, "true") || !strcmp(token.lx, "false") || !strcmp(token.lx, "null") || !strcmp(token.lx, "this"))){
		returnVal = relationalExpression();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// Zero or many
	// Check for & or |
	token = PeekNextToken();
	while(token.tp == SYMBOL && (!strcmp(token.lx,"&") || (!strcmp(token.lx,"|")))){
		GetNextToken();
		returnVal = relationalExpression();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	returnVal.er = none;
	return returnVal;
}

//relationalExpression -> ArithmeticExpression { ( = | > | < ) ArithmeticExpression } 
ParserInfo relationalExpression(){
	ParserInfo returnVal;

	//check for arithmeticExpression
	Token token = PeekNextToken();
	if( (token.tp == SYMBOL && (!strcmp(token.lx,"-")) || (!strcmp(token.lx,"~"))) || token.tp == INT || token.tp == ID || token.tp == STRING || token.tp == SYMBOL && !strcmp(token.lx, "(") || token.tp == RESWORD && (!strcmp(token.lx, "true") || !strcmp(token.lx, "false") || !strcmp(token.lx, "null") || !strcmp(token.lx, "this"))){
		returnVal = ArithmeticExpression();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// Zero or many
	// Check for = or > or <
	token = PeekNextToken();
	while(token.tp == SYMBOL && (!strcmp(token.lx,"=") || (!strcmp(token.lx,">")) || (!strcmp(token.lx,"<")))){
		GetNextToken();
		returnVal = ArithmeticExpression();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	returnVal.er = none;
	return returnVal;
}

//ArithmeticExpression -> term { ( + | - ) term }
ParserInfo ArithmeticExpression(){
	ParserInfo returnVal;

	// Check for term
	Token token = PeekNextToken();
	if( (token.tp == SYMBOL && (!strcmp(token.lx,"-")) || (!strcmp(token.lx,"~"))) || token.tp == INT || token.tp == ID || token.tp == STRING || token.tp == SYMBOL && !strcmp(token.lx, "(") || token.tp == RESWORD && (!strcmp(token.lx, "true") || !strcmp(token.lx, "false") || !strcmp(token.lx, "null") || !strcmp(token.lx, "this"))){
		returnVal = term();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// Zero or many
	// Check for + or - 
	token = PeekNextToken();
	while(token.tp == SYMBOL && (!strcmp(token.lx,"+") || (!strcmp(token.lx,"-")))){
		GetNextToken();
		returnVal = term();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	returnVal.er = none;
	return returnVal;
}

//term -> factor { ( * | / ) factor }
ParserInfo term(){
	ParserInfo returnVal; 

	// Check for term
	Token token = PeekNextToken();
	if( (token.tp == SYMBOL && (!strcmp(token.lx,"-")) || (!strcmp(token.lx,"~"))) || token.tp == INT || token.tp == ID || token.tp == STRING || token.tp == SYMBOL && !strcmp(token.lx, "(") || token.tp == RESWORD && (!strcmp(token.lx, "true") || !strcmp(token.lx, "false") || !strcmp(token.lx, "null") || !strcmp(token.lx, "this"))){
		returnVal = factor();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	// Zero or many
	// Check for * or /
	token = PeekNextToken();
	while(token.tp == SYMBOL && (!strcmp(token.lx,"*") || (!strcmp(token.lx,"/")))){
		GetNextToken();
		returnVal = factor();
		if(returnVal.er != none){
			return returnVal;
		}
	}

	returnVal.er = none;
	return returnVal;
}

//factor -> ( - | ~ | ε ) operand
ParserInfo factor(){
	ParserInfo returnVal;

	// Check for operand, intconst and id meaning ε
	Token token = PeekNextToken();
	if(token.tp == INT || token.tp == ID || token.tp == STRING || token.tp == SYMBOL && !strcmp(token.lx, "(") || token.tp == RESWORD && (!strcmp(token.lx, "true") || !strcmp(token.lx, "false") || !strcmp(token.lx, "null") || !strcmp(token.lx, "this"))){
		returnVal = operand();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	// check for - ~
	else if(token.tp == SYMBOL && (!strcmp(token.lx,"-") || (!strcmp(token.lx,"~")))){
		GetNextToken();
		returnVal = operand();
		if(returnVal.er != none){
			return returnVal;
		}
	}
	else {
		returnVal.tk = token;
		returnVal.er = syntaxError;
		return returnVal;
	}


}

//operand -> integerConstant | identifier [.identifier ] [ [ expression ] | (expressionList ) ] | (expression) | stringLiteral | true | false | null | this
ParserInfo operand(){
	ParserInfo returnVal;

	
	Token token = GetNextToken();
	// Check for integer constant
	if(token.tp == INT){
		;
	}
	// Check for identifier [.identifier ] [ [ expression ] | (expressionList ) ]
	else if(token.tp == ID){
		//zero or one .identifier
		token = PeekNextToken();
		if(token.tp == SYMBOL && !strcmp(token.lx, ".")){
			GetNextToken();
			token = GetNextToken();
			if(token.tp == ID){
				;
			}
			else{
				returnVal.er = idExpected;
				returnVal.tk = token;
				return returnVal;
			}
		}

		//zero or one [ [ expression ] | (expressionList ) ]
		token = PeekNextToken();
		if(token.tp == SYMBOL && !strcmp(token.lx, "[")){
			GetNextToken();
			returnVal = expression();
			if(returnVal.er != none){
				return returnVal;
			}

			token = GetNextToken();
			if(token.tp == SYMBOL && !strcmp(token.lx, "]")){
				;
			}
			else{
				returnVal.er = closeBracketExpected;
				returnVal.tk = token;
				return returnVal;
			}
		}
		else if(token.tp == SYMBOL && !strcmp(token.lx, "(")){
			GetNextToken();
			returnVal = expressionList();
			if(returnVal.er != none){
				return returnVal;
			}

			token = GetNextToken();
			if(token.tp == SYMBOL && !strcmp(token.lx, ")")){
				;
			}
			else{
				returnVal.er = closeParenExpected;
				returnVal.tk = token;
				return returnVal;
			}

		}
	}
	
	// Check for (expression)
	else if(token.tp == SYMBOL && !strcmp(token.lx, "(")){
		returnVal = expression();
		if(returnVal.er != none){
			return returnVal;
		}

		token = GetNextToken();
		if(token.tp == SYMBOL && !strcmp(token.lx, ")")){
			;
		}
		else{
			returnVal.er = closeParenExpected;
			returnVal.tk = token;
			return returnVal;
		}
	}

	// Check for stringLiteral 
	else if(token.tp == STRING){
		;
	}
	// Check for true
	else if(token.tp == RESWORD && !strcmp(token.lx, "true")){
		;
	}
	// Check for false
	else if(token.tp == RESWORD && !strcmp(token.lx, "false")){
		;
	}
	// Check for null
	else if(token.tp == RESWORD && !strcmp(token.lx, "null")){
		;
	}
	// Check for this
	else if(token.tp == RESWORD && !strcmp(token.lx, "this")){
		;
	}
	else{
		returnVal.er = syntaxError;
		returnVal.tk = token;
		return returnVal;
	}
	
	returnVal.er = none;
	return returnVal;
	
}


int InitParser (char* file_name)
{
	InitLexer(file_name);
	return 1;
}

ParserInfo Parse ()
{
	ParserInfo pi;

	pi = classDeclar();

	return pi;
}

int StopParser ()
{
	return 1;
}

#ifndef TEST_PARSER
int main ()
{
	printf("sdasdasdasdasdas");
	//InitParser("Ball.jack");
	//ParserInfo test = Parse();
	
	//printf("testing\n-------------\n%d",test.er);

	return 1;
}
#endif
