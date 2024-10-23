#include "unity.h"
#include "can.h"

void setUp(void) {}

void tearDown(void) {}

void test_can_init(void) {
    TEST_ASSERT_EQUAL_INT(0, can_init());
}

void test_can_send(void) {
    CANMessage message;
    message.id = 1;
    message.data[0] = 0x55;
    message.length = 1;
    
    TEST_ASSERT_EQUAL_INT(0, can_send(&message));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_can_init);
    RUN_TEST(test_can_send);
    return UNITY_END();
}