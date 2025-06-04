#ifndef VIEW_PATIENT_QUEUE_H
#define VIEW_PATIENT_QUEUE_H

#include "abstractDataTypes.h"
#include "utils.h"
#include "queuePrimitive.h"

// Function for a patient to view their own queue status.
// (Formerly the 'myQueue' function from the old myQueue.c)
void viewMySpecificQueueStatus(Hospital *hospital, Session *session);

#endif // VIEW_PATIENT_QUEUE_H
