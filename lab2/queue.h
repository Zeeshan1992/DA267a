# ifndef QUEUE
# define QUEUE

struct cQueue{
    int head;
    int tail;
    int maxLength;
    int* data;
};
/*
* init a circular queue
*/
void initQ(struct cQueue* qPtr, int* data, int maxLen);
// checks whether the queue is full
int isFull(struct cQueue* qPtr);
// checks whether the queue is empty
int isEmpty(struct cQueue* qPtr);
/*
* adds a value at the tail
*/
void enQueue(struct cQueue* qPtr, int value);
/*
* removes a value at the head
*/
int deQueue(struct cQueue* qPtr);
/*
* returns the next value at the head
*/
int peek(struct cQueue* qPtr);

# endif
