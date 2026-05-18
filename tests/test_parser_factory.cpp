#include <catch2/catch_test_macros.hpp>

#include "docgen/parser/CppParser.h"
#include "docgen/parser/HeaderParser.h"
#include "docgen/parser/ParserFactory.h"

using namespace docgen;

TEST_CASE("ParserFactory creates CppParser for cpp extension", "[parser][factory]") {
    auto parser = ParserFactory::createParser(".cpp");
    REQUIRE(parser != nullptr);
    REQUIRE(dynamic_cast<CppParser*>(parser.get()) != nullptr);
}

TEST_CASE("ParserFactory creates CppParser for alternative source extensions", "[parser][factory]") {
    REQUIRE(dynamic_cast<CppParser*>(ParserFactory::createParser(".cc").get()) != nullptr);
    REQUIRE(dynamic_cast<CppParser*>(ParserFactory::createParser(".cxx").get()) != nullptr);
}

TEST_CASE("ParserFactory creates HeaderParser for h extension", "[parser][factory]") {
    auto parser = ParserFactory::createParser(".h");
    REQUIRE(parser != nullptr);
    REQUIRE(dynamic_cast<HeaderParser*>(parser.get()) != nullptr);
}

TEST_CASE("ParserFactory creates HeaderParser for hpp extension", "[parser][factory]") {
    auto parser = ParserFactory::createParser(".hpp");
    REQUIRE(parser != nullptr);
    REQUIRE(dynamic_cast<HeaderParser*>(parser.get()) != nullptr);
}

TEST_CASE("ParserFactory returns null for unsupported extension", "[parser][factory]") {
    REQUIRE(ParserFactory::createParser(".txt") == nullptr);
}

TEST_CASE("ParserFactory is case-sensitive and rejects uppercase extension", "[parser][factory]") {
    REQUIRE(ParserFactory::createParser(".CPP") == nullptr);
}

TEST_CASE("ParserFactory returns null for empty extension", "[parser][factory]") {
    REQUIRE(ParserFactory::createParser("") == nullptr);
}
