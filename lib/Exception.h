#ifndef _EXCEPTION_H
#define _EXCEPTION_H

// Define the reserved value representing NO EXCEPTION
#define EXCEPTION_NONE (0x5A5A5A5A)

// Multi-Tasking environments will need a couple of macros defined to make this library 
// properly handle  multiple exception stacks.  You will need to include and required
// definitions, then define the following macros:
//    EXCEPTION_GET_ID - returns the id of the current task indexed 0 to (numtasks - 1)
//    EXCEPTION_NUM_ID - returns the number of tasks that might be returned
//
// For example, Quadros might include the following implementation:
#ifndef TEST
#include "OSAPI.h"
#define EXCEPTION_GET_ID()  (KS_GetTaskID())
#define EXCEPTION_NUM_ID    (NTASKS + 1)
#endif

// INCLUDE THE ACTUAL CEXCEPTION LIBRARY
#include "CException.h"

#endif // _EXCEPTION_H
