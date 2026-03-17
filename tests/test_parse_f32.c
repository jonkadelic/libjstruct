#include <math.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_parse.h"

static jstruct_value_t const f32_value = {
    .type = JSTRUCT_VALUE_TYPE_F32,
};

void setUp(void) { }
void tearDown(void) { }

void test_zero(void) {
    char const* const test_value = "0.0";
    float out = 123.456f;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f32_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, out);
}

void test_positive_value(void) {
    char const* const test_value = "3.14159";
    float out = 0.0f;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f32_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_FLOAT_WITHIN(0.00001f, 3.14159f, out);
}

void test_negative_value(void) {
    char const* const test_value = "-2.71828";
    float out = 0.0f;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f32_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_FLOAT_WITHIN(0.00001f, -2.71828f, out);
}

void test_integer_as_float(void) {
    char const* const test_value = "42";
    float out = 0.0f;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f32_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 42.0f, out);
}

void test_scientific_notation(void) {
    char const* const test_value = "1.23e-4";
    float out = 0.0f;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f32_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_FLOAT_WITHIN(0.000001f, 1.23e-4f, out);
}

void test_large_value(void) {
    char const* const test_value = "1234567.89";
    float out = 0.0f;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&f32_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1234567.89f, out);
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
        float out = 0.0f;

        jstruct_parse_result_t result = {0};
        bool const success = jstruct_parse(&f32_value, test_values[i], &out, &result);

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
    RUN_TEST(test_integer_as_float);
    RUN_TEST(test_scientific_notation);
    RUN_TEST(test_large_value);
    RUN_TEST(test_expects_numeric);
    return UNITY_END();
}
