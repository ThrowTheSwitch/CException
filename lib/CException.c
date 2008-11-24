#include "Exception.h"

volatile EXCEPTION_FRAME_T ExceptionFrames[EXCEPTION_NUM_ID];

//------------------------------------------------------------------------------------------
//  ThrowDetailed
//------------------------------------------------------------------------------------------
//
//  PARAMETERS:         None
//
//  DESCRIPTION:        throws a software exception with details
//
//  RETURNS:            None
//
//------------------------------------------------------------------------------------------
void ThrowDetailed(unsigned int ExceptionID, unsigned int Details)
{
    MY_FRAME.Exception = ExceptionID;
    MY_FRAME.Details   = Details;
    longjmp(*MY_FRAME.pFrame, 1);
}

//------------------------------------------------------------------------------------------
//  Rethrow
//------------------------------------------------------------------------------------------
//
//  PARAMETERS:         None
//
//  DESCRIPTION:        rethrows a software exception that has already been thrown
//
//  RETURNS:            None
//
//------------------------------------------------------------------------------------------
void Rethrow()
{
    longjmp(*MY_FRAME.pFrame, 1);
}

