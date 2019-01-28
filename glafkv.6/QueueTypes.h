/*********************************************************************/
/* QueueTypes.h                                                      */
/*                                                                   */
/* Queue data structure definition file.                             */
/* Author: Dr. Rick Coleman, Instructor CS207                        */
/* Date: February, 1998                                              */
/*********************************************************************/

typedef struct QueueNodeType
{
	int                    numItems;      /* random 5..30 items */
	long                   checkOutTime;  /* ceil(5.0 + (numItems * 1.75) + 76.8) */
	struct QueueNodeType   *next;
}Customer;

typedef Customer	ItemType;

typedef struct
{
	int       Count;
	Customer  *Head;
	Customer  *Tail;
}Queue;

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif
