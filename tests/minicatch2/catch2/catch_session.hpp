#pragma once

#include "catch2/catch_test_macros.hpp"

namespace Catch {

class Session {
public:
    int run(int argc, char* argv[]) const {
        const std::string selectedTest = argc > 1 ? argv[1] : "";
        int failed = 0;
        int passed = 0;
        int matched = 0;

        for (const auto& test : getRegistry().tests()) {
            if (!selectedTest.empty() && test.name != selectedTest) {
                continue;
            }

            ++matched;
            try {
                test.function();
                ++passed;
                std::cout << "[PASSED] " << test.name << '\n';
            } catch (const std::exception& error) {
                ++failed;
                std::cerr << "[FAILED] " << test.name << '\n'
                          << "         " << error.what() << '\n';
            } catch (...) {
                ++failed;
                std::cerr << "[FAILED] " << test.name << '\n'
                          << "         unknown exception\n";
            }
        }

        if (!selectedTest.empty() && matched == 0) {
            std::cerr << "Test case not found: " << selectedTest << '\n';
            return 1;
        }

        std::cout << "\nTests passed: " << passed << '\n'
                  << "Tests failed: " << failed << '\n';

        return failed == 0 ? 0 : 1;
    }
};

} // namespace Catch
