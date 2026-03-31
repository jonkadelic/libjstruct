#include "jstruct/jstruct_serialize.h"
#include <json-c/json_object.h>
#include <string.h>

typedef union in_union {
    void const* any;
    bool const* boolean;
    uint8_t const* u8;
    uint16_t const* u16;
    uint32_t const* u32;
    uint64_t const* u64;
    size_t const* usize;
    int8_t const* i8;
    int16_t const* i16;
    int32_t const* i32;
    int64_t const* i64;
    float const* f32;
    double const* f64;
    char const* string;
} in_union_t;

typedef bool (*fn_serializer_t)(jstruct_value_t const*, json_object**, in_union_t);

#define SERIALIZERS_LIST \
    X(BOOLEAN, NONE) \
    X(U8, UNUMERIC(u8)) \
    X(U16, UNUMERIC(u16)) \
    X(U32, UNUMERIC(u32)) \
    X(U64, UNUMERIC(u64)) \
    X(USIZE, UNUMERIC(usize)) \
    X(I8, INUMERIC(i8)) \
    X(I16, INUMERIC(i16)) \
    X(I32, INUMERIC(i32)) \
    X(I64, INUMERIC(i64)) \
    X(F32, FP(f32)) \
    X(F64, FP(f64)) \
    X(OBJECT, NONE) \
    X(ARRAY, NONE) \
    X(STRING, NONE) \
    X(CUSTOM, NONE)

#define X(name, _) static bool serialize_##name(jstruct_value_t const*, json_object**, in_union_t);
SERIALIZERS_LIST
#undef X

static fn_serializer_t const SERIALIZERS[] = {
#define X(name, _) [JSTRUCT_VALUE_TYPE_##name] = serialize_##name,
    SERIALIZERS_LIST
#undef X
};

bool jstruct_serialize(jstruct_value_t const* value, void const* in, size_t buffer_len, char buffer[static buffer_len]) {
    if (value == nullptr || in == nullptr || buffer_len == 0) {
        return false;
    }

    fn_serializer_t const serializer_fn = SERIALIZERS[value->type];

    json_object* object = nullptr;
    bool const success = serializer_fn(value, &object, (in_union_t) { .any = in });
    if (!success) {
        return false;
    }

    char const* const string = json_object_to_json_string(object);
    if (string == nullptr) {
        json_object_put(object);
        return false;
    }

    if (strlen(string) > buffer_len - 1) {
        json_object_put(object);
        return false;
    }

    strncpy(buffer, string, buffer_len);

    json_object_put(object);

    return true;
}

#define X(name, impl) static bool serialize_##name(jstruct_value_t const* value, json_object** object, in_union_t in) impl
#define NONE ;
#define UNUMERIC(name) { \
    *object = json_object_new_uint64(*in.name); \
    return true; \
}
#define INUMERIC(name) { \
    *object = json_object_new_int64(*in.name); \
    return true; \
}
#define FP(name) { \
    *object = json_object_new_double(*in.name); \
    return true; \
}

SERIALIZERS_LIST

#undef INUMERIC
#undef UNUMERIC
#undef NONE
#undef X

static bool serialize_BOOLEAN(jstruct_value_t const* value, json_object** object, in_union_t in) {
    *object = json_object_new_boolean(*in.boolean);
    return true;
}

static bool serialize_OBJECT(jstruct_value_t const* value, json_object** object, in_union_t in) {
    *object = json_object_new_object();

    for (size_t i = 0; i < value->data.object.num_fields; i++) {
        jstruct_field_t const* const child_field = &value->data.object.fields[i];
        fn_serializer_t const serializer_fn = SERIALIZERS[child_field->value.type];
        in_union_t const child_in = (in_union_t) { .any = ((uint8_t const*) in.any) + child_field->offset };

        json_object* child_object = nullptr;
        if (!serializer_fn(&child_field->value, &child_object, child_in)) {
            json_object_put(*object);
            return false;
        }

        if (json_object_object_add(*object, child_field->name, child_object) != 0) {
            json_object_put(child_object);
            json_object_put(*object);
            return false;
        }
    }

    return true;
}

static bool serialize_ARRAY(jstruct_value_t const* value, json_object** object, in_union_t in) {
    *object = json_object_new_array();

    size_t const array_len = *in.usize;

    fn_serializer_t const serializer_fn = SERIALIZERS[value->data.array.value_type->type];

    void const* array_ptr;
    if (value->data.array.num_elements == 0) {
        void const* const* array_ptr_ptr = (void const* const*) (((uint8_t const*) in.any) + sizeof(size_t));
        array_ptr = *array_ptr_ptr;
    } else {
        array_ptr = ((uint8_t const*) in.any) + sizeof(size_t);
    }

    void const* offset_ptr = array_ptr;
    for (size_t i = 0; i < array_len; i++) {
        json_object* child_object = nullptr;

        bool const success = serializer_fn(value->data.array.value_type, &child_object, (in_union_t) { .any = offset_ptr });
        if (!success) {
            json_object_put(*object);
            return false;
        }

        json_object_array_add(*object, child_object);

        offset_ptr = ((uint8_t const*) offset_ptr) + value->data.array.element_size;
    }

    return true;
}

static bool serialize_STRING(jstruct_value_t const* value, json_object** object, in_union_t in) {
    *object = json_object_new_string(in.string);
    return true;
}

static bool serialize_CUSTOM(jstruct_value_t const* value, json_object** object, in_union_t in) {
    if (!value->data.custom.serializer(in.any, object)) {
        return false;
    }

    return true;
}
