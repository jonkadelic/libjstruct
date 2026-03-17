#include <string.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_serialize.h"

static jstruct_value_t const string_value = {
    .type = JSTRUCT_VALUE_TYPE_STRING,
    .data.string.buffer_size = 64
};

void setUp(void) { }
void tearDown(void) { }

void test_simple_string(void) {
    char const input[] = "hello";
    char buffer[64] = {0};

    bool success = jstruct_serialize(&string_value, input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("\"hello\"", buffer);
}

void test_empty_string(void) {
    char const input[] = "";
    char buffer[64] = {0};

    bool success = jstruct_serialize(&string_value, input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("\"\"", buffer);
}

void test_string_with_spaces(void) {
    char const input[] = "hello world";
    char buffer[64] = {0};

    bool success = jstruct_serialize(&string_value, input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("\"hello world\"", buffer);
}

void test_string_with_special_chars(void) {
    char const input[] = "test\nwith\ttabs";
    char buffer[64] = {0};

    bool success = jstruct_serialize(&string_value, input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    // Should contain escaped characters
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    TEST_ASSERT_TRUE(strstr(buffer, "\\n") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\\t") != nullptr);
}

void test_unicode_string(void) {
    char const input[] = "Hello 世界";
    char buffer[64] = {0};

    bool success = jstruct_serialize(&string_value, input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_string);
    RUN_TEST(test_empty_string);
    RUN_TEST(test_string_with_spaces);
    RUN_TEST(test_string_with_special_chars);
    RUN_TEST(test_unicode_string);
    return UNITY_END();
}
