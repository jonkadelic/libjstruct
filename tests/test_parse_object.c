#include <stdint.h>
#include <unity.h>

#include "jstruct/jstruct.h"
#include "jstruct/jstruct_parse.h"

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

typedef struct {
    int32_t id;
    char name[64];
    bool active;
    int32_t score;
} optional_field_object_t;

static jstruct_field_t optional_field_object_fields[] = {
    {
        .offset = offsetof(optional_field_object_t, id),
        .name = "id",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_I32
        }
    },
    {
        .offset = offsetof(optional_field_object_t, name),
        .name = "name",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_STRING,
            .data.string.buffer_size = 64
        }
    },
    {
        .offset = offsetof(optional_field_object_t, active),
        .name = "active",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_BOOLEAN
        },
        .optional = true
    },
    {
        .offset = offsetof(optional_field_object_t, score),
        .name = "score",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_I32
        },
        .optional = true
    }
};

static jstruct_value_t const optional_field_object_value = {
    .type = JSTRUCT_VALUE_TYPE_OBJECT,
    .data.object = {
        .num_fields = 4,
        .fields = optional_field_object_fields
    }
};

typedef struct {
    int32_t id;
    char name[64];
    bool active;
    bool active_present;
    int32_t score;
    bool score_present;
} presence_object_t;

static jstruct_field_t presence_object_fields[] = {
    {
        .offset = offsetof(presence_object_t, id),
        .name = "id",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_I32
        }
    },
    {
        .offset = offsetof(presence_object_t, name),
        .name = "name",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_STRING,
            .data.string.buffer_size = 64
        }
    },
    {
        .offset = offsetof(presence_object_t, active),
        .name = "active",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_BOOLEAN
        },
        .optional = true,
        .present_offset = offsetof(presence_object_t, active_present)
    },
    {
        .offset = offsetof(presence_object_t, score),
        .name = "score",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_I32
        },
        .optional = true,
        .present_offset = offsetof(presence_object_t, score_present)
    }
};

static jstruct_value_t const presence_object_value = {
    .type = JSTRUCT_VALUE_TYPE_OBJECT,
    .data.object = {
        .num_fields = 4,
        .fields = presence_object_fields
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
    char const* const test_value = "{\"id\": 42, \"name\": \"test\", \"active\": true}";
    simple_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&simple_object_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_INT32(42, out.id);
    TEST_ASSERT_EQUAL_STRING("test", out.name);
    TEST_ASSERT_TRUE(out.active);
}

void test_object_different_field_order(void) {
    char const* const test_value = "{\"active\": false, \"id\": 100, \"name\": \"reordered\"}";
    simple_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&simple_object_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_INT32(100, out.id);
    TEST_ASSERT_EQUAL_STRING("reordered", out.name);
    TEST_ASSERT_FALSE(out.active);
}

void test_missing_field(void) {
    char const* const test_value = "{\"id\": 42, \"name\": \"incomplete\"}";
    simple_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&simple_object_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_MISSING_FIELD, result.type);
    TEST_ASSERT_NOT_NULL(result.field);
    TEST_ASSERT_EQUAL_STRING("active", result.field->name);
}

void test_wrong_field_type(void) {
    char const* const test_value = "{\"id\": \"not a number\", \"name\": \"test\", \"active\": true}";
    simple_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&simple_object_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE, result.type);
}

void test_nested_object(void) {
    char const* const test_value = "{\"position\": {\"x\": 10, \"y\": 20}, \"radius\": 5}";
    circle_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&circle_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_INT32(10, out.position.x);
    TEST_ASSERT_EQUAL_INT32(20, out.position.y);
    TEST_ASSERT_EQUAL_INT32(5, out.radius);
}

void test_nested_object_missing_field(void) {
    char const* const test_value = "{\"position\": {\"x\": 10}, \"radius\": 5}";
    circle_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&circle_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_MISSING_FIELD, result.type);
}

void test_expects_object(void) {
    char const* const test_values[] = {
        "true",
        "123",
        "3.14",
        "\"string\"",
        "[]"
    };
    size_t const num_test_values = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < num_test_values; i++) {
        simple_object_t out = {0};

        jstruct_parse_result_t result = {0};
        bool const success = jstruct_parse(&simple_object_value, test_values[i], &out, &result);

        char message[100];
        sprintf(message, "Test value #%zu: %s", i, test_values[i]);
        TEST_MESSAGE(message);

        TEST_ASSERT_FALSE(success);
        TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE, result.type);
    }
}

void test_optional_field_present(void) {
    char const* const test_value = "{\"id\": 7, \"name\": \"opt\", \"active\": true, \"score\": 99}";
    optional_field_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&optional_field_object_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_INT32(7, out.id);
    TEST_ASSERT_EQUAL_STRING("opt", out.name);
    TEST_ASSERT_TRUE(out.active);
    TEST_ASSERT_EQUAL_INT32(99, out.score);
}

void test_optional_field_absent(void) {
    char const* const test_value = "{\"id\": 7, \"name\": \"opt\"}";
    optional_field_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&optional_field_object_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_PASS, result.type);
    TEST_ASSERT_EQUAL_INT32(7, out.id);
    TEST_ASSERT_EQUAL_STRING("opt", out.name);
}

void test_required_field_absent_fails(void) {
    char const* const test_value = "{\"active\": true, \"score\": 5}";
    optional_field_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&optional_field_object_value, test_value, &out, &result);

    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(JSTRUCT_PARSE_RESULT_TYPE_MISSING_FIELD, result.type);
    TEST_ASSERT_NOT_NULL(result.field);
    TEST_ASSERT_EQUAL_STRING("id", result.field->name);
}

void test_presence_tracking_all_present(void) {
    char const* const test_value = "{\"id\": 1, \"name\": \"p\", \"active\": true, \"score\": 50}";
    presence_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&presence_object_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(out.active_present);
    TEST_ASSERT_TRUE(out.score_present);
    TEST_ASSERT_TRUE(out.active);
    TEST_ASSERT_EQUAL_INT32(50, out.score);
}

void test_presence_tracking_partial(void) {
    char const* const test_value = "{\"id\": 2, \"name\": \"q\", \"score\": 10}";
    presence_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&presence_object_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_FALSE(out.active_present);
    TEST_ASSERT_TRUE(out.score_present);
    TEST_ASSERT_EQUAL_INT32(10, out.score);
}

void test_presence_tracking_none_present(void) {
    char const* const test_value = "{\"id\": 3, \"name\": \"r\"}";
    presence_object_t out = {0};

    jstruct_parse_result_t result = {0};
    bool success = jstruct_parse(&presence_object_value, test_value, &out, &result);

    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_FALSE(out.active_present);
    TEST_ASSERT_FALSE(out.score_present);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_object);
    RUN_TEST(test_object_different_field_order);
    RUN_TEST(test_missing_field);
    RUN_TEST(test_wrong_field_type);
    RUN_TEST(test_nested_object);
    RUN_TEST(test_nested_object_missing_field);
    RUN_TEST(test_expects_object);
    RUN_TEST(test_optional_field_present);
    RUN_TEST(test_optional_field_absent);
    RUN_TEST(test_required_field_absent_fails);
    RUN_TEST(test_presence_tracking_all_present);
    RUN_TEST(test_presence_tracking_partial);
    RUN_TEST(test_presence_tracking_none_present);
    return UNITY_END();
}
