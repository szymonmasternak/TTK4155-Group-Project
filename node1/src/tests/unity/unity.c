#include "unity.h"

static int UnityFailures;

void UnityBegin(void) {
    UnityFailures = 0;
    printf("Unity Test Framework Initialized\n");
}

void UnityEnd(void) {
    if (UnityFailures == 0) {
        printf("All tests passed!\n");
    } else {
        printf("%d tests failed!\n", UnityFailures);
    }
}

void UnityAssertEqualInt(const int expected, const int actual, const char* msg, const int line) {
    if (expected != actual) {
        printf("Line %d: %s Expected: %d, Actual: %d\n", line, msg, expected, actual);
        UnityFailures++;
    }
}