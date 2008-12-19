#ifndef _CEXCEPTION_H
#define _CEXCEPTION_H

#include <setjmp.h>

#ifndef EXCEPTION_NONE
#define EXCEPTION_NONE      (0x5A5A5A5A)
#endif

#ifndef EXCEPTION_NUM_ID
#define EXCEPTION_NUM_ID    (1) //there is only the one stack by default
#endif

#ifndef EXCEPTION_GET_ID
#define EXCEPTION_GET_ID    (0) //use the first index always because there is only one anyway
#endif

#ifndef EXCEPTION_T
#define EXCEPTION_T         unsigned int
#endif

//exception frame structures
typedef struct {
  jmp_buf* pFrame;
  volatile EXCEPTION_T Exception;
} EXCEPTION_FRAME_T;

//actual root frame storage (only one if single-tasking)
extern volatile EXCEPTION_FRAME_T ExceptionFrames[];

//Try (see C file for explanation)
#define Try                                                         \
    {                                                               \
        jmp_buf *PrevFrame, NewFrame;                               \
        unsigned int MY_ID = EXCEPTION_GET_ID;                      \
        PrevFrame = ExceptionFrames[EXCEPTION_GET_ID].pFrame;       \
        ExceptionFrames[MY_ID].pFrame = &NewFrame;                  \
        ExceptionFrames[MY_ID].Exception = EXCEPTION_NONE;          \
        if (setjmp(NewFrame) == 0) {                                \
            if (&PrevFrame) 

//Catch (see C file for explanation)
#define Catch(e)                                                    \
            else { }                                                \
            ExceptionFrames[MY_ID].Exception = EXCEPTION_NONE;      \
        }                                                           \
        else                                                        \
        { e = ExceptionFrames[MY_ID].Exception; e=e; }              \
        ExceptionFrames[MY_ID].pFrame = PrevFrame;                  \
    }                                                               \
    if (ExceptionFrames[EXCEPTION_GET_ID].Exception != EXCEPTION_NONE)

//Throw an Error
void Throw(EXCEPTION_T ExceptionID);

#endif // _CEXCEPTION_H
