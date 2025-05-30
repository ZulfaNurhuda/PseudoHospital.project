#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include "abstractDataTypes.h" // For Queue, QueueInfo, QueueNode
#include "boolean.h"
#include "utils.h"           // For safeMalloc

// Initializes a Queue (front/rear to NULL, size to 0).
// Note: This might be redundant if initHospital already does this for all queues.
// If so, this function might not be strictly needed, but good for completeness.
void initializeQueue(Queue *q, const char* roomCode);

// Checks if the queue is empty.
boolean isQueueEmpty(const Queue *q);

// Returns the number of elements in the queue.
int queueSize(const Queue *q);

// Adds an element (patientID) to the rear of the queue.
// Returns true if successful, false otherwise (e.g., memory allocation failed).
boolean enqueue(Queue *q, int patientID);

// Removes an element from the front of the queue.
// Returns true and stores patientID if successful, false if queue is empty.
boolean dequeue(Queue *q, int *patientID);

// Peeks at the front element of the queue without removing it.
// Returns true and stores patientID if successful, false if queue is empty.
boolean peekQueue(const Queue *q, int *patientID);

// (Optional but helpful for F15/S02) Display contents of a specific queue.
// void displaySpecificQueue(const Queue *q, const Hospital *hospital);

// (Optional but helpful for F15) Get patient position in queue.
// int getPatientPosition(const Queue *q, int patientID);

#endif // MY_QUEUE_H
