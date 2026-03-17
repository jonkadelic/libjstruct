# libjstruct

A C library for declarative JSON parsing and serialization using compile-time type descriptors.

## Overview

libjstruct provides a structured approach to mapping between C data structures and JSON. Instead of writing custom parsing and serialization code for each type, you define the structure once using descriptors, and the library handles the conversion automatically.

The library performs type checking, bounds validation, and provides detailed error reporting for parse failures.

## Features

- Declarative type mapping between C structs and JSON
- Support for primitive types (booleans, integers, floats)
- Support for complex types (strings, objects, arrays)
- Custom type handlers for specialized parsing/serialization
- Comprehensive error reporting with context information
- Zero runtime overhead for type descriptors
- Built on json-c for robust JSON handling

## Requirements

- C23 compiler (GCC 15.2.1 or later, Clang with C23 support)
- CMake 3.21 or later
- json-c library
- Unity test framework (automatically fetched during build)

## Building

```bash
cmake -B build
cmake --build build
```

To run tests:

```bash
cd build
ctest --output-on-failure
```

## Installation

```bash
cmake -B build
cmake --build build
sudo cmake --install build
```

## Usage

### Basic Parsing Example

```c
#include <jstruct/jstruct.h>
#include <jstruct/jstruct_parse.h>

typedef struct {
    int32_t id;
    char name[64];
    bool active;
} user_t;

// Define the structure descriptor
static jstruct_field_t user_fields[] = {
    {
        .offset = offsetof(user_t, id),
        .name = "id",
        .value = { .type = JSTRUCT_VALUE_TYPE_I32 }
    },
    {
        .offset = offsetof(user_t, name),
        .name = "name",
        .value = {
            .type = JSTRUCT_VALUE_TYPE_STRING,
            .data.string.buffer_size = 64
        }
    },
    {
        .offset = offsetof(user_t, active),
        .name = "active",
        .value = { .type = JSTRUCT_VALUE_TYPE_BOOLEAN }
    }
};

static jstruct_value_t const user_value = {
    .type = JSTRUCT_VALUE_TYPE_OBJECT,
    .data.object = {
        .num_fields = 3,
        .fields = user_fields
    }
};

// Parse JSON into C struct
user_t user;
jstruct_parse_result_t result = {0};
const char* json = "{\"id\": 42, \"name\": \"Alice\", \"active\": true}";

if (jstruct_parse(&user_value, json, &user, &result)) {
    printf("User: id=%d, name=%s, active=%d\n", 
           user.id, user.name, user.active);
} else {
    printf("Parse failed: %d\n", result.type);
}
```

### Basic Serialization Example

```c
#include <jstruct/jstruct.h>
#include <jstruct/jstruct_serialize.h>

user_t user = {
    .id = 42,
    .name = "Alice",
    .active = true
};

char buffer[256];
if (jstruct_serialize(&user_value, &user, sizeof(buffer), buffer)) {
    printf("JSON: %s\n", buffer);
}
```

### Nested Objects

```c
typedef struct {
    int32_t x;
    int32_t y;
} point_t;

typedef struct {
    point_t position;
    int32_t radius;
} circle_t;

static jstruct_field_t point_fields[] = {
    {
        .offset = offsetof(point_t, x),
        .name = "x",
        .value = { .type = JSTRUCT_VALUE_TYPE_I32 }
    },
    {
        .offset = offsetof(point_t, y),
        .name = "y",
        .value = { .type = JSTRUCT_VALUE_TYPE_I32 }
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
        .value = { .type = JSTRUCT_VALUE_TYPE_I32 }
    }
};

static jstruct_value_t const circle_value = {
    .type = JSTRUCT_VALUE_TYPE_OBJECT,
    .data.object = {
        .num_fields = 2,
        .fields = circle_fields
    }
};
```

### Arrays

```c
#define MAX_POINTS 10

typedef struct {
    size_t length;
    point_t elements[MAX_POINTS];
} point_array_t;

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
        .num_elements = MAX_POINTS,
        .element_size = sizeof(point_t)
    }
};
```

## Supported Types

