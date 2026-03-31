#include <stdint.h>
#include <stdlib.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_parse.h"

typedef struct {
    size_t length;
    int32_t elements[10];
} int_array_t;

typedef struct {
    size_t length;
    bool elements[5];
} bool_array_t;

typedef struct {
    int32_t x;
    int32_t y;
} point_t;

typedef struct {
    size_t length;
    point_t elements[5];
} point_array_t;

typedef struct {
    size_t length;
    int32_t* elements;
} int_dyn_array_t;

typedef struct {
    size_t length;
    point_t* elements;
} point_dyn_array_t;

typedef struct {
    size_t length;
    int32_t* elements;
    int32_t sentinel;
} int_dyn_array_with_sentinel_t;

static jstruct_value_t int_element_value = {
    .type = JSTRUCT_VALUE_TYPE_I32
};

static jstruct_value_t const int_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &int_element_value,
        .num_elements = 10,
        .element_size = sizeof(int32_t)
    }
};

static jstruct_value_t bool_element_value = {
    .type = JSTRUCT_VALUE_TYPE_BOOLEAN
};

static jstruct_value_t const bool_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &bool_element_value,
        .num_elements = 5,
        .element_size = sizeof(bool)
    }
};

static jstruct_field_t point_fields[] = {
    {
        .offset = offsetof(point_t, x),
        .name = "x",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_I32
        }
    },
    {
        .offset = offsetof(point_t, y),
        .name = "y",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_I32
        }
    }
};

static jstruct_value_t point_value = {
    .type = JSTRUCT_VALUE_TYPE_OBJECT,
    .data.object = {
        .num_fields = 2,
        .fields = point_fields
    }
};

static jstruct_value_t const point_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &point_value,
        .num_elements = 5,
        .element_size = sizeof(point_t)
    }
};

static jstruct_value_t const int_dyn_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &int_element_value,
        .num_elements = 0,
        .element_size = sizeof(int32_t)
    }
};

static jstruct_value_t const point_dyn_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &point_value,
        .num_elements = 0,
        .element_size = sizeof(point_t)
    }
};

void setUp(void) { }
void tearDown(void) { }

void test_empty_array(void) {
    char const* const test_value = "[]";
    int_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(0, out.length);
}

void test_single_element(void) {
    char const* const test_value = "[42]";
    int_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(1, out.length);
    TEST_ASSERT_EQUAL_INT32(42, out.elements[0]);
}

void test_multiple_elements(void) {
    char const* const test_value = "[1, 2, 3, 4, 5]";
    int_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(5, out.length);
    TEST_ASSERT_EQUAL_INT32(1, out.elements[0]);
    TEST_ASSERT_EQUAL_INT32(2, out.elements[1]);
    TEST_ASSERT_EQUAL_INT32(3, out.elements[2]);
    TEST_ASSERT_EQUAL_INT32(4, out.elements[3]);
    TEST_ASSERT_EQUAL_INT32(5, out.elements[4]);
}

void test_max_elements(void) {
    char const* const test_value = "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]";
    int_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(10, out.length);
    for (size_t i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL_INT32(i, out.elements[i]);
    }
}

void test_too_many_elements(void) {
    char const* const test_value = "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]";
    int_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_array_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS, result.type);
}

void test_bool_array(void) {
    char const* const test_value = "[true, false, true]";
    bool_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&bool_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(3, out.length);
    TEST_ASSERT_TRUE(out.elements[0]);
    TEST_ASSERT_FALSE(out.elements[1]);
    TEST_ASSERT_TRUE(out.elements[2]);
}

void test_wrong_element_type(void) {
    char const* const test_value = "[1, 2, \"three\", 4]";
    int_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_array_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_ARRAY_ELEMENT, result.type);
    TEST_ASSERT_EQUAL_size_t(2, result.context.bad_array_element.index);
}

void test_object_array(void) {
    char const* const test_value = "[{\"x\": 1, \"y\": 2}, {\"x\": 3, \"y\": 4}]";
    point_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&point_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(2, out.length);
    TEST_ASSERT_EQUAL_INT32(1, out.elements[0].x);
    TEST_ASSERT_EQUAL_INT32(2, out.elements[0].y);
    TEST_ASSERT_EQUAL_INT32(3, out.elements[1].x);
    TEST_ASSERT_EQUAL_INT32(4, out.elements[1].y);
}

void test_object_array_bad_element(void) {
    char const* const test_value = "[{\"x\": 1, \"y\": 2}, {\"x\": 3}]";
    point_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&point_array_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_ARRAY_ELEMENT, result.type);
    TEST_ASSERT_EQUAL_size_t(1, result.context.bad_array_element.index);
}

