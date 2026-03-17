#include <math.h>

#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_parse.h"

static jstruct_value_t const f64_value = {
    .type = JSTRUCT_VALUE_TYPE_F64,
};

void setUp(void) { }
void tearDown(void) { }

void test_zero(void) {
    char const* const test_value = "0.0";
    double out = 123.456;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 0.0, out);
}

void test_positive_value(void) {
    char const* const test_value = "3.141592653589793";
    double out = 0.0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_DOUBLE_WITHIN(0.000000000000001, 3.141592653589793, out);
}

void test_negative_value(void) {
    char const* const test_value = "-2.718281828459045";
    double out = 0.0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_DOUBLE_WITHIN(0.000000000000001, -2.718281828459045, out);
}

void test_integer_as_double(void) {
    char const* const test_value = "42";
    double out = 0.0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 42.0, out);
}

void test_scientific_notation(void) {
    char const* const test_value = "1.23e-10";
    double out = 0.0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_DOUBLE_WITHIN(0.00000000001, 1.23e-10, out);
}

void test_large_value(void) {
    char const* const test_value = "123456789.987654321";
    double out = 0.0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 123456789.987654321, out);
}

void test_very_small_value(void) {
    char const* const test_value = "0.00000000123";
    double out = 0.0;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f64_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_DOUBLE_WITHIN(0.00000000001, 0.00000000123, out);
}

void test_expects_numeric(void) {
    char const* const test_values[] = {
        "true",
        "{}",
        "[]",
        "\"3.14\""
    };
    size_t const num_test_values = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < num_test_values; i++) {
        double out = 0.0;

        jstruct_parse_result_t result = {0};
        bool const success = jstruct_parse(&f64_value, test_values[i], &out, &result);

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
    RUN_TEST(test_positive_value);
    RUN_TEST(test_negative_value);
    RUN_TEST(test_integer_as_double);
    RUN_TEST(test_scientific_notation);
    RUN_TEST(test_large_value);
    RUN_TEST(test_very_small_value);
    RUN_TEST(test_expects_numeric);
    return UNITY_END();
}