| Type | Description | C Type |
|------|-------------|--------|
| `JSTRUCT_VALUE_TYPE_BOOLEAN` | Boolean | `bool` |
| `JSTRUCT_VALUE_TYPE_U8` | Unsigned 8-bit integer | `uint8_t` |
| `JSTRUCT_VALUE_TYPE_U16` | Unsigned 16-bit integer | `uint16_t` |
| `JSTRUCT_VALUE_TYPE_U32` | Unsigned 32-bit integer | `uint32_t` |
| `JSTRUCT_VALUE_TYPE_U64` | Unsigned 64-bit integer | `uint64_t` |
| `JSTRUCT_VALUE_TYPE_USIZE` | Unsigned size type | `size_t` |
| `JSTRUCT_VALUE_TYPE_I8` | Signed 8-bit integer | `int8_t` |
| `JSTRUCT_VALUE_TYPE_I16` | Signed 16-bit integer | `int16_t` |
| `JSTRUCT_VALUE_TYPE_I32` | Signed 32-bit integer | `int32_t` |
| `JSTRUCT_VALUE_TYPE_I64` | Signed 64-bit integer | `int64_t` |
| `JSTRUCT_VALUE_TYPE_F32` | 32-bit floating point | `float` |
| `JSTRUCT_VALUE_TYPE_F64` | 64-bit floating point | `double` |
| `JSTRUCT_VALUE_TYPE_STRING` | Null-terminated string | `char[]` |
| `JSTRUCT_VALUE_TYPE_OBJECT` | Nested object | `struct` |
| `JSTRUCT_VALUE_TYPE_ARRAY` | Array of values | Array |
| `JSTRUCT_VALUE_TYPE_CUSTOM` | Custom type | User-defined |

## Error Handling

The library provides detailed parse error information through the `jstruct_parse_result_t` structure:

```c
jstruct_parse_result_t result = {0};
if (!jstruct_parse(&value, json, &output, &result)) {
    switch (result.type) {
        case JSTRUCT_PARSE_RESULT_TYPE_JSON_PARSE_FAILED:
            printf("Invalid JSON syntax\n");
            break;
        case JSTRUCT_PARSE_RESULT_TYPE_BAD_TYPE:
            printf("Type mismatch: expected %d, got %d\n",
                   result.context.bad_type.expected,
                   result.context.bad_type.actual);
            break;
        case JSTRUCT_PARSE_RESULT_TYPE_OUT_OF_BOUNDS:
            printf("Value out of bounds\n");
            break;
        case JSTRUCT_PARSE_RESULT_TYPE_MISSING_FIELD:
            printf("Missing field: %s\n", result.field->name);
            break;
        case JSTRUCT_PARSE_RESULT_TYPE_BAD_ARRAY_ELEMENT:
            printf("Array element %zu failed to parse\n",
                   result.context.bad_array_element.index);
            break;
    }
}
```

## API Reference

### Core Functions

#### `jstruct_parse`
```c
bool jstruct_parse(jstruct_value_t const* value, 
                   char const* json, 
                   void* out, 
                   jstruct_parse_result_t* result);
```
Parse a JSON string into a C data structure.

**Parameters:**
- `value`: Descriptor defining the expected structure and types
- `json`: NULL-terminated JSON string to parse
- `out`: Pointer to output buffer where parsed data will be written
- `result`: Optional pointer to receive detailed parse result information (may be NULL)

**Returns:** `true` if parsing succeeded, `false` otherwise

#### `jstruct_serialize`
```c
bool jstruct_serialize(jstruct_value_t const* value, 
                       void const* in, 
                       size_t buffer_len, 
                       char buffer[static buffer_len]);
```
Serialize a C data structure to a JSON string.

**Parameters:**
- `value`: Descriptor defining the structure and types to serialize
- `in`: Pointer to input data structure to serialize
- `buffer_len`: Size of the output buffer in bytes
- `buffer`: Output buffer where JSON string will be written

**Returns:** `true` if serialization succeeded, `false` otherwise

## Testing

The library includes comprehensive test coverage with 20 test suites covering:
- Primitive type parsing and serialization (booleans, integers, floats)
- String handling with special characters
- Nested objects
- Arrays of primitives and objects
- Error conditions and edge cases

Run tests with:
```bash
cd build
ctest --output-on-failure
```

## Project Structure

```
libjstruct/
├── include/jstruct/      # Public headers
│   ├── jstruct.h         # Core types and structures
│   ├── jstruct_parse.h   # Parsing API
│   └── jstruct_serialize.h # Serialization API
├── src/                  # Implementation
│   ├── parse.c           # Parsing implementation
│   └── serialize.c       # Serialization implementation
├── tests/                # Unit tests
└── CMakeLists.txt        # Build configuration
```

## Contributing

When contributing to this project:
- Follow the existing code style
- Add tests for new features
- Update documentation as needed
- Ensure all tests pass before submitting

## Requirements for Development

- C23 compiler
- CMake 3.21+
- json-c development headers
- clangd (optional, for IDE support)
