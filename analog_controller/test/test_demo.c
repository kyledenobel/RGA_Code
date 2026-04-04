#include "unity.h"
#include "ceedling_demo.h"

void setUp(void) {}
void tearDown(void) {}

void test_demo(void)
{
  TEST_ASSERT_EQUAL_INT(3, run_demo(1,1));
  TEST_ASSERT_EQUAL_INT(6, run_demo(2,2));
  TEST_ASSERT_EQUAL_INT(13, run_demo(4,5));

}