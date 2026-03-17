#include <string.h>

#include <json-c/json_object.h>
#include <json-c/json_types.h>

#include "jstruct/jstruct_parse.h"
#include "jstruct/jstruct.h"

typedef union out_union {
    void* any;
    bool* boolean;
    uint8_t* u8;
    uint16_t* u16;
    uint32_t* u32;
    uint64_t* u64;
    size_t* usize;
    int8_t* i8;
    int16_t* i16;
    int32_t* i32;
    int64_t* i64;
    float* f32;
    double* f64;
    char* string;
} out_union_t;

typedef bool (*fn_parser_t)(json_object const*, jstruct_value_t const*, out_union_t, jstruct_parse_result_t*);

#define PARSERS_LIST \
    X(BOOLEAN, NONE) \
    X(U8, UNUMERIC(u8, UINT8)) \
    X(U16, UNUMERIC(u16, UINT16)) \
    X(U32, UNUMERIC(u32, UINT32)) \
    X(U64, NONE) \
    X(USIZE, UNUMERIC(usize, SIZE)) \
    X(I8, INUMERIC(i8, INT8)) \
    X(I16, INUMERIC(i16, INT16)) \
    X(I32, INUMERIC(i32, INT32)) \
    X(I64, NONE) \
    X(F32, FP(f32)) \
    X(F64, FP(f64)) \
    X(OBJECT, NONE) \
    X(ARRAY, NONE) \
    X(STRING, NONE) \
    X(CUSTOM, NONE)

#define X(name, _) static bool parse_##name(json_object const*, jstruct_value_t const*, out_union_t, jstruct_parse_result_t*);
PARSERS_LIST
#undef X

static fn_parser_t const PARSERS[] = {
#define X(name, _) [JSTRUCT_VALUE_TYPE_##name] = parse_##name,
    PARSERS_LIST
#undef X
};

bool jstruct_parse(jstruct_value_t const* value, char const* json, void* out, jstruct_parse_result_t* result) {
    if (value == nullptr || json == nullptr || out == nullptr) {
        return false;
    }

    fn_parser_t const parser_fn = PARSERS[value->type];

    json_object* object = json_tokener_parse(json);
    if (object == nullptr) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_JSON_PARSE_FAILED;
        }
        return false;
    }

    bool const success = parser_fn(object, value, (out_union_t) { .any = out }, result);

    json_object_put(object);

    if (!success) {
        return false;
    }

    return true;
}

#define X(name, impl) static bool parse_##name(json_object const* object, jstruct_value_t const* value, out_union_t out, jstruct_parse_result_t* result) impl
#define NONE ;
#define UNUMERIC(name, type_upper) { \
    if (json_object_get_type(object) != json_type_int) { \
        if (result != nullptr) { \
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE; \
            result->context.bad_type.expected = json_type_int; \
            result->context.bad_type.actual = json_object_get_type(object); \
        } \
        \
        return false; \
    } \
    \
    int64_t const obj_val = json_object_get_int64(object); \
    if (obj_val < 0 || obj_val > type_upper##_MAX) { \
        if (result != nullptr) { \
            result->type = JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS; \
        } \
        \
        return false; \
    } \
    \
    *out.name = (typeof(*out.name)) obj_val; \
    \
    return true; \
}
#define INUMERIC(name, type_upper) { \
    if (json_object_get_type(object) != json_type_int) { \
        if (result != nullptr) { \
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE; \
            result->context.bad_type.expected = json_type_int; \
            result->context.bad_type.actual = json_object_get_type(object); \
        } \
        \
        return false; \
    } \
    \
    int64_t const obj_val = json_object_get_int64(object); \
    if (obj_val < type_upper##_MIN || obj_val > type_upper##_MAX) { \
        if (result != nullptr) { \
            result->type = JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS; \
        } \
        \
        return false; \
    } \
    \
    *out.name = (typeof(*out.name)) obj_val; \
    \
    return true; \
}
#define FP(name) { \
    json_type const type = json_object_get_type(object); \
    if (type != json_type_double && type != json_type_int) { \
        if (result != nullptr) { \
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE; \
            result->context.bad_type.expected = json_type_double; \
            result->context.bad_type.actual = json_object_get_type(object); \
        } \
        \
        return false; \
    } \
    \
    double const obj_val = json_object_get_double(object); \
    \
    *out.name = (typeof(*out.name)) obj_val; \
    \
    return true; \
}

PARSERS_LIST

#undef FP
#undef INUMERIC
#undef UNUMERIC
#undef NONE
#undef X

static bool parse_BOOLEAN(json_object const* object, jstruct_value_t const* value, out_union_t out, jstruct_parse_result_t* result) {
    if (json_object_get_type(object) != json_type_boolean) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE;
            result->context.bad_type.expected = json_type_boolean;
            result->context.bad_type.actual = json_object_get_type(object);
        }

        return false;
    }

    *out.boolean = json_object_get_boolean(object);

    return true;
}

