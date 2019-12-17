#include <iostream>
#include <stdlib.h>
#include <vector>
#include "semantics.h"
#include "node.h"

static stack_t temp;
static int varScope = 0;
int stackIndex = 0;
int stackIndexCount = 0;
static int repeatNum = 0;
static int tempVar = 0;

void printStack()
{
	for(int i = 0; i < stack.size(); i++)
	{
		cout < "\tIndex " << 
