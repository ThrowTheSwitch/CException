#ifndef _EXCEPTION_CONFIG_H
#define _EXCEPTION_CONFIG_H

//#define EXCEPTION_MULTI_STACK
#define EXCEPTION_NONE              (0x5A5A5A5A)

//if using multistack support and not currently running a test, must include multistack support
#undef EXCEPTION_INCLUDE_MULTI_STACK_SUPPORT
#ifndef TEST
  #ifdef EXCEPTION_MULTI_STACK
    #define EXCEPTION_INCLUDE_MULTI_STACK_SUPPORT
  #endif
#endif

//define the method of automatically looking up the current frame index (mappable from task ID if the task ID's are consecutive)
//    The example below supports the Quadros OS in Multi-Stack Mode.  Most RTOS will support a similar function call.
#ifdef EXCEPTION_INCLUDE_MULTI_STACK_SUPPORT
  #include "OSAPI.h"
  #define EXCEPTION_GET_ID()  (KS_GetTaskID())
  #define EXCEPTION_NUM_ID    (NTASKS + 1)
#else
  #define EXCEPTION_GET_ID()  (0) //use the first index always because there is only one anyway
  #define EXCEPTION_NUM_ID    (1) //there is only the one stack
#endif

#endif //_EXCEPTION_CONFIG_H
