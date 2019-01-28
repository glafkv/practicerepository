/*********************************************************************/
/* Queue.c                                                           */
/*                                                                   */
/* Queue demonstration package.  Queue implementes as a linked ADT.  */
/* Author: Dr. Rick Coleman, Instructor CS207                        */
/* Date: February, 1998                                              */
/*********************************************************************/
#include "QueueTypes.h"
#include <stdio.h>
#include <stdlib.h>

/**********************************/
/* InitializeQueue()              */
/*                                */
/* Init a new empty Queue data    */
/*   structure.                   */
/**********************************/
void InitializeQueue(Queue *Q)
{
	Q->Count = 0;
	Q->Head = NULL;
	Q->Tail = NULL;
}

/**********************************/
/* EmptyQueue()                   */
/*                                */
/* Return TRUE if Queue is empty. */
/**********************************/
int EmptyQueue(Queue *Q)
{
	return(Q->Count == 0);
	/* or return(Q->Head == NULL); */
}

/**********************************/
/* Enqueue()                      */
/*                                */
/* Add a node to the queue.       */
/**********************************/
void Enqueue(ItemType *X, Queue *Q)
{
	if(Q->Head == NULL) /* Insert as first node in queue */
		Q->Head = Q->Tail = X;
	else  /* Insert at end of queue */
	{
		(Q->Tail)->next = X;
		Q->Tail = X;
	}
	Q->Count++;
}

/**********************************/
/* Dequeue()                      */
/*                                */
/* Remove a node from the queue.  */
/**********************************/
int Dequeue(Queue *Q, ItemType *X)
{
	Customer *temp;

	if(!EmptyQueue(Q))
	{
		temp = Q->Head;		/* Save to free the removed node */
		*X = *(Q->Head);	/* Copy entire structure */
		Q->Head = (Q->Head)->next;
		Q->Count--;
		if(Q->Head == NULL) Q->Tail = NULL;	/* Fix tail if Dequeuing last node */
		free(temp);
		return TRUE;
	}
	else
		return FALSE;	/* Flag nothing was dequeued */
}
