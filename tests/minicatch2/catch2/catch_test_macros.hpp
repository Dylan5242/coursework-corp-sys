#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Catch {

struct TestCase {
    std::string name;
    std::string tags;
    void (*function)();
};

class TestFailure : public std::runtime_error {
public:
    explicit TestFailure(const std::string& message)
        : std::runtime_error(message) {}
};

class Registry {
public:
    bool add(const std::string& name, const std::string& tags, void (*function)()) {
        tests_.push_back({name, tags, function});
        return true;
    }

    const std::vector<TestCase>& tests() const {
        return tests_;
    }

private:
    std::vector<TestCase> tests_;
};

inline Registry& getRegistry() {
    static Registry registry;
    return registry;
}

inline std::string makeFailureMessage(const char* expression, const char* file, int line) {
    std::ostringstream output;
    output << file << ':' << line << ": failed requirement: " << expression;
    return output.str();
}

inline void require(bool value, const char* expression, const char* file, int line) {
    if (!value) {
        throw TestFailure(makeFailureMessage(expression, file, line));
    }
}

} // namespace Catch

#define CATCH_INTERNAL_CONCAT_IMPL(left, right) left##right
#define CATCH_INTERNAL_CONCAT(left, right) CATCH_INTERNAL_CONCAT_IMPL(left, right)

#define TEST_CASE(name, tags) CATCH_INTERNAL_TEST_CASE(name, tags, __COUNTER__)
#define CATCH_INTERNAL_TEST_CASE(name, tags, id) \
    static void CATCH_INTERNAL_CONCAT(catch_test_, id)(); \
    namespace { \
    const bool CATCH_INTERNAL_CONCAT(catch_registered_, id) = \
        ::Catch::getRegistry().add(name, tags, &CATCH_INTERNAL_CONCAT(catch_test_, id)); \
    } \
    static void CATCH_INTERNAL_CONCAT(catch_test_, id)()

#define REQUIRE(expression) ::Catch::require(static_cast<bool>(expression), #expression, __FILE__, __LINE__)
#define CHECK(expression) REQUIRE(expression)
#define REQUIRE_FALSE(expression) ::Catch::require(!static_cast<bool>(expression), "FALSE(" #expression ")", __FILE__, __LINE__)
#define CHECK_FALSE(expression) REQUIRE_FALSE(expression)

#define REQUIRE_THROWS_AS(expression, exception_type) \
    do { \
        bool catch_threw_expected = false; \
        try { \
            (void)(expression); \
        } catch (const exception_type&) { \
            catch_threw_expected = true; \
        } catch (...) { \
            throw ::Catch::TestFailure(::Catch::makeFailureMessage("unexpected exception type for " #expression, __FILE__, __LINE__)); \
        } \
        if (!catch_threw_expected) { \
            throw ::Catch::TestFailure(::Catch::makeFailureMessage("expected exception: " #exception_type " from " #expression, __FILE__, __LINE__)); \
        } \
    } while (false)

#define REQUIRE_NOTHROW(expression) \
    do { \
        try { \
            (void)(expression); \
        } catch (const std::exception& catch_error) { \
            std::ostringstream catch_output; \
            catch_output << "unexpected exception from " #expression << ": " << catch_error.what(); \
            throw ::Catch::TestFailure(::Catch::makeFailureMessage(catch_output.str().c_str(), __FILE__, __LINE__)); \
        } catch (...) { \
            throw ::Catch::TestFailure(::Catch::makeFailureMessage("unexpected non-standard exception from " #expression, __FILE__, __LINE__)); \
        } \
    } while (false)
