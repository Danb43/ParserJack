/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Daniel Bradwell
Student ID: 201635396
Email: sc22d2b@leeds.ac.uk
Date Work Commenced: 8/2/2024
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

// cc -std=c99 lexer.h lexer.c -o lexer


// Global Variables
char *fileArray;
int currentCharacter;
long sizeOfArray;
char *fl;
int lineNumber;




// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{
  // Open File
  FILE *fp;
  fp = fopen(file_name, "r");
  // Check if file can be opened
  if(fp == NULL){
    // Return 0 as error occured
    return 0;
  }

  // Find file size
  fseek(fp, 0, SEEK_END);
  sizeOfArray = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // Read file into Array
  // Allocate array based on file size
  fileArray = (char *)malloc(sizeOfArray * sizeof(char));
  if(fileArray == NULL){
    // Return 0 as error occured
    return 0;
  }

  // Read file into array
  fread(fileArray, sizeof(char), sizeOfArray, fp);

  // Set up a varaible for tracking where in the input we are
  currentCharacter = 0;

  // Store file name in a global pointer
  fl = file_name;

  // Initialise the line count to be 1 
  lineNumber=1;

  // Return 1 as not errors caused earlier returns meaning file was read
  return 1;
}

// Get the next token from the source file
Token GetNextToken ()
{
  // Skip any white space characters, untill hit first non-whitepsace character
  // if C is the start symbol of a comment skipp characters in body until another non whitespace character
  // if C is end of file, return EOF token
  // if C is a quote, it may be the start of a literal string
  // if c is a letter it may be the first letter in a keyword or identifier 
  // if c is a digit it may be the first digit in a number then
  // not above, C must be a symbol


  // Variables
  char character;
  char *string;
  int stringLength;
  char keywords[21][12] = {"class", "constructor", "method", "function", "int", "boolean", "char", "void", "var", "static", "field", "let", "do", "if", "else", "while", "return", "true", "false", "null", "this"};
  char symbols[] = {'(', ')', '[', ']', '{', '}', ',', ';', '=', '.', '+', '-', '*', '/', '&', '|', '~', '<', '>'};
  int i;
  int multiLineComment;
  Token t;
  strcpy(t.fl, fl);
  

  // Loop until a token has been registered, 0-False 1-True
  int tokenFound = 0;
  while(tokenFound == 0){

    // Get the next character
    character = fileArray[currentCharacter];

    // Increment line number
    if (character == '\n'){
      currentCharacter++;
      lineNumber++;
    }

    // Skip any white space charatcers
    else if (isspace(character)){
      currentCharacter++;
    }

    // Check if comment
    else if (character == '/'){
      currentCharacter++;
      character = fileArray[currentCharacter];

      // Single line comment
      if (character == '/'){
        currentCharacter++;
        // Loop until end of sinle line comment 
        while( (character = fileArray[currentCharacter]) != '\n'){
          currentCharacter++;
        }
      }

      // Multi line comment
      else if (character == '*'){
        currentCharacter++;

        // Loop until end of multi line comment
        multiLineComment = 1;
        while(multiLineComment == 1) {

          // Checks if character is a star
          if((character = fileArray[currentCharacter]) == '*'){
            currentCharacter++;

            // Followed by a / to indicate the end of a comment
            if((character = fileArray[currentCharacter]) == '/'){
              currentCharacter++;
              multiLineComment = 0;
            } 
          }

          else {

            // Checks if file ends without closing comment indicating an error
            if(currentCharacter == sizeOfArray){
              t.ec = EofInCom;
              t.tp = ERR;
              t.ln = lineNumber;
              strcpy(t.lx, "Error: unexpected eof in comment");
              return t;
            }

            // Increment line count at new line
            else if(character == '\n'){
              lineNumber++;
            }
            currentCharacter++;
          }      
        }
      }
    
      else{
        currentCharacter--;
        character = fileArray[currentCharacter];
        t.tp = SYMBOL;
        t.ln = lineNumber;
        string = (char *)malloc(sizeof(char));
        string[0] = character;
        strcpy(t.lx, string);
        currentCharacter++;
        return t;
      }
    }

    // End of file
    else if (currentCharacter == sizeOfArray)
    {
      tokenFound = 1;
      t.tp = EOFile;
      strcpy(t.lx, "End of File");
      t.ln = lineNumber;
      return t;
    }

    // String
    else if (character == '"'){

      // Track length of string
      currentCharacter++;
      stringLength=0;

      // Loop until end of string indicated by closing quotes
      while((character=fileArray[currentCharacter]) != '"'){

        // Checks for eof within string
        if(currentCharacter == sizeOfArray){
          t.tp = ERR;
          strcpy(t.lx, "Error: unexpected eof in string constant");
          t.ec = NewLnInStr;
          t.ln = lineNumber;
          return t;
        }

        // Checks for new line within string
        if(character == '\n'){
          t.tp = ERR;
          strcpy(t.lx, "Error: new line in string constant");
          t.ec = EofInStr;
          t.ln = lineNumber;
          return t;
        }
        currentCharacter++;
        stringLength++;
      }

      // Create string array the size of the string read
      string = (char *)malloc(stringLength * sizeof(char));

      // Store the string read into created array
      for(i = 0; i < stringLength; i++){
        string[i] = fileArray[currentCharacter-(stringLength-i)];
      }

      // Set up token for the string
      t.ln = lineNumber;
      strcpy(t.lx, string);
      t.tp = STRING;
      tokenFound = 1;
      currentCharacter++;
      return t;
    }

    // letter, keyword or identfier 
    else if ((character >= 97 && character <= 122) || (character >= 65 && character <= 90) || (character == 95)){
      // Track length of string
      currentCharacter++;
      character=fileArray[currentCharacter];
      stringLength=1;

      // Keep reading more letters/digits until a character that is not
      while((character >= 97 && character <= 122) || (character >= 65 && character <= 90) || (character >= 48 && character <= 57) || (character == 95)){
        currentCharacter++;
        character=fileArray[currentCharacter];
        stringLength++;
      }

      // Create string array the size of the string read
      string = (char *)malloc(stringLength * sizeof(char));

      // Store the string read into created array
      for(i = 0; i < stringLength; i++){
        string[i] = fileArray[currentCharacter-(stringLength-i)];
      }

      // Compare string to list of keywords
      for(i = 0; i < 21; i++){
        // If found its a keyword
        if(strcmp(string, keywords[i]) == 0){
          t.tp = RESWORD;
          t.ln = lineNumber;
          strcpy(t.lx, string);
          return t;
        }  
      }
      // Else its an identifier  
      t.tp = ID;
      t.ln = lineNumber;
      strcpy(t.lx, string);
      return t;
      }

    // number
    else if ((character >= 48 && character <= 57)){

      // Track length of string
      currentCharacter++;
      character=fileArray[currentCharacter];
      stringLength=1;

      // keep reading in numbers
      while((character >= 48 && character <= 57)) {
        currentCharacter++;
        character=fileArray[currentCharacter];
        stringLength++;
      }

      // Create string array the size of the string read
      string = (char *)malloc(stringLength * sizeof(char));

      // Store the string read into created array
      for(i = 0; i < stringLength; i++){
        string[i] = fileArray[currentCharacter-(stringLength-i)];
      }

      t.tp = INT;
      t.ln = lineNumber;
      strcpy(t.lx, string);
      return t;

    }

    // symbol
    else{
      for(i = 0; i < 19; i++){
        if(character == symbols[i]){
          t.tp = SYMBOL;
          t.ln = lineNumber;
          string = (char *)malloc(sizeof(char));
          string[0] = character;
          strcpy(t.lx, string);
          currentCharacter++;
          return t;
        }
      }
      t.tp = ERR;
      t.ec = IllSym;
      strcpy(t.lx, "Error: illegal symbol in source file");
      t.ln = lineNumber;
      return t;
    }
    
    }
  }

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  int previousCurrentCharacter = currentCharacter;
  int previousLineNumber = lineNumber;
  Token t = GetNextToken();
  currentCharacter = previousCurrentCharacter;
  lineNumber = previousLineNumber;
  return t;
  
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
  // Free the dynamically allocated memory for fileArray
  //free(fileArray);

	return 0;
}

// do not remove the next line
#ifndef TEST
// int main ()
// {
// 	// implement your main function here
//   // NOTE: the autograder will not use your main function

//   int test = InitLexer("Main.jack");
//   Token testt = GetNextToken();
//   printf("< %s, %d, %s, %d >\n", testt.fl, testt.ln, testt.lx, testt.tp);
//   // testt = GetNextToken();
//   // printf("< %s, %d, %s, %d >\n", testt.fl, testt.ln, testt.lx, testt.tp);
//   //testt = GetNextToken();
//   //printf("< %s, %d, %s, %d >\n", testt.fl, testt.ln, testt.lx, testt.tp);
//   //testt = GetNextToken();
//   //printf("< %s, %d, %s, %d >\n", testt.fl, testt.ln, testt.lx, testt.tp);
  
//   for(int i=0; i<101; i++){
//     testt = GetNextToken();
//     printf("< %s, %d, %s, %d >\n", testt.fl, testt.ln, testt.lx, testt.tp);
//   }

// 	return 0;
// }

#endif
