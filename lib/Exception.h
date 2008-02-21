#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <setjmp.h>
#include "ExceptionConfig.h"

//exception frame structures
typedef struct {
  jmp_buf* pFrame;
  volatile unsigned int Exception;
  volatile unsigned int Details;
} EXCEPTION_FRAME_T;
extern volatile EXCEPTION_FRAME_T ExceptionFrames[];

#define MY_FRAME            (ExceptionFrames[EXCEPTION_GET_ID()])
#define MY_FRAME_FAST       (ExceptionFrames[MY_ID])
#define EXCEPTION_ID        (MY_FRAME.Exception)
#define EXCEPTION_DETAILS   (MY_FRAME.Details)

#define Try                                                         \
    {                                                               \
        jmp_buf *PrevFrame, NewFrame;                               \
        PrevFrame = MY_FRAME.pFrame;                                \
        unsigned int MY_ID = EXCEPTION_GET_ID();                    \
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

#endif // _EXCEPTION_H
