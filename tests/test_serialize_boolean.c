#include <string.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_serialize.h"

static jstruct_value_t const boolean_value = {
    .type = JSTRUCT_VALUE_TYPE_BOOLEAN,
};

void setUp(void) { }
void tearDown(void) { }

void test_true(void) {
    bool const input = true;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&boolean_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("true", buffer);
}

void test_false(void) {
    bool const input = false;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&boolean_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("false", buffer);
}

void test_buffer_too_small(void) {
    bool const input = true;
    char buffer[4] = {0};  // Not enough space for "true" + null terminator

    bool success = jstruct_serialize(&boolean_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_FALSE(success);
}

void test_null_input(void) {
    char buffer[32] = {0};

    bool success = jstruct_serialize(&boolean_value, nullptr, sizeof(buffer), buffer);

    TEST_ASSERT_FALSE(success);
}

void test_zero_buffer_len(void) {
    bool const input = true;
    char buffer[32] = {0};

    bool success = jstruct_serialize(&boolean_value, &input, 0, buffer);

    TEST_ASSERT_FALSE(success);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_true);
    RUN_TEST(test_false);
    RUN_TEST(test_buffer_too_small);
    RUN_TEST(test_null_input);
    RUN_TEST(test_zero_buffer_len);
    return UNITY_END();
}
