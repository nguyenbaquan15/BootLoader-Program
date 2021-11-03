#include "stdio.h"
#include "stdint.h"
#include "Queue.h"


/* -----------------------------Private variables ----------------------------*/

/* Difition 1 queue use array */
Queue_t Queue[MAXQUEUE];

/* Variable used to posh data from queue */
uint8_t Front;

/* Variable used to push data to queue */
uint8_t Rear;

/* Variable used count number element is full data in queue */
uint8_t Count;

/* ---------------------------------------------------------------------------*/


void Queue_Init(void)
{
    /* first element of queue */
    Front=0;

    /* End element of queue */
    Rear=0;

    /* element number has data in queue */
    Count=0;
}

void Queue_Push(char c)
{
	static uint8_t index=0;

    /* Check status of Queue */
    if( Queue_Status()!= QUEUE_FULL )
    {
        /* Receiver a character to element of queue */
        Queue[Rear].Element[index]=c;

        /* Check '\n' is end */
        if(Queue[Rear].Element[index]=='\n')
        {
            /* Reset index use for next element */
            index=0;

            /* Set element is full status */
            Queue[Rear].Full=1;

            /* Next element to receiver character */
            Rear++;

            /* Increase element number has data in queue */
            Count++;

            if(Rear==MAXQUEUE)
            {
                /* Move to first element of queue */
                Rear=0;
            }
        }
        else
        {
            index++;
        }
    }
    /* Queue full */
    else
    {
        /* do nothing */
    }

}

char *Queue_Peek(void)
{
    char *ptr;

    /* Check status element queue */
    if(Queue[Front].Full==1)
    {
        /* Get address element of queue */
        ptr=(char *)(&Queue[Front].Element);
    }
    else
    {
        /* do nothing */
    }

    return ptr;
}

void Queue_Pop(void)
{
    /* Set element is empty status */
    Queue[Front].Full=0;

    /* Decrease element number has data in queue */
    Count--;

    /* Next element */
    Front++;

    if(Front==MAXQUEUE)
    {
        /* Move to first element of queue */
        Front=0;
    }
}

QueueStatus_t Queue_Status(void)
{
    QueueStatus_t Status=PASS;

    /* Check status queue empty */
    if(Count==0)
    {
        Status=QUEUE_EMPTY;
    }

    /* Check status queue full */
    if(Count==MAXQUEUE)
    {
        Status=QUEUE_FULL;
    }

    return Status;
}