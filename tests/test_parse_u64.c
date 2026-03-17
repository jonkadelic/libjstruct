#include <stdint.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_parse.h"

static jstruct_value_t const u64_value = {
    .type = JSTRUCT_VALUE_TYPE_U64,
};

void setUp(void) { }
void tearDown(void) { }

void test_zero(void) {
    char const* const test_value = "0";
    uint64_t out = UINT64_MAX;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&u64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_UINT64(0, out);
}

void test_max_safe_value(void) {
    // JSON-C uses int64/uint64, so we test a safe large value
    char const* const test_value = "9223372036854775807";  // INT64_MAX
    uint64_t out = 0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&u64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_UINT64(9223372036854775807ULL, out);
}

void test_mid_value(void) {
    char const* const test_value = "1000000000000";
    uint64_t out = 0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&u64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_UINT64(1000000000000ULL, out);
}

void test_out_of_bounds_negative(void) {
    char const* const test_value = "-1";
    uint64_t out = 0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&u64_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS, result.type);
}

void test_expects_integer(void) {
    char const* const test_values[] = {
        "true",
        "1.5",
        "{}",
        "[]",
        "\"123\""
    };
    size_t const num_test_values = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < num_test_values; i++) {
        uint64_t out = 0;

        jstruct_parse_result_t result = {0};
        bool const success = jstruct_parse(&u64_value, test_values[i], &out, &result);

        char message[100];
        sprintf(message, "Test value #%zu: %s", i, test_values[i]);
        TEST_MESSAGE(message);

        TEST_ASSERT_FALSE(success);
        TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE, result.type);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_zero);
    RUN_TEST(test_max_safe_value);
    RUN_TEST(test_mid_value);
    RUN_TEST(test_out_of_bounds_negative);
    RUN_TEST(test_expects_integer);
    return UNITY_END();
}
