#include <catch2/catch_test_macros.hpp>

#include "docgen/core/DocumentationGenerator.h"
#include "docgen/observer/IGenerationObserver.h"
#include "test_helpers.h"

#include <stdexcept>
#include <vector>

using namespace docgen;

// End-to-end unit tests for scanning, parsing, generation, and progress events.
class RecordingObserver final : public IGenerationObserver {
public:
    void onProgress(const std::string& message) override {
        messages.push_back(message);
    }

    std::vector<std::string> messages;
};

TEST_CASE("DocumentationGenerator generates simple documentation end-to-end", "[core]") {
    TempDirectory temp;
    temp.writeFile("src/math.cpp", "/// Adds values\nint add(int a, int b);\n");

    DocumentationGenerator generator;
    generator.generate(temp.path() / "src", temp.path() / "docs", "auto", "simple");

    const auto html = readTextFile(temp.path() / "docs" / "index.html");
    REQUIRE(html.find("Документация проекта") != std::string::npos);
    REQUIRE(html.find("Adds values") != std::string::npos);
}

TEST_CASE("DocumentationGenerator generates advanced documentation end-to-end", "[core]") {
    TempDirectory temp;
    temp.writeFile("include/api.h", "/// API class\nclass Api {};\n");

    DocumentationGenerator generator;
    generator.generate(temp.path() / "include", temp.path() / "docs", "auto", "advanced");

    const auto html = readTextFile(temp.path() / "docs" / "index.html");
    REQUIRE(html.find("Расширенная документация проекта") != std::string::npos);
    REQUIRE(html.find("<nav>") != std::string::npos);
    REQUIRE(html.find("API class") != std::string::npos);
}

TEST_CASE("DocumentationGenerator sends progress notifications", "[core]") {
    TempDirectory temp;
    temp.writeFile("src/main.cpp", "/// Entry point\nint main();\n");

    RecordingObserver observer;
    DocumentationGenerator generator;
    generator.addObserver(&observer);
    generator.generate(temp.path() / "src", temp.path() / "docs", "auto", "simple");

    REQUIRE(observer.messages.size() >= 4);
    REQUIRE(observer.messages[0] == "Scanning files...");
    REQUIRE(observer.messages[1].find("Parsing:") != std::string::npos);
    REQUIRE(observer.messages[2] == "Generating HTML...");
    REQUIRE(observer.messages.back().find("Done") != std::string::npos);
}

TEST_CASE("DocumentationGenerator throws when no supported files found", "[core]") {
    TempDirectory temp;
    temp.writeFile("readme.txt", "no C++ files");

    DocumentationGenerator generator;
    REQUIRE_THROWS_AS(generator.generate(temp.path(), temp.path() / "docs", "auto", "simple"), std::runtime_error);
}

TEST_CASE("DocumentationGenerator throws for missing input path", "[core]") {
    TempDirectory temp;
    DocumentationGenerator generator;
    REQUIRE_THROWS_AS(generator.generate(temp.path() / "missing", temp.path() / "docs", "auto", "simple"), std::runtime_error);
}

TEST_CASE("DocumentationGenerator filters requested file type", "[core]") {
    TempDirectory temp;
    temp.writeFile("src/a.cpp", "/// Cpp function\nvoid cppOnly();\n");
    temp.writeFile("src/a.h", "/// Header function\nvoid headerOnly();\n");

    DocumentationGenerator generator;
    generator.generate(temp.path() / "src", temp.path() / "docs", ".h", "simple");

    const auto html = readTextFile(temp.path() / "docs" / "index.html");
    REQUIRE(html.find("Header function") != std::string::npos);
    REQUIRE(html.find("Cpp function") == std::string::npos);
}

TEST_CASE("DocumentationGenerator supports hpp headers", "[core]") {
    TempDirectory temp;
    temp.writeFile("include/config.hpp", "/// Config class\nclass Config {};\n");

    DocumentationGenerator generator;
    generator.generate(temp.path() / "include", temp.path() / "docs", "auto", "simple");

    const auto html = readTextFile(temp.path() / "docs" / "index.html");
    REQUIRE(html.find("Config class") != std::string::npos);
}
