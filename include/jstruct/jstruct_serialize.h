/**
 * @file jstruct_serialize.h
 * @brief JSON serialization functionality for jstruct library.
 *
 * This file provides functions for serializing C data structures into JSON strings
 * based on jstruct value descriptors.
 */

#ifndef JSTRUCT_SERIALIZE_H__
#define JSTRUCT_SERIALIZE_H__

#include <json-c/json_types.h>

#include "jstruct.h"

/**
 * @brief Serialize a C data structure to a JSON string.
 *
 * This function serializes a C data structure to JSON format according to
 * the provided value descriptor, writing the result to the output buffer.
 * The output is a NULL-terminated JSON string.
 *
 * @param value      Descriptor defining the structure and types to serialize
 * @param in         Pointer to input data structure to serialize
 * @param buffer_len Size of the output buffer in bytes
 * @param buffer     Output buffer where JSON string will be written
 *
 * @return true if serialization succeeded, false otherwise
 *
 * @note The buffer must be large enough to hold the entire JSON string plus NULL terminator.
 * @note Returns false if the buffer is too small, or if input/value pointers are NULL.
 */
bool jstruct_serialize(jstruct_value_t const* value, void const* in, size_t buffer_len, char buffer[static buffer_len]);

#endif // JSTRUCT_SERIALIZE_H__