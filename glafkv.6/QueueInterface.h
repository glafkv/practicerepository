/*********************************************************************/
/* QueueInterface.h                                                  */
/*                                                                   */
/* Interface file for a queue data structure                         */
/* Author: Dr. Rick Coleman, Instructor CS207                        */
/* Date: February, 1998                                              */
/*********************************************************************/

#include "QueueTypes.h"

void InitializeQueue(Queue *Q);
int EmptyQueue(Queue *Q);
void Enqueue(ItemType *X, Queue *Q);
int Dequeue(Queue *Q, ItemType *X);