static bool parse_U64(json_object const* object, jstruct_value_t const* value, out_union_t out, jstruct_parse_result_t* result) {
    if (json_object_get_type(object) != json_type_int) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE;
            result->context.bad_type.expected = json_type_int;
            result->context.bad_type.actual = json_object_get_type(object);
        }

        return false;
    }

    uint64_t const uvalue = json_object_get_uint64(object);
    int64_t const ivalue = json_object_get_int64(object);

    uint64_t obj_val;
    if (uvalue == ivalue || (uvalue != ivalue && ivalue == INT64_MAX)) {
        obj_val = uvalue;
    } else {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS;
        }

        return false;
    }

    *out.u64 = obj_val;

    return true;
}

static bool parse_I64(json_object const* object, jstruct_value_t const* value, out_union_t out, jstruct_parse_result_t* result) {
    if (json_object_get_type(object) != json_type_int) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE;
            result->context.bad_type.expected = json_type_int;
            result->context.bad_type.actual = json_object_get_type(object);
        }

        return false;
    }

    uint64_t const uvalue = json_object_get_uint64(object);
    int64_t const ivalue = json_object_get_int64(object);

    int64_t obj_val;
    if (uvalue == ivalue || (uvalue != ivalue && uvalue == 0)) {
        obj_val = ivalue;
    } else {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS;
        }

        return false;
    }

    *out.i64 = obj_val;

    return true;
}

static bool parse_OBJECT(json_object const* object, jstruct_value_t const* value, out_union_t out, jstruct_parse_result_t* result) {
    if (json_object_get_type(object) != json_type_object) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE;
            result->context.bad_type.expected = json_type_object;
            result->context.bad_type.actual = json_object_get_type(object);
        }

        return false;
    }

    for (size_t i = 0; i < value->data.object.num_fields; i++) {
        jstruct_field_t const* const child_field = &value->data.object.fields[i];
        fn_parser_t const parser_fn = PARSERS[child_field->value.type];
        out_union_t child_out = (out_union_t) { .any = ((uint8_t*) out.any) + child_field->offset };

        json_object* child_object = nullptr;
        if (json_object_object_get_ex(object, child_field->name, &child_object)) {
            bool const success = parser_fn(child_object, &child_field->value, child_out, result);
            if (!success) {
                if (result != nullptr && result->field == nullptr) {
                    result->field = child_field;
                }

                return false;
            }
        } else {
            if (result != nullptr) {
                result->type = JSTRUCT_PARSE_RESULT_TYPE_MISSING_FIELD;
                result->field = child_field;
            }

            return false;
        }
    }

    return true;
}

static bool parse_ARRAY(json_object const* object, jstruct_value_t const* value, out_union_t out, jstruct_parse_result_t* result) {
    if (json_object_get_type(object) != json_type_array) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE;
            result->context.bad_type.expected = json_type_array;
            result->context.bad_type.actual = json_object_get_type(object);
        }

        return false;
    }

    size_t const array_len = json_object_array_length(object);

    if (array_len > value->data.array.num_elements) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS;
        }

        return false;        
    }

    size_t* const out_array_len = out.usize;
    *out_array_len = array_len;

    fn_parser_t const parser_fn = PARSERS[value->data.array.value_type->type];

    void* offset_ptr = ((uint8_t*) out.any) + sizeof(size_t);
    for (size_t i = 0; i < array_len; i++) {
        json_object const* const child_object = json_object_array_get_idx(object, i);

        bool const success = parser_fn(child_object, value->data.array.value_type, (out_union_t) { .any = offset_ptr }, result);
        if (!success) {
            if (result != nullptr) {
                result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_ARRAY_ELEMENT;
                result->context.bad_array_element.index = i;
            }

            return false;
        }

        offset_ptr = ((uint8_t*) offset_ptr) + value->data.array.element_size;
    }

    return true;
}

static bool parse_STRING(json_object const* object, jstruct_value_t const* value, out_union_t out, jstruct_parse_result_t* result) {
    if (json_object_get_type(object) != json_type_string) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE;
            result->context.bad_type.expected = json_type_string;
            result->context.bad_type.actual = json_object_get_type(object);
        }

        return false;
    }

    size_t const string_len = json_object_get_string_len(object);
    if (string_len + 1 > value->data.string.buffer_size) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS;
        }

        return false;
    }

    char const* const string = json_object_get_string((void*) object);

    strncpy(out.string, string, value->data.string.buffer_size);

    return true;
}

static bool parse_CUSTOM(json_object const* object, jstruct_value_t const* value, out_union_t out, jstruct_parse_result_t* result) {
    if (!value->data.custom.parser(out.any, object)) {
        if (result != nullptr) {
            result->type = JSTRUCT_PARSE_RESULT_TYPE_CUSTOM_PARSE_FAILED;
        }
        
        return false;
    }

    return true;
}
