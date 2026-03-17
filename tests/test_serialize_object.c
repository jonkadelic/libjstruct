#include <stdint.h>
#include <string.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_serialize.h"

typedef struct {
    int32_t id;
    char name[64];
    bool active;
} simple_object_t;

typedef struct {
    int32_t x;
    int32_t y;
} point_t;

typedef struct {
    point_t position;
    int32_t radius;
} circle_t;

static jstruct_field_t simple_object_fields[] = {
    {
        .offset = offsetof(simple_object_t, id),
        .name = "id",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_I32
        }
    },
    {
        .offset = offsetof(simple_object_t, name),
        .name = "name",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_STRING,
            .data.string.buffer_size = 64
        }
    },
    {
        .offset = offsetof(simple_object_t, active),
        .name = "active",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_BOOLEAN
        }
    }
};

static jstruct_value_t const simple_object_value = {
    .type = JSTRUCT_VALUE_TYPE_OBJECT,
    .data.object = {
        .num_fields = 3,
        .fields = simple_object_fields
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

static jstruct_field_t circle_fields[] = {
    {
        .offset = offsetof(circle_t, position),
        .name = "position",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_OBJECT,
            .data.object = {
                .num_fields = 2,
                .fields = point_fields
            }
        }
    },
    {
        .offset = offsetof(circle_t, radius),
        .name = "radius",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_I32
        }
    }
};

static jstruct_value_t const circle_value = {
    .type = JSTRUCT_VALUE_TYPE_OBJECT,
    .data.object = {
        .num_fields = 2,
        .fields = circle_fields
    }
};

void setUp(void) { }
void tearDown(void) { }

void test_simple_object(void) {
    simple_object_t const input = {
        .id = 42,
        .name = "test",
        .active = true
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&simple_object_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    // Check that all fields are present
    TEST_ASSERT_TRUE(strstr(buffer, "\"id\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "42") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"name\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"test\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"active\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "true") != nullptr);
}

void test_nested_object(void) {
    circle_t const input = {
        .position = {
            .x = 10,
            .y = 20
        },
        .radius = 5
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&circle_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    // Check that nested fields are present
    TEST_ASSERT_TRUE(strstr(buffer, "\"position\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"x\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "10") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"y\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "20") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"radius\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "5") != nullptr);
}

void test_object_with_empty_string(void) {
    simple_object_t const input = {
        .id = 1,
        .name = "",
        .active = false
    };
    char buffer[256] = {0};

    bool success = jstruct_serialize(&simple_object_value, &input, sizeof(buffer), buffer);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_NOT_EQUAL(0, strlen(buffer));
    TEST_ASSERT_TRUE(strstr(buffer, "\"name\"") != nullptr);
    TEST_ASSERT_TRUE(strstr(buffer, "\"\"") != nullptr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_object);
    RUN_TEST(test_nested_object);
    RUN_TEST(test_object_with_empty_string);
    return UNITY_END();
}
