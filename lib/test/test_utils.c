#include <stdbool.h>

#include "test_utils.h"

void runTest(char* testName, bool (*testFunction)()) {
    if (testFunction()) {
        LOG_PASS("Test %s passed!\n", testName);
    } else {
        LOG_FAIL("Test %s failed.\n", testName);
    }
}

