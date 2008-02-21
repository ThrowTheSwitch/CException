====================================================================
CException
====================================================================

CException is a basic exception framework for C, suitable for use in
embedded applications.  It provides an exception framework similar in
use to C++, but with much less overhead.

CException uses C standard library functions setjmp and longjmp to 
operate.  As long as the target system has these two functions defined,
this library should be useable with very little configuration.  It 
even supports environments where multiple program flows are in use, 
such as real-time operating systems.

For the latest version, go to http://cexception.sourceforge.net

--------------------------------------------------------------------
CONTENTS OF THIS DOCUMENT
--------------------------------------------------------------------

Usage
Limitations
API
Configuration
Testing
License

--------------------------------------------------------------------
Usage
--------------------------------------------------------------------

Code that is to be protected are wrapped in Try { } Catch { } blocks.
The code directly following the Try call is "protected", meaning that
if any Throws occur, program control is directly transferred to the
start of the Catch block.

A numerical exception ID is included with Throw, and is made accessible
from the Catch block.  In addition, an optional details field can be
included, allowing a pointer or additional numerical information to be
added.

Throws can occur from within function calls or directly within the 
function itself.

--------------------------------------------------------------------
Limitations
--------------------------------------------------------------------

This library was made to be as fast as possible, and provide *basic*
exception handling.  It is not a full-blown exception library as
provided by many other operating systems.  Because of this, there are 
a number of limitations that should be observed in order to
successfully utilize this library:

1. Do not directly "return" from within a Try block, nor "goto" 
   into or out of a Try block.  
   
   Why? 
   
   The "Try" macro allocates some local memory and alters a global 
   pointer.  These are cleaned up at the top of the "Catch" macro. 
   Gotos and returns would bypass some of these steps, resulting in
   memory leaks or unpredictable behavior.
   
2. If you change stack variables within your Try block (local variables
   for example), and wish to make use of the updated values after an
   exception is thrown, those variables should be made volatile. Note
   that this is ONLY for locals and ONLY when you need access to them
   after a throw.
   
   Why?
   
   Compilers optimize.  There is no way to guarantee that the actual 
   memory location was updated and not just a register unless the 
   variable is marked volatile.

3. While EXCEPTION_ID and EXCEPTION_DETAIL are available outside a 
   Catch, it is good practice to ONLY access them from within the 
   Catch block.
   
   Why?
   
   EXCEPTION_ID is altered whenever a new Try block is encountered.
   Both are obviously altered on Throws.  It's best to isolate calls
   to these to places where your state is well known.

4. Memory which is malloc'd or new'd is not automatically released
   when an error is thrown.  This will sometimes be desirable, and
   othertimes may not.  It will be the responsibility of the Catch
   block to perform this kind of cleanup.
   
   Why?
   
   There's just no easy way to track malloc'd memory, etc., without
   replacing or wrapping malloc calls or something like that.  This 
   is a light framework, so these options were not desirable.
   
5. If additional Try blocks are started from within a Catch (either
   in the immediate function or from within a function called from
   the Catch block), it will alter the EXCEPTION_ID.  It is therefore 
   good practice to NOT call such functions from a Catch block.
   
   Why?
   
   See #3 above.
   
--------------------------------------------------------------------
API
--------------------------------------------------------------------

void Exception_Init(void);

Try
---

Try is a macro which starts a protected block.  It MUST be followed by 
a pair of braces, enclosing the data that is to be protected.  It MUST
be followed by a Catch block.

Catch
-----

Catch is a macro which ends the Try block and starts the error handling 
block. The catch block is called if and only if an exception was thrown 
while within the Try block.  This error could have been thrown by a 
call to Throw or ThrowDetailed.

EXCEPTION_ID
------------

EXCEPTION_ID is a macro which returns the ID of the exception which was
passed to Throw or ThrowDetailed.

EXCEPTION_DETAILS
----------------

EXCEPTION_DETAILS is a macro which returns the detail int of the exception.
If Throw was called, it will contain the line number of the Throw.  If 
ThrowDetailed was called, it will contain whatever detail information was
provided.

Throw
-----

The method of throwing an error.  Throws should only occur from within a
protected (Try...Catch) block, though it may easily be nested many function
calls deep without an impact on performance or functionality.  Try takes 
a single argument, which is an exception id to be used in Catch as the 
reason for the error.

ThrowDetailed
-------------

Operates identically to Throw, with the added ability to send additional
details.  This detail is a single int, which can represent whatever the
developer desires.  This has been used as a pointer to an error message, 
pointer to data which must be freed, an instance id of a problem, etc.

Rethrow
-------
Rethrow can be used in a Catch block and allows the current error to be
pushed back an additional level of protected blocks.  Because protected
blocks can be nested, it is sometimes useful to handle some errors in 
inner blocks while throwing the rest to outside blocks.  This function
does that, allowing the error to be passed on unchanged.

--------------------------------------------------------------------
CONFIGURATION
--------------------------------------------------------------------

CException is a mostly portable library.  It has one universal 
dependency, and another which is required if working in a multi-tasking
environment.

1. The standard C library setjmp must be available

2. If working in a multitasking environment, a method of obtaining an 
   index into an array of frames is required.  If the OS supports a
   method to retrieve Task ID's, and those Tasks are number 0, 1, 2..
   you are in an ideal situation.  Otherwise, a more creative mapping
   function may be required.  Note that this function is likely to be
   called twice for each protected block.  This is the greatest
   overhead in the system.
   
ExceptionConfig.h
-----------------

EXCEPTION_MULTI_STACK - define if you are in a multi-tasking environment

EXCEPTION_NONE - set to a number which will never be an exception id in
                 your system.

EXCEPTION_GET_ID() - If in a multi-tasking environment, this should be 
                     set to be a call to the function described in #2 above.
                     
EXCEPTION_NUM_ID - If in a multi-tasking environment, this should be set
                   to the number of ID's required (usually the number of
                   tasks in the system)

You may also want to include any header files which will commonly be
needed by the rest of your application where it uses exception handling 
here.  For example, OS header files or exception codes would be useful.

--------------------------------------------------------------------
TESTING
--------------------------------------------------------------------

The test suite included makes use of the Unity Test Framework.  It will
require a native C compiler, the example makefile using MinGW's gcc.  
Modify the makefile to include the proper paths to tools, then run make 
to compile and run the test application.  

C_COMPILER - The C compiler to use to perform the tests
C_LIBS - The path to the C libraries (including setjmp) 
UNITY_DIR - The path to the Unity framework (required to run tests)
            (get it at http://embunity.sourceforge.net)

--------------------------------------------------------------------
LICENSE
--------------------------------------------------------------------

This software is licensed under the MIT License

Copyright (c) 2007 Mark VanderVoord

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
