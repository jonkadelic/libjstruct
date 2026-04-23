/**
 * @file jstruct.h
 * @brief Core data structures and types for jstruct JSON mapping library.
 *
 * This file defines the fundamental types used to describe the structure of C data types
 * for JSON parsing and serialization. It provides a declarative way to map between
 * C structs and JSON objects.
 */

#ifndef JSTRUCT_H__
#define JSTRUCT_H__

#include <stddef.h>

#include <json-c/json.h>

typedef struct jstruct_field jstruct_field_t;
typedef struct jstruct_value jstruct_value_t;

/**
 * @brief Enumeration of supported value types for JSON mapping.
 *
 * These types correspond to both JSON types and C primitive types,
 * allowing flexible mapping between JSON and C data structures.
 */
typedef enum jstruct_value_type {
    JSTRUCT_VALUE_TYPE_BOOLEAN,  /**< Boolean type (bool) */
    JSTRUCT_VALUE_TYPE_U8,       /**< Unsigned 8-bit integer (uint8_t) */
    JSTRUCT_VALUE_TYPE_U16,      /**< Unsigned 16-bit integer (uint16_t) */
    JSTRUCT_VALUE_TYPE_U32,      /**< Unsigned 32-bit integer (uint32_t) */
    JSTRUCT_VALUE_TYPE_U64,      /**< Unsigned 64-bit integer (uint64_t) */
    JSTRUCT_VALUE_TYPE_USIZE,    /**< Unsigned size type (size_t) */
    JSTRUCT_VALUE_TYPE_I8,       /**< Signed 8-bit integer (int8_t) */
    JSTRUCT_VALUE_TYPE_I16,      /**< Signed 16-bit integer (int16_t) */
    JSTRUCT_VALUE_TYPE_I32,      /**< Signed 32-bit integer (int32_t) */
    JSTRUCT_VALUE_TYPE_I64,      /**< Signed 64-bit integer (int64_t) */
    JSTRUCT_VALUE_TYPE_F32,      /**< 32-bit floating point (float) */
    JSTRUCT_VALUE_TYPE_F64,      /**< 64-bit floating point (double) */
    JSTRUCT_VALUE_TYPE_OBJECT,   /**< Nested object (struct) */
    JSTRUCT_VALUE_TYPE_ARRAY,    /**< Array of values */
    JSTRUCT_VALUE_TYPE_STRING,   /**< Null-terminated string (char[]) */
    JSTRUCT_VALUE_TYPE_CUSTOM    /**< Custom type with user-defined parser/serializer */
} jstruct_value_type_t;

/**
 * @brief Describes the structure and type of a value for JSON mapping.
 *
 * This structure defines how a particular C type should be interpreted
 * when parsing from or serializing to JSON. The type field determines
 * which member of the data union is valid.
 */
typedef struct jstruct_value {
    jstruct_value_type_t type;  /**< The type of this value */
    union {
        /**< Configuration for JSTRUCT_VALUE_TYPE_OBJECT */
        struct {
            size_t num_fields;           /**< Number of fields in the object */
            jstruct_field_t* fields;     /**< Array of field descriptors */
        } object;
        /**< Configuration for JSTRUCT_VALUE_TYPE_ARRAY */
        struct {
            jstruct_value_t* value_type; /**< Type of elements in the array */
            size_t num_elements;         /**< Maximum number of elements. Allocated dynamically if 0. */
            size_t element_size;         /**< Size of each element in bytes */
        } array;
        /**< Configuration for JSTRUCT_VALUE_TYPE_STRING */
        struct {
            size_t buffer_size;          /**< Size of the string buffer including null terminator */
        } string;
        /**< Configuration for JSTRUCT_VALUE_TYPE_CUSTOM */
        struct {
            bool (*parser)(void* field, struct json_object const* object);        /**< Custom parser function */
            bool (*serializer)(void const* field, struct json_object** object);   /**< Custom serializer function */
        } custom;
    } data;  /**< Type-specific configuration data */
} jstruct_value_t;

/**
 * @brief Describes a field within a struct for JSON object mapping.
 *
 * This structure associates a JSON object field name with a member
 * of a C struct, specifying the offset and type information needed
 * for parsing and serialization.
 */
typedef struct jstruct_field {
    size_t offset;           /**< Offset of the field within the parent struct (use offsetof) */
    char name[256];          /**< JSON field name */
    jstruct_value_t value;   /**< Type descriptor for this field */
    bool optional;           /**< If true, the field may be absent from the JSON object without error */
    size_t present_offset;   /**< Offset of a bool member that receives presence status. Set to 0 to disable (default). Must be non-zero to enable. */
} jstruct_field_t;

#endif // JSTRUCT_H__