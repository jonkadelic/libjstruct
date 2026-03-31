#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_serialize.h"

#define MAX_ARRAY_SIZE 10

typedef struct {
    size_t length;
    int32_t elements[MAX_ARRAY_SIZE];
} int_array_t;

typedef struct {
    int32_t x;
    int32_t y;
} point_t;

typedef struct {
    size_t length;
    point_t elements[MAX_ARRAY_SIZE];
} point_array_t;

typedef struct {
    size_t length;
    int32_t* elements;
} int_dyn_array_t;

typedef struct {
    size_t length;
    point_t* elements;
} point_dyn_array_t;

static jstruct_value_t i32_element_value = {
    .type = JSTRUCT_VALUE_TYPE_I32
};

static jstruct_value_t const int_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &i32_element_value,
        .num_elements = MAX_ARRAY_SIZE,
        .element_size = sizeof(int32_t)
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

static jstruct_value_t point_element_value = {
    .type = JSTRUCT_VALUE_TYPE_OBJECT,
    .data.object = {
        .num_fields = 2,
        .fields = point_fields
    }
};

static jstruct_value_t const point_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &point_element_value,
        .num_elements = MAX_ARRAY_SIZE,
        .element_size = sizeof(point_t)
    }
};

static jstruct_value_t const int_dyn_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &i32_element_value,
        .num_elements = 0,
        .element_size = sizeof(int32_t)
    }
};

static jstruct_value_t const point_dyn_array_value = {
    .type = JSTRUCT_VALUE_TYPE_ARRAY,
    .data.array = {
        .value_type = &point_element_value,
        .num_elements = 0,
        .element_size = sizeof(point_t)
    }
};

void setUp(void) { }
void tearDown(void) { }

void test_empty_array(void) {
    int_array_t const input = {
        .length = 0
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&int_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    // json-c may format empty array as "[]" or "[ ]"
    TEST_ASSERT_TRUE(strstr(buffer, "[") != nullptr && strstr(buffer, "]") != nullptr);
}

void test_single_element(void) {
    int_array_t const input = {
        .length = 1,
        .elements = {42}
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&int_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    TEST_ASSERT_TRUE(strstr(buffer, "42") != nullptr);
    TEST_ASSERT_TRUE(buffer[0] == '[');
    TEST_ASSERT_TRUE(buffer[strlen(buffer) - 1] == ']');
}

void test_multiple_elements(void) {
    int_array_t const input = {
        .length = 5,
        .elements = {1, 2, 3, 4, 5}
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&int_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    TEST_ASSERT_TRUE(strstr(buffer, "1") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "2") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "3") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "4") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "5") != nullptr);
}

void test_array_of_objects(void) {
    point_array_t const input = {
        .length = 3,
        .elements = {
            {.x = 1, .y = 2},
            {.x = 3, .y = 4},
            {.x = 5, .y = 6}
        }
    };
    char buffer[512] = {0};

    bool success = jstruct_serialize(&point_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    // Check for nested object structure
    TEST_ASSERT_TRUE(strstr(buffer, "\"x\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"y\"") != nullptr);
}

void test_dynamic_empty_array(void) {
    int_dyn_array_t const input = {
        .length = 0,
        .elements = nullptr
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&int_dyn_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(strstr(buffer, "[") != nullptr && strstr(buffer, "]") != nullptr);
}

void test_dynamic_single_element(void) {
    int32_t elements[] = {42};
    int_dyn_array_t const input = {
        .length = 1,
        .elements = elements
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&int_dyn_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(strstr(buffer, "42") != nullptr);
    TEST_ASSERT_TRUE(buffer[0] == '[');
    TEST_ASSERT_TRUE(buffer[strlen(buffer) - 1] == ']');
}

void test_dynamic_multiple_elements(void) {
    int32_t elements[] = {10, 20, 30, 40, 50};
    int_dyn_array_t const input = {
        .length = 5,
        .elements = elements
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&int_dyn_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(strstr(buffer, "10") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "20") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "30") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "40") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "50") != nullptr);
    TEST_ASSERT_TRUE(buffer[0] == '[');
    TEST_ASSERT_TRUE(buffer[strlen(buffer) - 1] == ']');
}

void test_dynamic_array_of_objects(void) {
    point_t elements[] = {
        {.x = 1, .y = 2},
        {.x = 3, .y = 4}
    };
    point_dyn_array_t const input = {
        .length = 2,
        .elements = elements
    };
    char buffer[512] = {0};

    bool success = jstruct_serialize(&point_dyn_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(strstr(buffer, "\"x\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"y\"") != nullptr);
    TEST_ASSERT_TRUE(buffer[0] == '[');
    TEST_ASSERT_TRUE(buffer[strlen(buffer) - 1] == ']');
}

void test_full_array(void) {
    int_array_t input = {
        .length = MAX_ARRAY_SIZE
    };
    for (size_t i = 0; i < MAX_ARRAY_SIZE; i++) {
        input.elements[i] = (int32_t)i * 10;
    }
    char buffer[512] = {0};

    bool success = jstruct_serialize(&int_array_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty_array);
    RUN_TEST(test_single_element);
    RUN_TEST(test_multiple_elements);
    RUN_TEST(test_array_of_objects);
    RUN_TEST(test_full_array);
    RUN_TEST(test_dynamic_empty_array);
    RUN_TEST(test_dynamic_single_element);
    RUN_TEST(test_dynamic_multiple_elements);
    RUN_TEST(test_dynamic_array_of_objects);
    return UNITY_END();
}
