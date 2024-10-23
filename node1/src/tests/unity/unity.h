#ifndef UNITY_H
#define UNITY_H

void UnityBegin(void);
void UnityEnd(void);
void UnityAssertEqualInt(const int expected, const int actual, const char* msg, const int line);

#define UNITY_BEGIN() UnityBegin()
#define UNITY_END() UnityEnd()
#define RUN_TEST(func) func()
#define TEST_ASSERT_EQUAL_INT(expected, actual) UnityAssertEqualInt(expected, actual, #expected " != " #actual, __LINE__)
#define TEST_ASSERT_EQUAL_HEX8(expected, actual) UnityAssertEqualInt((int)(expected), (int)(actual), #expected " != " #actual, __LINE__)

#endif // UNITY_H