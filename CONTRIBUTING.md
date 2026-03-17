# Contributing to libjstruct

Thank you for considering contributing to libjstruct. This document outlines the guidelines and processes for contributing to the project.

## Getting Started

### Development Environment

1. **Clone the repository**
   ```bash
   git clone https://github.com/jonkadelic/libjstruct.git
   cd libjstruct
   ```

2. **Install dependencies**
   - C23-compatible compiler (GCC 15.2.1+ or Clang with C23 support)
   - CMake 3.21 or later
   - json-c development headers
   - pkg-config

   On Ubuntu/Debian:
   ```bash
   sudo apt install build-essential cmake libjson-c-dev pkg-config
   ```

3. **Build the project**
   ```bash
   cmake -B build
   cmake --build build
   ```

4. **Run tests**
   ```bash
   cd build
   ctest --output-on-failure
   ```

### IDE Setup

The project includes configuration for clangd. To generate compile commands:
```bash
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

## Code Style

### C Code Style

- **Standard**: C23
- **Indentation**: 4 spaces (no tabs)
- **Naming conventions**:
  - Types: `snake_case_t` suffix
  - Functions: `jstruct_` prefix with `snake_case`
  - Enum constants: `JSTRUCT_UPPER_CASE`
  - Local variables: `snake_case`
- **Braces**: K&R style (opening brace on same line)
- **Line length**: Aim for 120 characters maximum
- **Comments**: Use `//` for single-line, `/* */` for multi-line
- **Documentation**: Doxygen-style comments for public APIs

### Example

```c
/**
 * @brief Brief description of function.
 *
 * Detailed description if needed.
 *
 * @param value Description of parameter
 * @return Description of return value
 */
bool jstruct_example_function(jstruct_value_t const* value) {
    if (value == nullptr) {
        return false;
    }
    
    // Implementation
    return true;
}
```

## Making Changes

### Branch Naming

- Feature branches: `feature/descriptive-name`
- Bug fixes: `fix/issue-description`
- Documentation: `docs/what-changed`

### Commit Messages

Write clear, concise commit messages:

```
Short summary (50 chars or less)

More detailed explanatory text if necessary. Wrap at 72 characters.
Explain the problem this commit solves and why you chose this approach.

- Bullet points are acceptable
- Use present tense ("Add feature" not "Added feature")
- Reference issues: Fixes #123
```

### Testing Requirements

All code changes must include appropriate tests:

1. **Unit tests**: Add tests in the `tests/` directory
2. **Test naming**: `test_<component>_<specific_case>.c`
3. **Coverage**: Aim for comprehensive coverage of new code paths
4. **All tests must pass**: Run `ctest` before submitting

#### Writing Tests

Tests use the Unity framework. Example:

```c
#include <unity.h>
#include "jstruct/jstruct.h"

void setUp(void) { }
void tearDown(void) { }

void test_example_case(void) {
    // Arrange
    int expected = 42;
    
    // Act
    int actual = some_function();
    
    // Assert
    TEST_ASSERT_EQUAL(expected, actual);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_example_case);
    return UNITY_END();
}
```

Add your test to `tests/CMakeLists.txt`:

```cmake
add_jstruct_test(test_example test_example.c)
```

### Documentation

- Update documentation for all public API changes
- Add Doxygen comments for new public functions
- Update README.md if adding new features
- Update CHANGELOG.md following Keep a Changelog format

## Pull Request Process

1. **Fork the repository** and create your branch from `main`

2. **Make your changes** following the guidelines above

3. **Test thoroughly**
   ```bash
   cmake -B build
   cmake --build build
   cd build && ctest --output-on-failure
   ```

4. **Update documentation**
   - Add/update Doxygen comments
   - Update README.md if needed
   - Add entry to CHANGELOG.md under "Unreleased"

5. **Submit pull request**
   - Provide a clear description of the changes
   - Reference any related issues
   - Ensure CI passes (if configured)

6. **Code review**
   - Address reviewer feedback
   - Keep the PR focused on a single concern
   - Be responsive to comments

### Pull Request Checklist

- [ ] Code follows the project style guidelines
- [ ] All tests pass locally
- [ ] New tests added for new functionality
- [ ] Documentation updated
- [ ] CHANGELOG.md updated
- [ ] Commit messages are clear and descriptive
- [ ] No unnecessary changes or reformatting

## Reporting Issues

### Bug Reports

When reporting bugs, include:

1. **Description**: Clear description of the bug
2. **Reproduction steps**: Minimal code to reproduce the issue
3. **Expected behavior**: What you expected to happen
4. **Actual behavior**: What actually happened
5. **Environment**:
   - OS and version
   - Compiler and version
   - json-c version
   - libjstruct version

### Feature Requests

When requesting features, include:

1. **Use case**: Describe the problem you're trying to solve
2. **Proposed solution**: How you envision it working
3. **Alternatives**: Other approaches you've considered
4. **Examples**: Code examples if applicable

## Code Review Process

- Maintainers will review pull requests as time permits
- Feedback may be provided as comments or suggestions
- Changes may be requested before merging
- Be patient and respectful in discussions

## Community Guidelines

- Be respectful and constructive
- Focus on the code, not the person
- Accept constructive criticism gracefully
- Help others learn and grow

## License

By contributing to libjstruct, you agree that your contributions will be licensed under the same license as the project.

## Questions?

If you have questions not covered here, feel free to:
- Open an issue for discussion
- Reach out to the maintainers

Thank you for contributing to libjstruct!
