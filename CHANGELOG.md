# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.0] - 2026-03-17

### Added
- Core JSON parsing functionality with `jstruct_parse()`
- JSON serialization functionality with `jstruct_serialize()`
- Support for primitive types: boolean, integers (8/16/32/64-bit signed and unsigned), floats (32/64-bit)
- Support for complex types: strings, objects, arrays
- Custom type handler support for user-defined parsers and serializers
- Comprehensive error reporting with context information
- Detailed Doxygen documentation for all public APIs
- Complete test suite with 20 test cases covering parsing and serialization
- CMake build system with installation targets
- README.md with usage examples and API reference
- CONTRIBUTING.md with development guidelines
- Unity test framework integration with double precision support
- Type-safe declarative JSON mapping using compile-time descriptors
- Bounds checking and validation for numeric types
- Nested object support with arbitrary depth
- Array support with length tracking
- String buffer size validation

[Unreleased]: https://github.com/yourusername/libjstruct/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/yourusername/libjstruct/releases/tag/v0.1.0
