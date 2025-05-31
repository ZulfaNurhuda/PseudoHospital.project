#include "queuePrimitive.h"

// Initializes a Queue (front/rear to NULL, size to 0).
void initializeQueue(Queue *q, const char *roomCode)
{
    if (q == NULL)
    {
        return;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0; // Initialize size
    if (roomCode != NULL)
    {
        strcpy(q->roomCode, roomCode);
    }
    else
    {
        q->roomCode[0] = '\0';
    }
}

// Checks if the queue is empty.
boolean isQueueEmpty(const Queue *q)
{
    if (q == NULL)
    {
        return true;
    }
    return q->size == 0;
}

// Returns the number of elements in the queue.
int queueSize(const Queue *q)
{
    if (q == NULL)
    {
        return 0;
    }
    return q->size;
}

// Adds an element (patientId) to the rear of the queue.
// Returns true if successful, false otherwise (e.g., q is NULL).
// safeMalloc handles memory allocation failure by exiting.
boolean enqueue(Queue *q, int patientId)
{
    if (q == NULL)
    {
        return false;
    }
    QueueNode *newNode = (QueueNode *)safeMalloc(sizeof(QueueNode));
    if (newNode == NULL)
    {
        printError("Gagal alokasi memori untuk node antrian baru!");
        return false;
    }
    newNode->info.patientId = patientId;
    newNode->next = NULL;
    newNode->prev = q->tail; // Set prev to current tail
    if (isQueueEmpty(q))
    {
        q->head = newNode;
        q->tail = newNode;
    }
    else
    {
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->size++;
    return true;
}

// Removes an element from the front of the queue.
// Returns true and stores patientId if successful, false if queue is empty or q is NULL.
boolean dequeue(Queue *q, int *patientId)
{
    if (q == NULL || isQueueEmpty(q))
    {
        if (patientId != NULL)
            *patientId = -1;
        return false;
    }
    QueueNode *temp = q->head;
    if (patientId != NULL)
    {
        *patientId = temp->info.patientId;
    }
    q->head = q->head->next;
    if (q->head == NULL)
    {
        q->tail = NULL;
    }
    else
    {
        q->head->prev = NULL; // Update prev for new head
    }
    free(temp);
    q->size--;
    return true;
}

// Peeks at the front element of the queue without removing it.
// Returns true and stores patientId if successful, false if queue is empty or q is NULL.
boolean peekQueue(const Queue *q, int *patientId)
{
    if (q == NULL || isQueueEmpty(q))
    {
        if (patientId != NULL)
            *patientId = -1;
        return false;
    }
    if (patientId != NULL)
    {
        *patientId = q->head->info.patientId;
    }
    return true;
}