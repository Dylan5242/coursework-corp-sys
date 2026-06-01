#include <catch2/catch_session.hpp>

// Shared test entry point for the lightweight Catch-compatible runner.
int main(int argc, char* argv[]) {
    return Catch::Session().run(argc, argv);
}
