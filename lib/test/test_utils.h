#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>

// Allows color coded the test results 
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Print red for fail and green for pass
#define LOG_PASS(format, ...) printf(ANSI_COLOR_GREEN format ANSI_COLOR_RESET, ##__VA_ARGS__)
#define LOG_FAIL(format, ...) printf(ANSI_COLOR_RED format ANSI_COLOR_RESET, ##__VA_ARGS__)

// Util function for calling tests
// Displays the appropriate message depending on whether a test passed or failed
void runTest(char* testName, bool (*testFunction)());

#endif // !TEST_UTILS_H
#define TEST_UTILS_H
