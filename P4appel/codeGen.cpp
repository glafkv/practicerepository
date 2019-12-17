#include "semantics.h"
#include "token.h"
#include "node.h"
#include "codeGen.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;

static int varScope = 0;
static int tempVars = 0;
static int numRepeat = 0;

static stack_t temp;
vector<stack_t> stack;

int stackLoc = 0;
int stackVars = 0;

void codeGen(node_t *root)
{
	if(root == NULL)
	{
		return;
	}
	if(root->label == "<program>")
	{
		codeGen(root->child1);
		codeGen(root->child2);
	
		targetFile << "STOP\n";
		removeLocalVars(varScope);
		varScope--;
		cout << "Semantics OK\n";
		while(tempVars > 0)
		{
			targetFile << "TEMP" << (tempVars - 1) << " 0\n";
			tempVars--;
		}
		return;
	}
	else if(root->label == "<block>")
	{
		varScope++;
		codeGen(root->child1);
		codeGen(root->child2);
		
		removeLocalVars(varScope);
		varScope--;
		return;
	}
	else if(root->label == "<vars>")
	{
		temp.token = root->tokens.front();
		temp.scope = varScope;
		root->tokens.erase(root->tokens.begin());
		
		if(temp.token.desc != "EMPTY")
		{
			checkVar(temp);
			stackVars++;
			targetFile << "LOAD TEMP" << getTokenInt(temp.token) << "\n";
			targetFile << "STORE TEMP" << getTokenInt(temp.token) << "\n";
			codeGen(root->child1);
			return;
		}
	}
	else if(root->label == "<expr>")
	{
		if(root->tokens.empty())
		{
			codeGen(root->child1);
			return;
		}
		else
		{
			int localVar = tempVars;
			tempVars++;
			codeGen(root->child2);
		
			targetFile << "STORE TEMP" << localVar << "\n";
			codeGen(root->child1);
			targetFile << "ADD TEMP" << localVar << "\n";
			return;
		}
		
	}
	else if(root->label == "<A>")
	{
		if(root->tokens.empty())
		{
			codeGen(root->child1);
			return;
		}
		else 
		{
			int localVar = tempVars;
			tempVars++;
			codeGen(root->child2);
			targetFile << "STORE TEMP" << localVar << "\n";
			codeGen(root->child1);
			targetFile << "SUB TEMP" << localVar << "\n";
			return;
		}
	}
	else if(root->label == "<N>")
	{
		if(root->tokens.empty())
		{
			codeGen(root->child1);
			return;
		}
		else
		{
			int localVar = tempVars++;
			tempVars++;
			codeGen(root->child2);
			targetFile << "STORE TEMP" << localVar << "\n";
			codeGen(root->child1);
			if(operatorMap[root->tokens.at(0).desc] == "SLASHtk")
			{
				targetFile << "DIV TEMP" << localVar << "\n";
			}
			else
			{
				targetFile << "MULT TEMP" << localVar << "\n";
			}
			return;
		}
	}
	else if(root->label == "<M>")
	{
		if(root->tokens.empty())
		{
			codeGen(root->child1);
			return;
		}
		else
		{
			codeGen(root->child1);
			targetFile << "MULT -1\n";
			return;
		}
	}
	else if(root->label == "<R>")
	{
		if(operatorMap[root->tokens.at(0).desc] == "LEFTBRACKETtk")
		{
			codeGen(root->child1);
			return;
		}
		else
		{
			temp.token = root->tokens.front();
			temp.scope = varScope;
			targetFile << "LOAD " << getTokenInt(temp.token) << "\n";
			return;
		}
	}
	else if(root->label == "<stats>")
	{
		codeGen(root->child1);
		codeGen(root->child2);
	}
	else if(root->label == "<mStat>")
	{
		if(root->tokens.empty())
		{
			codeGen(root->child1);
			codeGen(root->child2);
			return;
		}
		return;
	}
	else if(root->label == "<stat>")
	{
		codeGen(root->child1);
		return;
	}
	else if(root->label == "<in>")
	{
		temp.token = root->tokens.front();
		temp.scope = varScope;
		int localVar = tempVars;
		tempVars++;
		targetFile << "READ " << findOnStack(temp) << "\n";
		return;
	}
	else if(root->label == "<out>")
	{
		int localVar = tempVars;
		tempVars++;
		
		codeGen(root->child1);
		targetFile << "STORE TEMP" << localVar << "\n";
		targetFile << "WRITE TEMP" << localVar << "\n";
		return;
	}
	else if(root->label == "<if>")
	{
		int loop = ++numRepeat;
		int localVar = tempVars;
		tempVars++;
		
		targetFile << "loop" << loop << ": ";
		codeGen(root->child3);
		targetFile << "STORE TEMP" << localVar << "\n";
		
		codeGen(root->child1);
		targetFile << "SUB TEMP" << localVar << "\n";
		
		codeGen(root->child2);
		codeGen(root->child4);
		
		targetFile << "branch" << loop << ": NOOP\n";
		
		return;
	}
	else if(root->label == "<loop>")
	{
		int loop = ++numRepeat;
		int localVar = tempVars;
		tempVars++;
	
		targetFile << "loop" << localVar << ": ";
		codeGen(root->child3);
		targetFile << "STORE TEMP" << localVar << "\n";
		codeGen(root->child1);
		targetFile << "SUB TEMP" << localVar << "\n";
		
		codeGen(root->child2);
		codeGen(root->child4);
		targetFile << "BR loop" << loop << "\n";	
		targetFile << "branch" << loop << ": NOOP\n";
		return;
	}
	else if(root->label == "<assign>")
	{
		temp.token = root->tokens.front();
		temp.scope = varScope;
		codeGen(root->child1);
		temp.token = root->tokens.front();
		temp.scope = varScope;
		targetFile << "LOAD " << findOnStack(temp) << "\n";
		return;
	}
	else if(root->label == "<RO>")
	{
		temp.token = root->tokens.front();
		temp.scope = varScope;
		root->tokens.erase(root->tokens.begin());
		
		if(operatorMap[temp.token.desc] == "LESSTHANtk")
		{
			if(!root->tokens.empty())
			{
				stack_t temp2;
				temp2.token = root->tokens.front();
				temp2.scope = varScope;
				root->tokens.erase(root->tokens.begin());
				
				if(operatorMap[temp2.token.desc] == "LESSTHANtk")
				{
					targetFile << "BRNEG branch" << numRepeat << "\n";
					return;
				}
				else if(operatorMap[temp2.token.desc] == "GREATERTHANtk")
				{
					targetFile << "BRZERO branch" << numRepeat << "\n";
					return;
				}
			}
			targetFile << "BRZNEG branch" << numRepeat << "\n";
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
					targetFile << "BRPOS branch" << numRepeat << "\n";
					return;
				}
			}
			targetFile << "BRZPOS branch" << numRepeat << "\n";
			return;
		}
		else if(operatorMap[temp.token.desc] == "EQUALStk")
		{
			if(!root->tokens.empty())
			{
				stack_t temp2;
				temp2.token = root->tokens.front();
				temp2.scope = varScope;
				root->tokens.erase(root->tokens.begin());
			}
			targetFile << "BRPOS branch" << numRepeat << "\n";
			targetFile << "BRNEG branch" << numRepeat << "\n";
			return;
		}
	}
}
void checkVar(stack_t var)
{
	int varDefined = find(var);
	if(varDefined > 0)
	{
		cout << "Semantics error\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		stack.push_back(var);
		stackLoc++;
	}
}
int checkVarExists(stack_t var)
{
	for(int counter = 0; counter < stack.size(); counter++)
	{
		if(stack.at(counter).token.desc == var.token.desc)
			return counter;
	}
	return -1;
}
int find(stack_t var)
{
	for(int counter = 0; counter < stack.size(); counter++)
	{
		if((stack.at(counter).token.desc == var.token.desc) && (stack.at(counter).scope == var.scope))
		{
			return counter;
		}
	}
	return -1;
}
int findOnStack(stack_t var)
{
	for(int counter = 0; counter < stack.size(); counter++)
	{
		if((stack.at(counter).token.desc == var.token.desc) && (stack.at(counter).scope <= var.scope))
			return counter;
	
	}
	cout << "The identifier " << var.token.desc << " was not declared in this scope.\n";
		exit(EXIT_FAILURE);
}
int compareScope(stack_t var)
{
	int varLoc = checkVarExists(var);
		
	if(varLoc >= 0)
	{
		if(stack.at(varLoc).scope > var.scope)
		{
			cout << "Error\n";
			exit(EXIT_FAILURE);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		cout << "Error\n";
		exit(EXIT_FAILURE);
	}
}
void removeLocalVars(int scope)
{
	if(scope > 0)
	{
		for(int counter = 0; counter < stack.size(); counter++)
		{
			if(stack.at(counter).scope == scope)
			{
				stack.erase(stack.begin() + counter);
			}
		}
	}
	else if(scope == 0)
	{
		int counter = stack.size();
		counter--;
		for(counter; counter >= 0; counter--)
		{
			if(stack.at(counter).scope == scope)
			{
				stack.erase(stack.begin() + counter);
			}
		}
	}
}
void printStack()
{
	for(int i = 0; i < stack.size(); i++)
	{
		cout << "\tIndex " << i << " = " << stack.at(i).token.desc << ", scope is " << stack.at(i).scope << "\n";
	}
}
			
