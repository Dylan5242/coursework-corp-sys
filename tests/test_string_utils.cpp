#include <catch2/catch_test_macros.hpp>

#include "docgen/utils/StringUtils.h"

using namespace docgen;

// Unit tests for small string helpers used by parsers and HTML generation.
TEST_CASE("trim removes leading and trailing spaces", "[utils][trim]") {
    REQUIRE(utils::trim("  hello  ") == "hello");
}

TEST_CASE("trim removes tabs and new lines", "[utils][trim]") {
    REQUIRE(utils::trim("\t\n value \r\n") == "value");
}

TEST_CASE("trim returns empty string for whitespace-only input", "[utils][trim]") {
    REQUIRE(utils::trim("   \t\n") == "");
}

TEST_CASE("trim keeps internal spaces", "[utils][trim]") {
    REQUIRE(utils::trim("  hello   world  ") == "hello   world");
}

TEST_CASE("htmlEscape escapes special html characters", "[utils][html]") {
    REQUIRE(utils::htmlEscape("<tag a=\"1\">Tom & 'Bob'</tag>") == "&lt;tag a=&quot;1&quot;&gt;Tom &amp; &#39;Bob&#39;&lt;/tag&gt;");
}

TEST_CASE("htmlEscape keeps regular text unchanged", "[utils][html]") {
    REQUIRE(utils::htmlEscape("regular text 123") == "regular text 123");
}

TEST_CASE("getFileExtension returns extension with dot", "[utils][path]") {
    REQUIRE(utils::getFileExtension("src/main.cpp") == ".cpp");
}

TEST_CASE("removeCommentMarkers handles single line slash comments", "[utils][comments]") {
    REQUIRE(utils::removeCommentMarkers("/// Adds two numbers\n") == "Adds two numbers");
}

TEST_CASE("removeCommentMarkers handles multiple slash comments", "[utils][comments]") {
    REQUIRE(utils::removeCommentMarkers("/// First line\n/// Second line\n") == "First line\nSecond line");
}

TEST_CASE("removeCommentMarkers handles block comments", "[utils][comments]") {
    const std::string comment = "/**\n * Class description\n * second line\n */\n";
    REQUIRE(utils::removeCommentMarkers(comment) == "Class description\nsecond line");
}

TEST_CASE("extractFunctionName reads simple function name", "[utils][parse]") {
    REQUIRE(utils::extractFunctionName("int add(int a, int b);") == "add");
}

TEST_CASE("extractFunctionName removes namespace or class prefix", "[utils][parse]") {
    REQUIRE(utils::extractFunctionName("void Math::reset();") == "reset");
}

TEST_CASE("extractFunctionName returns fallback for invalid signature", "[utils][parse]") {
    REQUIRE(utils::extractFunctionName("not a function") == "unknown_function");
}

TEST_CASE("extractClassName reads class and struct names", "[utils][parse]") {
    REQUIRE(utils::extractClassName("class Parser {") == "Parser");
    REQUIRE(utils::extractClassName("struct Token;") == "Token");
}

TEST_CASE("extractClassName returns fallback for invalid signature", "[utils][parse]") {
    REQUIRE(utils::extractClassName("enum Color {") == "unknown_class");
}

TEST_CASE("isSupportedExtension accepts supported C++ extensions", "[utils][extensions]") {
    REQUIRE(utils::isSupportedExtension(".cpp"));
    REQUIRE(utils::isSupportedExtension(".h"));
    REQUIRE(utils::isSupportedExtension(".hpp"));
    REQUIRE(utils::isSupportedExtension(".cc"));
    REQUIRE(utils::isSupportedExtension(".cxx"));
}

TEST_CASE("isSupportedExtension rejects unsupported extensions", "[utils][extensions]") {
    REQUIRE_FALSE(utils::isSupportedExtension(".txt"));
    REQUIRE_FALSE(utils::isSupportedExtension("cpp"));
    REQUIRE_FALSE(utils::isSupportedExtension(""));
}
