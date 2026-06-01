#include <catch2/catch_test_macros.hpp>

#include "docgen/parser/CppParser.h"
#include "docgen/parser/HeaderParser.h"
#include "test_helpers.h"

using namespace docgen;

// Unit tests for extracting documented C++ entities from source and header files.
TEST_CASE("CppParser extracts documented function after slash comment", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("math.cpp", "/// Adds two numbers\nint add(int a, int b);\n");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.fileName == "math.cpp");
    REQUIRE(documentation.functions.size() == 1);
    REQUIRE(documentation.functions[0].name == "add");
    REQUIRE(documentation.functions[0].comment == "Adds two numbers");
}

TEST_CASE("CppParser joins multiple slash comment lines", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("api.cpp", "/// First line\n/// Second line\nvoid run();\n");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.functions.size() == 1);
    REQUIRE(documentation.functions[0].comment == "First line\nSecond line");
}

TEST_CASE("CppParser extracts class after block comment", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("parser.h", "/**\n * Parser class\n */\nclass Parser {\n};\n");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.classes.size() == 1);
    REQUIRE(documentation.classes[0].name == "Parser");
    REQUIRE(documentation.classes[0].comment == "Parser class");
}

TEST_CASE("CppParser extracts struct as documented class entity", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("token.h", "/// Token data\nstruct Token {\n};\n");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.classes.size() == 1);
    REQUIRE(documentation.classes[0].name == "Token");
}

TEST_CASE("CppParser ignores undocumented entities", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("plain.cpp", "int hidden();\nclass Hidden {};\n");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.functions.empty());
    REQUIRE(documentation.classes.empty());
}

TEST_CASE("CppParser handles empty files", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("empty.cpp", "");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.fileName == "empty.cpp");
    REQUIRE(documentation.functions.empty());
    REQUIRE(documentation.classes.empty());
}

TEST_CASE("CppParser handles missing files without throwing", "[parser][cpp]") {
    TempDirectory temp;
    const auto missing = temp.path() / "missing.cpp";

    CppParser parser;
    const auto documentation = parser.parse(missing);

    REQUIRE(documentation.fileName == "missing.cpp");
    REQUIRE(documentation.functions.empty());
    REQUIRE(documentation.classes.empty());
}

TEST_CASE("CppParser collects multiline function signature", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("multi.cpp", "/// Multiline declaration\nint\naddNumbers(int a,\n           int b);\n");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.functions.size() == 1);
    REQUIRE(documentation.functions[0].name == "addNumbers");
    REQUIRE(documentation.functions[0].signature == "int addNumbers(int a, int b);");
}

TEST_CASE("CppParser ignores control statements after comments", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("control.cpp", "/// not a function\nif (value) {\n}\n");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.functions.empty());
    REQUIRE(documentation.classes.empty());
}

TEST_CASE("CppParser extracts several documented entities from one file", "[parser][cpp]") {
    TempDirectory temp;
    const auto file = temp.writeFile("many.cpp", "/// A class\nclass A {};\n\n/// first function\nvoid first();\n\n/// second function\nint second(int value);\n");

    CppParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.classes.size() == 1);
    REQUIRE(documentation.functions.size() == 2);
    REQUIRE(documentation.functions[0].name == "first");
    REQUIRE(documentation.functions[1].name == "second");
}

TEST_CASE("HeaderParser reuses cpp parsing rules for header files", "[parser][header]") {
    TempDirectory temp;
    const auto file = temp.writeFile("api.h", "/// Public API\nvoid api();\n");

    HeaderParser parser;
    const auto documentation = parser.parse(file);

    REQUIRE(documentation.fileName == "api.h");
    REQUIRE(documentation.functions.size() == 1);
    REQUIRE(documentation.functions[0].name == "api");
}
