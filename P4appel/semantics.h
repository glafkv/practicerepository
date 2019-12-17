#ifndef SEMANTICS_H
#define SEMANTICS_H
#include <vector>
#include <iostream>
#include <fstream>
#include "token.h"
#include "node.h"

using namespace std;

//this is the target file/generated executable
extern ofstream targetFile;

//define a variable on a stack
struct stack_t
{
	Token token;
	int scope;
	int lineNumber;
};

//stack that holds variables
extern vector<stack_t> stack;

//functions
/*void semantics(node_t*);
void checkVar(stack_t);
int checkVarExists(stack_t);
int find(stack_t);
int findOnStack(stack_t);
int compareScope(stack_t);
void removeLocalVars(int);
void printStack();
*/
int checkVarExist(stack_t);
int find(stack_t);
int findStackLocation(stack_t);
int compareScope(stack_t);
void checkVar(stack_t);
void removeLocalVar(int);
void printStack();
void semantics(node_t*);
	
const int MAX_VARS = 100;
#endif
