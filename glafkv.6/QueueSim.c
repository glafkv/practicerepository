/*********************************************************************/
/* QueueSim.c                                                        */
/*                                                                   */
/* Simulation of a grocery store checkout line using a queue data    */
/*  structure implemented as a linked list.                          */
/* Author: Dr. Rick Coleman, Instructor CS207                        */
/* Date: February, 1998                                              */
/*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "QueueInterface.h"


#define RandomArrivalAvg   120       /* Customer arrives approx. every x seconds */
#define ItemTime           1.75
#define StartUpTime        5.0
#define CloseOutTime       76.8

void AddCustomer(Queue *Q);

int main(void)
{
	Queue    theQueue;
	time_t   simStartTime;     /* System time when simulation is begun */
	time_t   simEndTime;       /* System time when simulation is to end */
	time_t   simTime;          /* Current time in the simulation */
	time_t   holdTime;         /* Holder to mark simTime change */
	time_t   plus3RunningTot;  /* Total time there has been more then 3 customers in line */
	time_t   plus3Current;     /* Current time there has been more then 3 customers */
	time_t   checkOutEndTime;  /* Time when the current customer will be finished checking out */
	time_t   addCustomerTime;  /* Time when a new customer is added to the line */
	int      recording = FALSE;/* Boolean flag.  Is time recording for >3 in queue? */
	Customer dummy;            /* Holder for structure Dequeue() returns */

	/* Note: time_t is typedefed as a long in time.h.  This is PC specific code. */

	InitializeQueue(&theQueue);
	srand((unsigned)time(NULL)); /* Seed the random number generator. */

	/* Input simulation data */
	printf("\n\nEnter total running time in minutes.\n");
	scanf("%d", &simEndTime);
	AddCustomer(&theQueue); /* Add first customer to the queue */
	time(&simStartTime); /* This function get the elapsed seconds since        */
	                     /*  midnight Jan 31, 1970, Coordinated Universal Time */
	simTime = simStartTime;
	simEndTime = ((simEndTime) + simStartTime);
	checkOutEndTime = simTime + (theQueue.Head)->checkOutTime;
	plus3RunningTot = 0;
	addCustomerTime = simTime + (rand() % RandomArrivalAvg); /* Set time to add the next customer */
	holdTime = 0;

	/* Main Simulation Loop */
	do
	{
		time(&simTime); /* Get current simulation time */
		if(holdTime < simTime)
		{
			printf("\n\nSim Time = %d:%d (min:sec).\n",
				(simTime - simStartTime) / 60,
				(simTime - simStartTime) % 60);
			printf("Customers in line = %d", theQueue.Count);
			holdTime = simTime;
		}
		/* holdTime--Because the computer runs so fast and time() returns the time
 * 		in seconds, this could be executed many times.  By saving the current
 * 				time in holdTime we can make sure the time is only printed each time the
 * 						second reading changes. */
			
		if((checkOutEndTime <= simTime) && (checkOutEndTime != 0)) 
		{   /* Time for new customer to check out. First remove customer who just */
			/* finished checking out from line, */
			Dequeue(&theQueue, &dummy); 
			printf("\n\nCustomer checked out and leaving store.\n");
			/* Check to see if any more customers are still in line */
			if(theQueue.Head != NULL)
				/* If still a customer in line calc. the time when he will be checked out */
				checkOutEndTime = simTime + (theQueue.Head)->checkOutTime;	 
			else
				checkOutEndTime = 0;
		}
		/* Check number in the queue and recording of time for > 3 in queue */
		if(theQueue.Count > 3)
		{
			if(!recording)	 /* Start recording */
			{
				recording = TRUE;
				plus3Current = simTime;
			}
		}
		else
		{
			if(recording) /* Stop and save time */
			{
				recording = FALSE;
				plus3RunningTot += (simTime - plus3Current);
			}
		}
		/* Check for time to add a new customer to the queue */
		if(addCustomerTime <= simTime) /* Time to add one */
		{
			AddCustomer(&theQueue);
			addCustomerTime = simTime + (rand() % RandomArrivalAvg);
			printf("\n\nNew customer arriving in line.\n");
		}
	}
	while(simTime < simEndTime);

	if(recording) /* Add in any remaining time with more than 3 customers in line */
		plus3RunningTot += (simTime - plus3Current);

	printf("\n\nResults from simulation:\n");
	printf("Total simulation time = %d:%d (min:sec)\n", 
		(simEndTime - simStartTime) / 60,
		(simEndTime - simStartTime) % 60);
	printf("Total time more then 3 customers in line = %d:%d (min:sec)\n", 
		plus3RunningTot / 60,
		plus3RunningTot % 60);
	printf("Number of customers left in line at end of simulation = %d\n", theQueue.Count);
	printf("-----End Simulation Run-----\n\n");
	return(0);
}



/***********************************/
/* AddCustomer()                   */
/*                                 */
/* Create a new customer data      */
/*  structure and add it to the    */
/*  queue.                         */
/***********************************/
void AddCustomer(Queue *Q)
{
	Customer *newCustomer;

	newCustomer = (Customer *)malloc(sizeof(Customer));
	newCustomer->numItems = (rand() % 25) + 5; /* Set to 5..30 items */
	newCustomer->checkOutTime = (long)(StartUpTime + (newCustomer->numItems * ItemTime) +
									   CloseOutTime);
	Enqueue(newCustomer, Q);
}
	
