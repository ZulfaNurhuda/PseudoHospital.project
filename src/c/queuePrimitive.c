#include "queuePrimitive.h"

void initializeQueue(Queue *q, const char *roomCode)
{
    if (q == NULL)
    {
        return;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    if (roomCode != NULL)
    {
        strcpy(q->roomCode, roomCode);
    }
    else
    {
        q->roomCode[0] = '\0';
    }
}

boolean isQueueEmpty(const Queue *q)
{
    if (q == NULL)
    {
        return true;
    }
    return q->size == 0;
}

int queueSize(const Queue *q)
{
    if (q == NULL)
    {
        return 0;
    }
    return q->size;
}

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
    newNode->prev = q->tail;
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
        q->head->prev = NULL;
    }
    free(temp);
    q->size--;
    return true;
}

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