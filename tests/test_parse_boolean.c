#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_parse.h"

static jstruct_value_t const boolean_value = {
    .type = JSTRUCT_VALUE_TYPE_BOOLEAN,
};

void setUp(void) { }
void tearDown(void) { }

void test_true(void) {
    char const* const test_value = "true";
    bool out = false;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&boolean_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(result.type, JSTRUCT_PARSE_RESULT_TYPE_PASS);
    TEST_ASSERT_TRUE(out);
}

void test_false(void) {
    char const* const test_value = "false";
    bool out = true;

    jstruct_parse_result_t result = {0};
    bool const success = jstruct_parse(&boolean_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_FALSE(out);
}

void test_expects_boolean(void) {
    char const* const test_values[] = {
        "1.0",
        "1",
        "{}",
        "[]",
        "\"test\""
    };
    size_t const num_test_values = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < num_test_values; i++) {
        bool out = false;

        jstruct_parse_result_t result = {0};
        bool const success = jstruct_parse(&boolean_value, test_values[i], &out, &result);

        char message[100];
        sprintf(message, "Test value #%zu: %s", i, test_values[i]);
        TEST_MESSAGE(message);

        TEST_ASSERT_FALSE(success);
        TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE, result.type);
    }
}

void test_invalid_json(void) {
    char const* const test_value = "not valid json";
    bool out = false;

    jstruct_parse_result_t result = {0};
    bool const success = jstruct_parse(&boolean_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_JSON_PARSE_FAILED, result.type);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_true);
    RUN_TEST(test_false);
    RUN_TEST(test_expects_boolean);
    RUN_TEST(test_invalid_json);
    return UNITY_END();
}
