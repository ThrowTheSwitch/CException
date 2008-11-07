#include "unity.h"
#include "Exception.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_BasicTryDoesNothingIfNoThrow(void)
{
  int i;
  Try
  {
    i += 1;
  }
  Catch
  {
    TEST_FAIL("Should Not Enter Catch If Not Thrown")
  }
}

void test_BasicThrowAndCatch(void)
{
  volatile unsigned int ID = 0;

  Try
  {
    Throw(0xBEEFBEEF);
    TEST_FAIL("Should Have Thrown An Error")
  }
  Catch
  {
    ID = EXCEPTION_ID;
  }

  TEST_ASSERT_EQUAL(0xBEEFBEEF, ID);
}

void test_VerifyVolatilesSurviveThrowAndCatch(void)
{
  volatile unsigned int VolVal = 0;

  Try
  {
    VolVal = 2;
    Throw(0xBEEFBEEF);
    TEST_FAIL("Should Have Thrown An Error")
  }
  Catch
  {
    VolVal += 2;
    TEST_ASSERT_EQUAL(0xBEEFBEEF, EXCEPTION_ID);
  }

  TEST_ASSERT_EQUAL(4, VolVal);
}

void HappyExceptionThrower(unsigned int ID)
{
  if (ID != 0)
  Throw(ID);
}

void test_ThrowFromASubFunctionAndCatchInRootFunc(void)
{
  volatile  unsigned int ID = 0;

  Try
  {

    HappyExceptionThrower(0xBADDF00D);
    TEST_FAIL("Should Have Thrown An Exception");
  }
  Catch
  {
    ID = EXCEPTION_ID;
  }

  TEST_ASSERT_EQUAL(0xBADDF00D, ID);
}

void HappyExceptionRethrower(unsigned int ID)
{
  Try
  {
    Throw(ID);
  }
  Catch
  {
    switch (EXCEPTION_ID)
    {
    case 0xBADDF00D:
      Rethrow();
      break;
    default:
      break;
    }
  }
}

void test_ThrowAndCatchFromASubFunctionAndRethrowToCatchInRootFunc(void)
{
  volatile  unsigned int ID = 0;
  Try
  {
    HappyExceptionRethrower(0xBADDF00D);
    TEST_FAIL("Should Have Rethrown Exception");
  }
  Catch
  {
    ID = EXCEPTION_ID;
  }

  TEST_ASSERT_EQUAL(0xBADDF00D, ID);
}

void test_ThrowAndCatchFromASubFunctionAndNoRethrowToCatchInRootFunc(void)
{
  Try
  {
    HappyExceptionRethrower(0xBADDBEEF);
  }
  Catch
  {
    TEST_FAIL("Should Not Have Thrown Error");
  }
}

void test_CanHaveMultipleTryBlocksInASingleFunction(void)
{
  Try
  {
    HappyExceptionThrower(0x01234567);
    TEST_FAIL("Should Have Thrown Exception");
  }
  Catch
  {
    TEST_ASSERT_EQUAL(0x01234567, EXCEPTION_ID);
  }

  Try
  {
    HappyExceptionThrower(0xF00D8888);
    TEST_FAIL("Should Have Thrown Exception");
  }
  Catch
  {
    TEST_ASSERT_EQUAL(0xF00D8888, EXCEPTION_ID);
  }
}

void test_CanHaveNestedTryBlocksInASingleFunction_ThrowInside(void)
{
  int i = 0;
  Try
  {
    Try
    {
      HappyExceptionThrower(0x01234567);
      i = 1;
      TEST_FAIL("Should Have Rethrown Exception");
    }
    Catch
    {
      TEST_ASSERT_EQUAL(0x01234567, EXCEPTION_ID);
    }
  }
  Catch
  {
    TEST_FAIL("Should Have Been Caught By Inside Catch");
  }
}

void test_CanHaveNestedTryBlocksInASingleFunction_ThrowOutside(void)
{
  int i = 0;
  Try
  {
    Try
    {
      i = 2;
    }
    Catch
    {
      TEST_FAIL("Should NotBe Caught Here");
    }
    HappyExceptionThrower(0x01234567);
    TEST_FAIL("Should Have Rethrown Exception");
  }
  Catch
  {
    TEST_ASSERT_EQUAL(0x01234567, EXCEPTION_ID);
  }
}

void HappyDetailedExceptionThrower(unsigned int ID, unsigned int Details)
{
  if (ID != 0)
  ThrowDetailed(ID, Details);
}

void test_CanThrowADetailedExceptionAndCheckOutTheResults(void)
{
  Try
  {
    HappyDetailedExceptionThrower(0x12345678, 0x90ABCDEF);
    TEST_FAIL("Should Have Thrown An Exception");
  }
  Catch
  {
    TEST_ASSERT_EQUAL(0x12345678, EXCEPTION_ID);
    TEST_ASSERT_EQUAL(0x90ABCDEF, EXCEPTION_DETAILS);
  }
}

