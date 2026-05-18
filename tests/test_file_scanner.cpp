#include <catch2/catch_test_macros.hpp>

#include "docgen/filesystem/FileScanner.h"
#include "test_helpers.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

using namespace docgen;

namespace {
std::vector<std::string> fileNames(const std::vector<std::filesystem::path>& paths) {
    std::vector<std::string> names;
    for (const auto& path : paths) {
        names.push_back(path.filename().string());
    }
    std::sort(names.begin(), names.end());
    return names;
}
}

TEST_CASE("FileScanner throws for missing input path", "[filesystem][scanner]") {
    FileScanner scanner;
    REQUIRE_THROWS_AS(scanner.scan("/path/that/does/not/exist", "auto"), std::runtime_error);
}

TEST_CASE("FileScanner accepts a single cpp file in auto mode", "[filesystem][scanner]") {
    TempDirectory temp;
    const auto file = temp.writeFile("main.cpp", "int main() { return 0; }");
    FileScanner scanner;
    const auto result = scanner.scan(file, "auto");
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == file);
}

TEST_CASE("FileScanner ignores unsupported single file in auto mode", "[filesystem][scanner]") {
    TempDirectory temp;
    const auto file = temp.writeFile("notes.txt", "text");
    FileScanner scanner;
    REQUIRE(scanner.scan(file, "auto").empty());
}

TEST_CASE("FileScanner recursively finds supported files", "[filesystem][scanner]") {
    TempDirectory temp;
    temp.writeFile("a.cpp", "");
    temp.writeFile("b.h", "");
    temp.writeFile("nested/c.hpp", "");
    temp.writeFile("nested/d.cc", "");
    temp.writeFile("nested/e.cxx", "");
    temp.writeFile("nested/readme.md", "");

    FileScanner scanner;
    const auto result = scanner.scan(temp.path(), "auto");
    REQUIRE(result.size() == 5);

    const auto names = fileNames(result);
    REQUIRE(std::find(names.begin(), names.end(), "a.cpp") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "b.h") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "c.hpp") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "d.cc") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "e.cxx") != names.end());
}

TEST_CASE("FileScanner filters by requested extension", "[filesystem][scanner]") {
    TempDirectory temp;
    temp.writeFile("a.cpp", "");
    temp.writeFile("b.h", "");
    temp.writeFile("c.h", "");

    FileScanner scanner;
    const auto result = scanner.scan(temp.path(), ".h");
    REQUIRE(result.size() == 2);
    const auto names = fileNames(result);
    REQUIRE(names[0] == "b.h");
    REQUIRE(names[1] == "c.h");
}

TEST_CASE("FileScanner returns sorted paths", "[filesystem][scanner]") {
    TempDirectory temp;
    temp.writeFile("z.cpp", "");
    temp.writeFile("a.cpp", "");
    temp.writeFile("m.cpp", "");

    FileScanner scanner;
    const auto result = scanner.scan(temp.path(), ".cpp");
    REQUIRE(result.size() == 3);
    REQUIRE(result[0].filename().string() == "a.cpp");
    REQUIRE(result[1].filename().string() == "m.cpp");
    REQUIRE(result[2].filename().string() == "z.cpp");
}

TEST_CASE("FileScanner returns empty list for empty directory", "[filesystem][scanner]") {
    TempDirectory temp;
    FileScanner scanner;
    REQUIRE(scanner.scan(temp.path(), "auto").empty());
}

TEST_CASE("FileScanner returns empty list for unsupported requested extension", "[filesystem][scanner]") {
    TempDirectory temp;
    temp.writeFile("a.cpp", "");
    FileScanner scanner;
    REQUIRE(scanner.scan(temp.path(), ".txt").empty());
}
