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

//exception frame structures
typedef struct {
  jmp_buf* pFrame;
  volatile unsigned int Exception;
  volatile unsigned int Details;
} EXCEPTION_FRAME_T;
extern volatile EXCEPTION_FRAME_T ExceptionFrames[];

#define MY_FRAME            (ExceptionFrames[EXCEPTION_GET_ID])
#define MY_FRAME_FAST       (ExceptionFrames[MY_ID])
#define EXCEPTION_ID        (MY_FRAME.Exception)
#define EXCEPTION_DETAILS   (MY_FRAME.Details)

#define Try                                                         \
    {                                                               \
        jmp_buf *PrevFrame, NewFrame;                               \
        unsigned int MY_ID = EXCEPTION_GET_ID;                      \
        PrevFrame = MY_FRAME.pFrame;                                \
        MY_FRAME_FAST.pFrame = &NewFrame;                           \
        MY_FRAME_FAST.Details = 0;                                  \
        MY_FRAME_FAST.Exception = EXCEPTION_NONE;                   \
        if (setjmp(NewFrame) == 0) {                                \
            if (&PrevFrame)

#define Catch                                                       \
            else { }                                                \
            MY_FRAME_FAST.Exception = EXCEPTION_NONE;               \
        }                                                           \
        MY_FRAME_FAST.pFrame = PrevFrame;                           \
    }                                                               \
    if (MY_FRAME.Exception != EXCEPTION_NONE)

#define Throw(id)       ThrowDetailed(id, __LINE__)
void ThrowDetailed(unsigned int ExceptionID, unsigned int Details);
void Rethrow(void);

#endif // _CEXCEPTION_H
