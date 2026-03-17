#include <string.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_parse.h"

static jstruct_value_t const string_value_small = {
    .type = JSTRUCT_VALUE_TYPE_STRING,
    .data.string.buffer_size = 16
};

static jstruct_value_t const string_value_large = {
    .type = JSTRUCT_VALUE_TYPE_STRING,
    .data.string.buffer_size = 256
};

void setUp(void) { }
void tearDown(void) { }

void test_empty_string(void) {
    char const* const test_value = "\"\"";
    char out[16] = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&string_value_small, test_value, out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_STRING("", out);
}

void test_short_string(void) {
    char const* const test_value = "\"hello\"";
    char out[16] = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&string_value_small, test_value, out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_STRING("hello", out);
}

void test_max_length_string(void) {
    char const* const test_value = "\"123456789012345\"";  // 15 chars + null terminator = 16
    char out[16] = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&string_value_small, test_value, out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_STRING("123456789012345", out);
}

void test_string_too_long(void) {
    char const* const test_value = "\"1234567890123456\"";  // 16 chars + null = 17
    char out[16] = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&string_value_small, test_value, out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS, result.type);
}

void test_string_with_spaces(void) {
    char const* const test_value = "\"hello world\"";
    char out[256] = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&string_value_large, test_value, out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_STRING("hello world", out);
}

void test_string_with_special_chars(void) {
    char const* const test_value = "\"hello\\nworld\\t!\"";
    char out[256] = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&string_value_large, test_value, out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_STRING("hello\nworld\t!", out);
}

void test_string_with_unicode(void) {
    char const* const test_value = "\"Hello \\u4e16\\u754c\"";
    char out[256] = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&string_value_large, test_value, out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    // json-c will decode the unicode escapes
    TEST_ASSERT_NOT_EQUAL(0, strlen(out));
}

void test_expects_string(void) {
    char const* const test_values[] = {
        "true",
        "123",
        "3.14",
        "{}",
        "[]"
    };
    size_t const num_test_values = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < num_test_values; i++) {
        char out[256] = {0};

        jstruct_parse_result_t result = {0};
        bool const success = jstruct_parse(&string_value_large, test_values[i], out, &result);

        char message[100];
        sprintf(message, "Test value #%zu: %s", i, test_values[i]);
        TEST_MESSAGE(message);

        TEST_ASSERT_FALSE(success);
        TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE, result.type);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty_string);
    RUN_TEST(test_short_string);
    RUN_TEST(test_max_length_string);
    RUN_TEST(test_string_too_long);
    RUN_TEST(test_string_with_spaces);
    RUN_TEST(test_string_with_special_chars);
    RUN_TEST(test_string_with_unicode);
    RUN_TEST(test_expects_string);
    return UNITY_END();
}
