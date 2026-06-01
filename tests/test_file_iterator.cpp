#include <catch2/catch_test_macros.hpp>

#include "docgen/filesystem/FileIterator.h"

using namespace docgen;

// Unit tests for FileIterator collection state and traversal behavior.
TEST_CASE("FileIterator reports empty collection", "[filesystem][iterator]") {
    FileIterator iterator({});
    REQUIRE(iterator.empty());
    REQUIRE(iterator.size() == 0);
}

TEST_CASE("FileIterator reports non-empty collection", "[filesystem][iterator]") {
    FileIterator iterator({"a.cpp"});
    REQUIRE_FALSE(iterator.empty());
    REQUIRE(iterator.size() == 1);
}

TEST_CASE("FileIterator preserves file order", "[filesystem][iterator]") {
    FileIterator iterator({"first.cpp", "second.h", "third.hpp"});
    auto current = iterator.begin();
    REQUIRE(current->string() == "first.cpp");
    ++current;
    REQUIRE(current->string() == "second.h");
    ++current;
    REQUIRE(current->string() == "third.hpp");
}

TEST_CASE("FileIterator can be used in range based for", "[filesystem][iterator]") {
    FileIterator iterator({"a.cpp", "b.h"});
    std::size_t count = 0;
    for (const auto& path : iterator) {
        REQUIRE_FALSE(path.empty());
        ++count;
    }
    REQUIRE(count == 2);
}

TEST_CASE("FileIterator begin equals end for empty list", "[filesystem][iterator]") {
    FileIterator iterator({});
    REQUIRE(iterator.begin() == iterator.end());
}
