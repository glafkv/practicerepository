#include <iostream>
#include <stdlib.h>
#include <vector>
#include "semantics.h"
#include "token.h"
#include "node.h"

using namespace std;

static stack_t temp;
static int varScope = 0;
int stackIndex = 0;
int stackIndexCount = 0;
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
void checkVar(stack_t var)
{
	int varDefined = find(var);
	
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
		
		removeLocalVar(varScope);
		varScope--;
		
		targetFile << "STOP\n";
		
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
		varScope++;
	
		semantics(root->child1);
		semantics(root->child2);
		
		removeLocalVar(varScope);	
		varScope--;
	}
	if(root->label == "<vars>")
	{
		temp.token = root->tokens.front();
		temp.scope = varScope;
		root->tokens.erase(root->tokens.begin());
		
		if(temp.token.desc != "EMPTY")
		{
			checkVar(temp);
			stackIndexCount++;
			targetFile << "LOAD " << findStackLocation(temp) << "\n";
		//	targetFile << "STORE " << findStackLocation(temp) << "\n";
				//targetFile << "STACKW 0\n";
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
			int originalVar = (tempVar) - 1;
			int localVar = tempVar;
			tempVar++;
			
			semantics(root->child2);
			
			targetFile << "STORE TEMP" << localVar << "\n";
			//targetFile << "LOAD TEMP" << originalVar << "\n";
				
			semantics(root->child1);
			
			if(operatorMap[root->tokens.at(0).desc] == "PLUStk")
			{
				targetFile << "ADD TEMP" << localVar << "\n";
				//targetFile << "ADD TEMP" << originalVar <<"\n";

			}
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
			int originalVar = (tempVar) - 1;
			int localVar = tempVar;
			tempVar++;
			
			semantics(root->child2);
			
			targetFile << "STORE TEMP" << localVar << "\n";
			//targetFile << "LOAD TEMP" << originalVar << "\n";
			
			semantics(root->child1);
			if(operatorMap[root->tokens.at(0).desc] == "MINUStk")
			{
				targetFile << "SUB TEMP" << localVar << "\n";
			}
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
			int originalVar = (tempVar) - 1;
			int localVar = tempVar;
			tempVar++;
		
			semantics(root->child2);
			
			targetFile << "STORE TEMP" << localVar << "\n";
			//targetFile << "LOAD TEMP" << originalVar << "\n";
		
			semantics(root->child1);
			
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
			temp.token = root->tokens.front();
			temp.scope = varScope;
			
			if(temp.token.ID == IDtk)
			{
				compareScope(temp);
				//targetFile << "STACKR " << findStackLocation(temp) << "\n";
			}
			else if(temp.token.ID == INTtk)
			{
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
		temp.token = root->tokens.front();
		temp.scope = varScope;	
		
		int localVar = tempVar;
		tempVar++;
		
		compareScope(temp);
		targetFile << "READ TEMP" << localVar << "\n";
		targetFile << "LOAD TEMP" << localVar << "\n";
		/*if(temp.token.ID == IDtk)
		{
			targetFile << "STACKW " << findStackLocation(temp) << "\n";
		}*/
		return;
	}
	if(root->label == "<out>")
	{
		int localVar = tempVar;
		tempVar++;
		semantics(root->child1);
		targetFile << "STORE TEMP" << localVar << "\n";
		targetFile << "WRITE TEMP" << localVar << "\n";
	}
	if(root->label == "<if>")
	{
		int loop = ++repeatNum;
		int localVar = tempVar;
		tempVar++;
		
		semantics(root->child1);
		
		targetFile << "STORE TEMP" << localVar << "\n";
		
		semantics(root->child3);
		targetFile << "SUB TEMP" << localVar << "\n";
		
		semantics(root->child2);
		semantics(root->child4);
		
		targetFile << "label" << loop << ": NOOP\n";
		
		return;	
	}
	if(root->label == "<loop>")
	{
		int loop = ++repeatNum;	
		int localVar = tempVar;
		tempVar++;
	
		semantics(root->child1);
		targetFile << "STORE TEMP" << localVar << "\n";
		
		semantics(root->child3);
		targetFile << "SUB TEMP" << localVar << "\n";
			
		semantics(root->child2);
		semantics(root->child4);
		
		targetFile << "BR loop" << loop << "\n";
		targetFile << "label" << loop << ": NOOP\n";
	}
	if(root->label == "<assign>")
	{
		temp.token = root->tokens.front();
		temp.scope = varScope;
		
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
