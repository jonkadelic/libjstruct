cmake_minimum_required(VERSION 3.21)
project(jstruct VERSION 0.1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON-C REQUIRED IMPORTED_TARGET json-c)

# Define library
add_library(jstruct)

# Set include directories
target_include_directories(jstruct
    PUBLIC       
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
 
# Link dependencies
target_link_libraries(jstruct
    PRIVATE
        PkgConfig::JSON-C
)

# Add source files
target_sources(jstruct PRIVATE
    src/parse.c
    src/serialize.c
)

# Add header files (for IDE support)
target_sources(jstruct PUBLIC FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/jstruct/jstruct.h
        include/jstruct/jstruct_parse.h
        include/jstruct/jstruct_serialize.h
)

# Installation rules
include(GNUInstallDirs)
install(TARGETS jstruct
    EXPORT jstructTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT jstructTargets
    FILE jstructTargets.cmake
    NAMESPACE jstruct::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jstruct
)

# Testing
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
