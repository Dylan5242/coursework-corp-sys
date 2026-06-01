#include "docgen/core/DocumentationGenerator.h"
#include "scenario_helpers.h"

#include <filesystem>
#include <iostream>

// Scenario test for the simple documentation generation workflow.
int main() {
    ScenarioTempDirectory temp("docgen_scenario_simple");
    temp.writeFile("src/math.cpp", "/// Adds two numbers\nint add(int a, int b);\n");

    docgen::DocumentationGenerator generator;
    generator.generate(temp.path() / "src", temp.path() / "docs", "auto", "simple");

    const auto index = temp.path() / "docs" / "index.html";
    if (!std::filesystem::exists(index)) {
        return scenarioFail("index.html was not created");
    }

    const auto html = scenarioReadFile(index);
    if (html.find("Adds two numbers") == std::string::npos) {
        return scenarioFail("function comment was not found in generated HTML");
    }

    std::cout << "Simple generation scenario passed\n";
    return 0;
}
