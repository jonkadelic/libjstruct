#include <stdint.h>
#include <string.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_serialize.h"

static jstruct_value_t const i32_value = {
    .type = JSTRUCT_VALUE_TYPE_I32,
};

void setUp(void) { }
void tearDown(void) { }

void test_zero(void) {
    int32_t const input = 0;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&i32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("0", buffer);
}

void test_positive_value(void) {
    int32_t const input = 42;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&i32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("42", buffer);
}

void test_negative_value(void) {
    int32_t const input = -42;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&i32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("-42", buffer);
}

void test_max_value(void) {
    int32_t const input = INT32_MAX;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&i32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("2147483647", buffer);
}

void test_min_value(void) {
    int32_t const input = INT32_MIN;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&i32_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("-2147483648", buffer);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_zero);
    RUN_TEST(test_positive_value);
    RUN_TEST(test_negative_value);
    RUN_TEST(test_max_value);
    RUN_TEST(test_min_value);
    return UNITY_END();
}
