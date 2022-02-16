#include <stdio.h>
#include "queue.h"
#include <limits.h>

void initQ(struct cQueue* qPtr, int* data, int maxLen)
{
    qPtr->head = -1;
    qPtr->tail = -1;
    qPtr->maxLength = maxLen;
    qPtr->data = data;
}

int isFull(struct cQueue* qPtr)
{
    int full = qPtr->head+1 == qPtr->tail ? 1 : 0;
    return full;
}

int isEmpty(struct cQueue* qPtr)
{
    int empty = (qPtr->head == -1 && qPtr->tail == -1) ? 1 : 0;
    return empty;
}

void enQueue(struct cQueue* qPtr, int value)
{
    if (isFull(qPtr) == 1)
    {
        printf("Full... replacing value. FIFO\n");
        qPtr->tail = 0;
    }
    else if(isEmpty(qPtr) == 1)
    {
        qPtr->head = 0;
        qPtr->tail = 0;
    }
    else
    {
        qPtr->tail = (qPtr->tail+1) % qPtr->maxLength;
    }
    qPtr->data[qPtr->tail] = value;
}

int deQueue(struct cQueue* qPtr)
{
    if(isEmpty(qPtr) == 1)
    {
        printf("Empty...\n");
        return INT_MIN;
    }
    int data = qPtr->data[qPtr->head];
    qPtr->data[qPtr->head] = 0;
    if(qPtr->head == qPtr->tail) // Last element
    {
        qPtr->head = -1;
        qPtr->tail = -1;
    }
    else if(qPtr->head == qPtr->maxLength-1)
    {
        qPtr->head = 0;
    }
    else
    {
        qPtr->head = (qPtr->head+1) % qPtr->maxLength;
    }
    return data;
}

int peek(struct cQueue* qPtr)
{
    if(isEmpty(qPtr) == 1)
    {
        return INT_MIN;
    }
    return qPtr->data[qPtr->tail];
}

