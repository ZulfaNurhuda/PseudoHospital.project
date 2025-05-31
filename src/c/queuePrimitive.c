#include "queuePrimitive.h"

// Initializes a Queue (front/rear to NULL, size to 0).
void initializeQueue(Queue *q, const char* roomCode) {
    if (q == NULL) {
        // Depending on project's error handling, might print an error or assert
        // printError("Error: Queue pointer is NULL in initializeQueue.\n");
        return; // Or handle error more gracefully if possible
    }
    q->head = NULL;
    q->tail = NULL;
    
    if (roomCode != NULL) {
        // customSafeStrcpy(q->roomCode, roomCode, sizeof(q->roomCode));
        strcpy(q->roomCode, roomCode); // Assuming q->roomCode is always large enough
    } else {
        q->roomCode[0] = '\0'; // Default to empty string if roomCode is NULL
    }
}

// Checks if the queue is empty.
boolean isQueueEmpty(const Queue *q) {
    if (q == NULL) {
        // printError("Error: Queue pointer is NULL in isQueueEmpty.\n");
        return true; // Consistent with queueSize returning 0 for NULL queue
    }
    // return q->front == NULL; // This also works if size is not maintained or for extra check
    return q->size == 0; // More reliable if q->size is accurately maintained
}

// Returns the number of elements in the queue.
int queueSize(const Queue *q) {
    if (q == NULL) {
        // printError("Error: Queue pointer is NULL in queueSize.\n");
        return 0; 
    }
    return q->size;
}

// Adds an element (patientId) to the rear of the queue.
// Returns true if successful, false otherwise (e.g., q is NULL).
// safeMalloc handles memory allocation failure by exiting.
boolean enqueue(Queue *q, int patientId) {
    if (q == NULL) {
        // printError("Error: Queue pointer is NULL in enqueue.\n");
        return false;
    }

    QueueNode *newNode = (QueueNode *)safeMalloc(sizeof(QueueNode));
    if (newNode == NULL) {
        printError("Gagal alokasi memori untuk node antrian baru!");
        return false; // Allocation failed
    }
    newNode->info.patientId = patientId;
    newNode->next = NULL;

    if (isQueueEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode; // Update rear to the new node
    }
    q->size++;
    return true;
}

// Removes an element from the front of the queue.
// Returns true and stores patientId if successful, false if queue is empty or q is NULL.
boolean dequeue(Queue *q, int *patientId) {
    if (q == NULL) {
        // printError("Error: Queue pointer is NULL in dequeue.\n");
        if (patientId != NULL) *patientId = -1; // Indicate error or invalid Id
        return false;
    }
    if (isQueueEmpty(q)) {
        // This is a valid operational state, usually not an "error" to print.
        if (patientId != NULL) *patientId = -1; // Indicate queue was empty
        return false;
    }

    QueueNode *temp = q->front;
    if (patientId != NULL) { // Allow patientId to be NULL if caller doesn't need the value
        *patientId = temp->info.patientId;
    }
    
    q->front = q->front->next;
    if (q->front == NULL) { // If queue becomes empty after dequeue
        q->rear = NULL;
    }
    
    free(temp); // Free the dequeued node
    q->size--;
    return true;
}

// Peeks at the front element of the queue without removing it.
// Returns true and stores patientId if successful, false if queue is empty or q is NULL.
boolean peekQueue(const Queue *q, int *patientId) {
    if (q == NULL) {
        // printError("Error: Queue pointer is NULL in peekQueue.\n");
        if (patientId != NULL) *patientId = -1;
        return false;
    }
    if (isQueueEmpty(q)) {
        // Valid operational state.
        if (patientId != NULL) *patientId = -1;
        return false;
    }
    
    if (patientId != NULL) { // Allow patientId to be NULL
        *patientId = q->front->info.patientId;
    }
    return true;
}
