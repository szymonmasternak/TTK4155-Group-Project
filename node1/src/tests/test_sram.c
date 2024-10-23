#include "unity.h"
#include "sram.h"

void setUp(void) {}

void tearDown(void) {}

void test_sram_init(void) {
    TEST_ASSERT_EQUAL_INT(0, sram_init());
}

void test_sram_write_read(void) {
    uint8_t test_data = 0xAB;
    sram_write(0x1000, test_data);
    TEST_ASSERT_EQUAL_HEX8(test_data, sram_read(0x1000));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_sram_init);
    RUN_TEST(test_sram_write_read);
    return UNITY_END();
}