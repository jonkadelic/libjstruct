# Fetch Unity testing framework
include(FetchContent)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.0
)
FetchContent_MakeAvailable(unity)

# Enable double precision support in Unity
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_DOUBLE)

# Helper function to add tests
function(add_jstruct_test test_name)
    add_executable(${test_name} ${ARGN})
    target_link_libraries(${test_name}
        PRIVATE
            jstruct
            unity
    )
    target_include_directories(${test_name}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Add test executables here
add_jstruct_test(test_parse_boolean test_parse_boolean.c)
add_jstruct_test(test_parse_u8 test_parse_u8.c)
add_jstruct_test(test_parse_u16 test_parse_u16.c)
add_jstruct_test(test_parse_u32 test_parse_u32.c)
add_jstruct_test(test_parse_u64 test_parse_u64.c)
add_jstruct_test(test_parse_i8 test_parse_i8.c)
add_jstruct_test(test_parse_i32 test_parse_i32.c)
add_jstruct_test(test_parse_i64 test_parse_i64.c)
add_jstruct_test(test_parse_f32 test_parse_f32.c)
add_jstruct_test(test_parse_f64 test_parse_f64.c)
add_jstruct_test(test_parse_string test_parse_string.c)
add_jstruct_test(test_parse_object test_parse_object.c)
add_jstruct_test(test_parse_array test_parse_array.c)

# Serialize tests
add_jstruct_test(test_serialize_boolean test_serialize_boolean.c)
add_jstruct_test(test_serialize_u32 test_serialize_u32.c)
add_jstruct_test(test_serialize_i32 test_serialize_i32.c)
add_jstruct_test(test_serialize_f64 test_serialize_f64.c)
add_jstruct_test(test_serialize_string test_serialize_string.c)
add_jstruct_test(test_serialize_object test_serialize_object.c)
add_jstruct_test(test_serialize_array test_serialize_array.c)
