#include <iostream>
#include <stdlib.h>
#include <vector>
#include "semantics.h"
#include "token.h"
#include "node.h"

using namespace std;
//to test individual variables
static stack_t temp;
//scope of variables
static int varScope = 0;
//current location of stack
int stackIndex = 0;
//number of variables on stack
int stackIndexCount = 0;
//temp variables for local scope
static int repeatNum = 0;
static int tempVar = 0;

vector<stack_t> stack;

void printStack()
{
	for(int i = 0; i < stack.size(); i++)
	{
		cout << "\tIndex " << i << " = " << stack.at(i).token.desc << " Current scope: " << stack.at(i).scope << "\n";
	}
}
//when a variable is defined, check if it has been defined yet
void checkVar(stack_t var)
{
	int varDefined = find(var);
	//if variable is on the stack, it will throw an error. if not, it will push the variable onto the stack
	if(varDefined > 0)
	{
		cout << "Semantics Error: " << varDefined << " on line: " << var.token.lineNumber << "has already been defined in the current scope at: " << stack.at(varDefined).token.lineNumber << "\n";
	
		exit(EXIT_FAILURE);
	}
	else
	{
		stack.push_back(var);
		stackIndex++;
	}
}
//search the stack to see if this variable had been defined 
int checkVarExist(stack_t var)
{
	for(int i = 0; i < stack.size(); i++)
	{
		if(stack.at(i).token.desc == var.token.desc)
		{
			return i;
		}
	}
	return -1;
}
//find the variable in thes tack
int find(stack_t var)
{
	for(int i = 0; i < stack.size(); i++)
	{
		if((stack.at(i).token.desc == var.token.desc) && (stack.at(i).scope == var.scope))
		{
			return i;
		}
	}
	return -1;
}
//find the location of the identifier
int findStackLocation(stack_t var)
{
	for(int i = 0; i < stack.size(); i++)
	{
		if((stack.at(i).token.desc == var.token.desc) && (stack.at(i).scope <= var.scope))
		{
			return i;
		}
	}
	cout << "ID: " << var.token.desc << " was not declared in current scope.\n";
	exit(EXIT_FAILURE);
}
//ensure the variable can be accessed in this scope
int compareScope(stack_t var)
{
	int varLocation = checkVarExist(var);
	if(varLocation >= 0)
	{
		if(stack.at(varLocation).scope > var.scope)
		{
			cout << "Semantics Error: " << getTokenDesc(var.token) << " on line " << var.token.lineNumber << " cannot be accessed in current scope\n";
			exit(EXIT_FAILURE);
		}
		else
			return 1;
	}
	else
	{
		cout << "Semantics error: " << getTokenDesc(var.token) << " on line " << var.token.lineNumber << " is not found or cannot be accessed in current scope\n";
		exit(EXIT_FAILURE);
	}
}
//remove the variables within the scope when leaving
void removeLocalVar(int scope)
{
	if(scope > 0)
	{
		for(int i = 0; i < stack.size(); i++)
		{
			if(stack.at(i).scope == scope)
			{
				targetFile << "POP\n";
				stack.erase(stack.begin() + i);
			}
		}
	}
	else if(scope == 0)
	{
		int stackSize = stack.size();
		stackSize--;
		for(stackSize; stackSize > 0; stackSize--)
		{
			if(stack.at(stackSize).scope == scope)
			{
				targetFile << "POP\n";
				stack.erase(stack.begin() + stackSize);
			}
		}
	}
}


