#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"
/**
 * Daniel Lone Author Abdullahi Farah
 */

void initCircularBuffer(struct circularBuffer *bufferPtr, int *data, int maxLength)
{
  bufferPtr->data = data;
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
  bufferPtr->maxLength = maxLength;
  bufferPtr->count = 0;
}

int getCount(struct circularBuffer* bufferPtr)
{
  int len = bufferPtr->count;
  return len;
}


int addElement(struct circularBuffer *bufferPtr, int value)
{
  int res;
  if (bufferPtr->head == (bufferPtr->tail % bufferPtr->maxLength) + 1)
  {
    return INT_MIN;
  }
  else if (bufferPtr->head == 0 && bufferPtr->tail == 0)
  {
    bufferPtr->data[bufferPtr->tail] = value;
    res = bufferPtr->data[bufferPtr->tail];
    bufferPtr->tail = (bufferPtr->tail + 1) % bufferPtr->maxLength;
    bufferPtr->count = (bufferPtr->count + 1) % bufferPtr->maxLength;

    return res;
  }
  else
  {
    bufferPtr->data[bufferPtr->tail] = value;
    res = bufferPtr->data[bufferPtr->tail];
    bufferPtr->tail = (bufferPtr->tail + 1) % bufferPtr->maxLength;
    bufferPtr->count = (bufferPtr->count + 1) % bufferPtr->maxLength;

    return res;
  }

  return INT_MIN;
}



int removeHead(struct circularBuffer *bufferPtr)
{
  int value = INT_MIN;
  if (bufferPtr->head != bufferPtr->tail)
  {
    value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = 0;
    bufferPtr->head = bufferPtr->head + 1 % bufferPtr->maxLength;   
    if (bufferPtr->count > 0)
    {
      bufferPtr->count =bufferPtr->count - 1;
    }
  }

  return value;
}

int getHead(struct circularBuffer *bufferPtr)
{
  int value = INT_MIN;
  if (bufferPtr->head != bufferPtr->tail)
  {
    value = bufferPtr->data[bufferPtr->head];
    bufferPtr->head = bufferPtr->head + 1 % bufferPtr->maxLength;
  }

  return value;
}


void printBuffer(struct circularBuffer *bufferPtr)
{

  if (bufferPtr->count == 0)
  {
    printf("Buffert tom!");
  }
  else
  {
    printf("\n\n");
    for (size_t i = 0; i < bufferPtr->maxLength; i++)
    {
      printf("\n%d", bufferPtr->data[i]);
    }
  }
}

void resetHead(struct circularBuffer* bufferPtr)
{
  bufferPtr->head = 0;
}