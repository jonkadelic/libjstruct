#include <math.h>
#include <string.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_serialize.h"

static jstruct_value_t const f64_value = {
    .type = JSTRUCT_VALUE_TYPE_F64,
};

void setUp(void) { }
void tearDown(void) { }

void test_zero(void) {
    double const input = 0.0;
    char buffer[64] = {0};

    bool success = jstruct_serialize(&f64_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    // JSON-C might output "0" or "0.0" or "0.000000"
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    TEST_ASSERT_TRUE(buffer[0] == '0');
}

void test_positive_value(void) {
    double const input = 3.141592653589793;
    char buffer[64] = {0};

    bool success = jstruct_serialize(&f64_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    // Verify we can parse it back
    double parsed = atof(buffer);
    TEST_ASSERT_DOUBLE_WITHIN(0.000000000000001, input, parsed);
}

void test_negative_value(void) {
    double const input = -2.718281828459045;
    char buffer[64] = {0};

    bool success = jstruct_serialize(&f64_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    // Verify we can parse it back
    double parsed = atof(buffer);
    TEST_ASSERT_DOUBLE_WITHIN(0.000000000000001, input, parsed);
}

void test_large_value(void) {
    double const input = 1.0e308;
    char buffer[64] = {0};

    bool success = jstruct_serialize(&f64_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
}

void test_small_value(void) {
    double const input = 1.0e-308;
    char buffer[64] = {0};

    bool success = jstruct_serialize(&f64_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_zero);
    RUN_TEST(test_positive_value);
    RUN_TEST(test_negative_value);
    RUN_TEST(test_large_value);
    RUN_TEST(test_small_value);
    return UNITY_END();
}
