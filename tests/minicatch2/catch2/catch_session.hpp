#pragma once

#include "catch2/catch_test_macros.hpp"

namespace Catch {

class Session {
public:
    int run(int, char*[]) const {
        int failed = 0;
        int passed = 0;

        for (const auto& test : getRegistry().tests()) {
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

        std::cout << "\nTests passed: " << passed << '\n'
                  << "Tests failed: " << failed << '\n';

        return failed == 0 ? 0 : 1;
    }
};

} // namespace Catch
