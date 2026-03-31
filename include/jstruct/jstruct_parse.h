/**
 * @file jstruct_parse.h
 * @brief JSON parsing functionality for jstruct library.
 *
 * This file provides functions and types for parsing JSON strings into C data structures
 * based on jstruct value descriptors. It includes detailed error reporting for parse failures.
 */

#ifndef JSTRUCT_PARSE_H__
#define JSTRUCT_PARSE_H__

#include <json-c/json_types.h>

#include "jstruct.h"

/**
 * @brief Enumeration of possible parse result types.
 *
 * These values indicate the outcome of a parse operation, including
 * success and various failure modes with specific error contexts.
 */
typedef enum jstruct_parse_result_type {
    JSTRUCT_PARSE_RESULT_TYPE_PASS,                   /**< Parse succeeded */
    JSTRUCT_PARSE_RESULT_TYPE_JSON_PARSE_FAILED,      /**< JSON string is malformed */
    JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE,               /**< JSON type doesn't match expected C type */
    JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS,          /**< Value is outside valid range */
    JSTRUCT_PARSE_RESULT_TYPE_MISSING_FIELD,          /**< Required object field is missing */
    JSTRUCT_PARSE_RESULT_TYPE_BAD_ARRAY_ELEMENT,      /**< Array element failed to parse */
    JSTRUCT_PARSE_RESULT_TYPE_CUSTOM_PARSE_FAILED,    /**< Custom parser function returned false */
    JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_MEMORY,          /**< Failed to allocate memory for array */
} jstruct_parse_result_type_t;

/**
 * @brief Detailed result information from a parse operation.
 *
 * This structure provides information about parse failures, including
 * the type of error, which field failed (if applicable), and additional
 * context-specific error information.
 */
typedef struct jstruct_parse_result {
    jstruct_parse_result_type_t type;  /**< Type of result (success or error) */
    jstruct_field_t const* field;      /**< Field that failed to parse (NULL if not applicable) */
    union {
        /**< Context for JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE */
        struct {
            json_type expected;  /**< Expected JSON type */
            json_type actual;    /**< Actual JSON type encountered */
        } bad_type;
        /**< Context for JSTRUCT_PARSE_RESULT_TYPE_BAD_ARRAY_ELEMENT */
        struct {
            size_t index;        /**< Index of array element that failed */
        } bad_array_element;
    } context;  /**< Additional error context based on result type */
} jstruct_parse_result_t;

/**
 * @brief Parse a JSON string into a C data structure.
 *
 * This function parses a JSON string according to the provided value descriptor
 * and writes the result to the output buffer. It performs type checking and
 * validation, providing detailed error information on failure.
 *
 * @param value  Descriptor defining the expected structure and types
 * @param json   NULL-terminated JSON string to parse
 * @param out    Pointer to output buffer where parsed data will be written
 * @param result Optional pointer to receive detailed parse result information (may be NULL)
 *
 * @return true if parsing succeeded, false otherwise
 *
 * @note The output buffer must be large enough to hold the parsed data structure.
 * @note If result is NULL, detailed error information will not be available.
 */
bool jstruct_parse(jstruct_value_t const* value, char const* json, void* out, jstruct_parse_result_t* result);

#endif // JSTRUCT_PARSE_H__
