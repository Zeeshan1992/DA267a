#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H


/* 
 * Data structure used to hold a circular buffer.
 */
struct circularBuffer{
  int * data;
  int head;
  int tail;
  int maxLength;
  int count;
};


/*
 * Initialize an empty buffer.
 */
void initCircularBuffer(struct circularBuffer* bufferPtr, int* data, int maxLen);




/*
 * This function should add the value specified by the 'value' 
 * argument at the tail of the buffer.
 *
 * The function should return:
 *  - 'value' if the value was successfully added to the queue.
 *  - INT_MIN (defined in limits.h) if the value was not added.
 */
int addElement(struct circularBuffer* bufferPtr, int value);



/*
* Same as removeHead except it wont delete the value
*/
int getHead(struct circularBuffer *bufferPtr);


/* 
 * Remove the oldest element, which is at the head of the queue. 
 * 
 * The function should return:
 *   - 'value' if the head element was successfully removed
 *   - INT_MIN (defined in limits.h) if no element was removed (i.e., the
 *     queue was empty when the function was called.       
 */
int removeHead(struct circularBuffer* bufferPtr);

/* 
 * Print the elements in the buffer from head to tail. 
 */
void printBuffer(struct circularBuffer* bufferPtr);

// set head to 0
void resetHead(struct circularBuffer* bufferPtr);

//get count
int getCount(struct circularBuffer* bufferPtr);

#endif