void semantics(node_t *root)
{
	if(root==NULL)
		return;
	
	if(root->label == "<program>")
	{
		semantics(root->child1);
		semantics(root->child2);
		
		targetFile << "STOP\n";
		//pop the rest of the variables from the stack
		removeLocalVar(varScope);
		varScope--;
		
		
		//initialize the temporary variables to 0
		while(tempVar > 0)
		{
			targetFile << "TEMP" << (tempVar - 1) << " 0\n";
			tempVar--;
		}
		cout << "Semantics OK\n";
		return;
	}
	if(root->label == "<block>")
	{
		//increment the scope for the variables defined in this block
		varScope++;
	
		semantics(root->child1);
		semantics(root->child2);
		
		//when leaving, remove all local variables
		removeLocalVar(varScope);	
		varScope--;
	}
	if(root->label == "<vars>")
	{
		//check if the token is empty or not
		temp.token = root->tokens.front();
		temp.scope = varScope;
		root->tokens.erase(root->tokens.begin());
		
		if(temp.token.desc != "EMPTY")
		{
			
			//see if it has been declared or not
			checkVar(temp);
			//increment the stack
			stackIndexCount++;
			
			//load into accumulator
			targetFile << "LOAD " << findStackLocation(temp) << "\n";
			
			//process child		
			semantics(root->child1);
		}
	}
	if(root->label == "<expr>")
	{
		if(root->tokens.empty())
		{
			semantics(root->child1);
			
		}
		else
		{
			//get the number for the temp variable, and increment the number of temp variables
			int localVar = tempVar;
			tempVar++;
			
			semantics(root->child2);
			//give the temp variable the value in the accumulator
			targetFile << "STORE TEMP" << localVar << "\n";
		
			semantics(root->child1);
			
			
			targetFile << "ADD TEMP" << localVar << "\n";
				
			return;
		}
	}
	if(root->label == "<A>")
	{
		if(root->tokens.empty())
		{
			semantics(root->child1);
		
		}
		else
		{
			//get the number for temp vars and increment
			int localVar = tempVar;
			tempVar++;
			
			semantics(root->child2);
			//give the temp var the value in the accumulator
			targetFile << "STORE TEMP" << localVar << "\n";
						
			semantics(root->child1);
			
			targetFile << "SUB TEMP" << localVar << "\n";
			
			return;
		}
	}
	if(root->label == "<N>")
	{
		if(root->tokens.empty())
		{
			semantics(root->child1);
		}
		else
		{
			
			int localVar = tempVar;
			tempVar++;
		
			semantics(root->child2);
			
			targetFile << "STORE TEMP" << localVar << "\n";
			
		
			semantics(root->child1);
			//check if its a / or *
			if(operatorMap[root->tokens.at(0).desc] == "SLASHtk")
			{
				targetFile << "DIV TEMP" << localVar << "\n";
			}
			else if(operatorMap[root->tokens.at(0).desc] == "ASTERIKtk")
			{
				targetFile << "MULT TEMP" << localVar << "\n";
			}
			return;
		}
	}
	if(root->label == "<M>")
	{
		semantics(root->child1);
		if(!root->tokens.empty())
		{
			targetFile << "MULT -1\n";
		}
		return;
	}
	if(root->label == "<R>")
	{
		if(root->tokens.empty())
		{
			semantics(root->child1);
		}
		else
		{
			//if an ID was used, verify it was defined and can be accessed
			temp.token = root->tokens.front();
			temp.scope = varScope;
			
			if(temp.token.ID == IDtk)
			{
				//check if this variable is accessable
				compareScope(temp);
			}
			else if(temp.token.ID == INTtk)
			{
				//load the value of the integer into the accumulator
				targetFile << "LOAD " << getTokenInt(temp.token) << "\n";
			}
			return;
		}
	}
	if(root->label == "<stats>")
	{
		semantics(root->child1);
		semantics(root->child2);
	}
	if(root->label == "<stat>")
	{
		semantics(root->child1);
	}
	if(root->label == "<mStat>")
	{
		if(root->tokens.empty())
		{
			semantics(root->child1);
			semantics(root->child2);
		}
	}
	if(root->label == "<in>")
	{
		//verify ID exists
		temp.token = root->tokens.front();
		temp.scope = varScope;	
		
		int localVar = tempVar;
		tempVar++;
		//check if variable was defined
		compareScope(temp);
		
		targetFile << "READ TEMP" << localVar << "\n";
		targetFile << "LOAD TEMP" << localVar << "\n";
		
		return;
	}
	if(root->label == "<out>")
	{
		int localVar = tempVar;
		tempVar++;
		semantics(root->child1);
		//assign the temp var to the value in the accumulator then write 
		targetFile << "STORE TEMP" << localVar << "\n";
		targetFile << "WRITE TEMP" << localVar << "\n";
		return;
	}
	if(root->label == "<if>")
	{
		int loop = ++repeatNum;
		int localVar = tempVar;
		tempVar++;
		
		//declare a variable named loop that will execute the following instruction
		targetFile << "loop" << loop << ": ";
		
		semantics(root->child3);
		//assign the value to the value in accumulator
		targetFile << "STORE TEMP" << localVar << "\n";
		
		semantics(root->child1);
		//subtract the accumulator from temp var
		targetFile << "SUB TEMP" << localVar << "\n";
		
		semantics(root->child2);
		semantics(root->child4);
		
		//this will execute NOOP when it is branched to
		targetFile << "branch" << loop << ": NOOP\n";
		
		return;	
	}
	if(root->label == "<loop>")
	{
		int loop = ++repeatNum;	
		int localVar = tempVar;
		tempVar++;
	
		targetFile << "loop" << localVar << ": ";
		semantics(root->child3);
		targetFile << "STORE TEMP" << localVar << "\n";
		
		semantics(root->child1);
		targetFile << "SUB TEMP" << localVar << "\n";
			
		semantics(root->child2);
		semantics(root->child4);
		
		targetFile << "BR loop" << loop << "\n";
		targetFile << "branch" << loop << ": NOOP\n";
	}
	if(root->label == "<assign>")
	{
		//check if ID has been defined
		temp.token = root->tokens.front();
		temp.scope = varScope;
		//check to see if var may be accessed
		compareScope(temp);
	
		stackIndexCount++;
		semantics(root->child1);
		return;
	}
	if(root->label == "<RO>")
	{
		temp.token = root->tokens.front();
		temp.scope = varScope;
		root->tokens.erase(root->tokens.begin());
		
		if(operatorMap[temp.token.desc] == "LESSTHANtk")
		{
			//check if root node's tokens are empty
			if(!root->tokens.empty())
			{
				stack_t temp2;
				temp2.token = root->tokens.front();
				temp2.scope = varScope;
				root->tokens.erase(root->tokens.begin());
			
				if(operatorMap[temp2.token.desc] == "LESSTHANtk")
				{
					targetFile << "BRNEG label" << repeatNum << "\n";
					return;
				}
				else if(operatorMap[temp2.token.desc] == "GREATERTHANtk")
				{
					targetFile << "BRZERO label" << repeatNum << "\n";
					return;			
				}
			}
			targetFile << "BRZNEG label" << repeatNum << "\n";
			return;
		}
		else if(operatorMap[temp.token.desc] == "GREATERTHANtk")
		{
			if(!root->tokens.empty())
			{
				stack_t temp2;
				temp2.token = root->tokens.front();
				temp2.scope = varScope;
				root->tokens.erase(root->tokens.begin());
				
				if(operatorMap[temp2.token.desc] == "GREATERTHANtk")
				{
					targetFile << "BRPOS label" << repeatNum << "\n";
					return;
				}
			}
			targetFile << "BRZPOS label" << repeatNum << "\n";
			return;
		}
		else if(operatorMap[temp.token.desc] == "EQUALStk")
		{
			targetFile << "BRPOS label" << repeatNum << "\n";
			targetFile << "BRNEG label" << repeatNum << "\n";
			return;
		}
	}
}

