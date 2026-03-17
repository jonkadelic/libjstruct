#include <stdint.h>
#include <string.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_serialize.h"

static jstruct_value_t const u32_value = {
    .type = JSTRUCT_VALUE_TYPE_U32,
};

void setUp(void) { }
void tearDown(void) { }

void test_zero(void) {
    uint32_t const input = 0;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&u32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("0", buffer);
}

void test_positive_value(void) {
    uint32_t const input = 42;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&u32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("42", buffer);
}

void test_max_value(void) {
    uint32_t const input = UINT32_MAX;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&u32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("4294967295", buffer);
}

void test_large_value(void) {
    uint32_t const input = 1234567890;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&u32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("1234567890", buffer);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_zero);
    RUN_TEST(test_positive_value);
    RUN_TEST(test_max_value);
    RUN_TEST(test_large_value);
    return UNITY_END();
}
