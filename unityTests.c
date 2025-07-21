#include "unityTestSuite/unity.h"
#include "unityTestSuite/unity.c"
#include "library.h"
#include "library.c"
#include <stdbool.h>
#include <limits.h>

void setUp(){

}

void test_correctCMDLineInput(){
    TEST_ASSERT_EQUAL_INT(true, correctCMDLineInput(2));
    TEST_ASSERT_EQUAL_INT(false, correctCMDLineInput(1));
    TEST_ASSERT_EQUAL_INT(false, correctCMDLineInput(INT_MAX));
}


void tearDown(){

}

int main(){
    UNITY_BEGIN();

    RUN_TEST(test_correctCMDLineInput);

    return UNITY_END();
}