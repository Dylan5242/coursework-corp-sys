#include <catch2/catch_test_macros.hpp>

#include "docgen/generator/AdvancedHtmlStrategy.h"
#include "docgen/generator/GenerationStrategyFactory.h"
#include "docgen/generator/SimpleHtmlStrategy.h"
#include "test_helpers.h"

using namespace docgen;

namespace {
DocumentationModel sampleModel() {
    DocumentationModel model;
    FileDocumentation file;
    file.fileName = "math.cpp";
    file.filePath = "math.cpp";
    file.functions.push_back({"add", "int add(int a, int b);", "Adds numbers"});
    model.files.push_back(file);
    return model;
}
}

TEST_CASE("GenerationStrategyFactory creates advanced strategy", "[generator][strategy]") {
    auto strategy = GenerationStrategyFactory::create("advanced");
    REQUIRE(strategy != nullptr);
    REQUIRE(dynamic_cast<AdvancedHtmlStrategy*>(strategy.get()) != nullptr);
}

TEST_CASE("GenerationStrategyFactory creates simple strategy for simple type", "[generator][strategy]") {
    auto strategy = GenerationStrategyFactory::create("simple");
    REQUIRE(strategy != nullptr);
    REQUIRE(dynamic_cast<SimpleHtmlStrategy*>(strategy.get()) != nullptr);
}

TEST_CASE("GenerationStrategyFactory falls back to simple strategy for unknown type", "[generator][strategy]") {
    auto strategy = GenerationStrategyFactory::create("unknown");
    REQUIRE(strategy != nullptr);
    REQUIRE(dynamic_cast<SimpleHtmlStrategy*>(strategy.get()) != nullptr);
}

TEST_CASE("SimpleHtmlStrategy creates index html", "[generator][strategy]") {
    TempDirectory temp;
    SimpleHtmlStrategy strategy;
    strategy.generate(sampleModel(), temp.path() / "docs");

    const auto index = temp.path() / "docs" / "index.html";
    REQUIRE(std::filesystem::exists(index));
    const auto html = readTextFile(index);
    REQUIRE(html.find("Документация проекта") != std::string::npos);
    REQUIRE(html.find("Adds numbers") != std::string::npos);
}

TEST_CASE("AdvancedHtmlStrategy creates index html with navigation", "[generator][strategy]") {
    TempDirectory temp;
    AdvancedHtmlStrategy strategy;
    strategy.generate(sampleModel(), temp.path() / "docs");

    const auto html = readTextFile(temp.path() / "docs" / "index.html");
    REQUIRE(html.find("Расширенная документация проекта") != std::string::npos);
    REQUIRE(html.find("<nav>") != std::string::npos);
    REQUIRE(html.find("math.cpp") != std::string::npos);
}

TEST_CASE("Generation strategies create nested output directories", "[generator][strategy]") {
    TempDirectory temp;
    SimpleHtmlStrategy strategy;
    const auto output = temp.path() / "nested" / "docs";
    strategy.generate(sampleModel(), output);
    REQUIRE(std::filesystem::exists(output / "index.html"));
}

TEST_CASE("Generation strategies escape html in output", "[generator][strategy]") {
    DocumentationModel model;
    FileDocumentation file;
    file.fileName = "unsafe.cpp";
    file.functions.push_back({"unsafe", "void unsafe();", "Uses <tag> & data"});
    model.files.push_back(file);

    TempDirectory temp;
    SimpleHtmlStrategy strategy;
    strategy.generate(model, temp.path() / "docs");

    const auto html = readTextFile(temp.path() / "docs" / "index.html");
    REQUIRE(html.find("Uses &lt;tag&gt; &amp; data") != std::string::npos);
}

TEST_CASE("Generation strategies handle empty documentation model", "[generator][strategy]") {
    TempDirectory temp;
    SimpleHtmlStrategy strategy;
    DocumentationModel model;
    strategy.generate(model, temp.path() / "docs");
    REQUIRE(std::filesystem::exists(temp.path() / "docs" / "index.html"));
}