void test_dynamic_empty_array(void) {
    char const* const test_value = "[]";
    int_dyn_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_dyn_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(0, out.length);

    free(out.elements);
}

void test_dynamic_single_element(void) {
    char const* const test_value = "[42]";
    int_dyn_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_dyn_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(1, out.length);
    TEST_ASSERT_NOT_NULL(out.elements);
    TEST_ASSERT_EQUAL_INT32(42, out.elements[0]);

    free(out.elements);
}

void test_dynamic_multiple_elements(void) {
    char const* const test_value = "[1, 2, 3, 4, 5]";
    int_dyn_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_dyn_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(5, out.length);
    TEST_ASSERT_NOT_NULL(out.elements);
    TEST_ASSERT_EQUAL_INT32(1, out.elements[0]);
    TEST_ASSERT_EQUAL_INT32(2, out.elements[1]);
    TEST_ASSERT_EQUAL_INT32(3, out.elements[2]);
    TEST_ASSERT_EQUAL_INT32(4, out.elements[3]);
    TEST_ASSERT_EQUAL_INT32(5, out.elements[4]);

    free(out.elements);
}

void test_dynamic_wrong_element_type(void) {
    char const* const test_value = "[1, 2, \"three\", 4]";
    int_dyn_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_dyn_array_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_ARRAY_ELEMENT, result.type);
    TEST_ASSERT_EQUAL_size_t(2, result.context.bad_array_element.index);

    free(out.elements);
}

void test_dynamic_object_array(void) {
    char const* const test_value = "[{\"x\": 1, \"y\": 2}, {\"x\": 3, \"y\": 4}]";
    point_dyn_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&point_dyn_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(2, out.length);
    TEST_ASSERT_NOT_NULL(out.elements);
    TEST_ASSERT_EQUAL_INT32(1, out.elements[0].x);
    TEST_ASSERT_EQUAL_INT32(2, out.elements[0].y);
    TEST_ASSERT_EQUAL_INT32(3, out.elements[1].x);
    TEST_ASSERT_EQUAL_INT32(4, out.elements[1].y);

    free(out.elements);
}

void test_dynamic_array_no_sentinel_trampling(void) {
    char const* const test_value = "[1, 2, 3]";
    int_dyn_array_with_sentinel_t out = {0};
    out.sentinel = 0x12345678;

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&int_dyn_array_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_size_t(3, out.length);
    TEST_ASSERT_NOT_NULL(out.elements);
    TEST_ASSERT_EQUAL_INT32(1, out.elements[0]);
    TEST_ASSERT_EQUAL_INT32(2, out.elements[1]);
    TEST_ASSERT_EQUAL_INT32(3, out.elements[2]);
    TEST_ASSERT_EQUAL_HEX32(0x12345678, out.sentinel);

    free(out.elements);
}

void test_dynamic_object_array_bad_element(void) {
    char const* const test_value = "[{\"x\": 1, \"y\": 2}, {\"x\": 3}]";
    point_dyn_array_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&point_dyn_array_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_ARRAY_ELEMENT, result.type);
    TEST_ASSERT_EQUAL_size_t(1, result.context.bad_array_element.index);

    free(out.elements);
}

void test_expects_array(void) {
    char const* const test_values[] = {
        "true",
        "123",
        "3.14",
        "\"string\"",
        "{}"
    };
    size_t const num_test_values = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < num_test_values; i++) {
        int_array_t out = {0};

        jstruct_parse_result_t result = {0};
        bool const success = jstruct_parse(&int_array_value, test_values[i], &out, &result);

        char message[100];
        sprintf(message, "Test value #%zu: %s", i, test_values[i]);
        TEST_MESSAGE(message);

        TEST_ASSERT_FALSE(success);
        TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE, result.type);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty_array);
    RUN_TEST(test_single_element);
    RUN_TEST(test_multiple_elements);
    RUN_TEST(test_max_elements);
    RUN_TEST(test_too_many_elements);
    RUN_TEST(test_bool_array);
    RUN_TEST(test_wrong_element_type);
    RUN_TEST(test_object_array);
    RUN_TEST(test_object_array_bad_element);
    RUN_TEST(test_expects_array);
    RUN_TEST(test_dynamic_empty_array);
    RUN_TEST(test_dynamic_single_element);
    RUN_TEST(test_dynamic_multiple_elements);
    RUN_TEST(test_dynamic_wrong_element_type);
    RUN_TEST(test_dynamic_object_array);
    RUN_TEST(test_dynamic_object_array_bad_element);
    RUN_TEST(test_dynamic_array_no_sentinel_trampling);
    return UNITY_END();
}
