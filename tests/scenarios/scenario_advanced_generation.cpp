#include "docgen/core/DocumentationGenerator.h"
#include "scenario_helpers.h"

#include <filesystem>
#include <iostream>

int main() {
    ScenarioTempDirectory temp("docgen_scenario_advanced");
    temp.writeFile("include/api.h", "/// Main API class\nclass Api {};\n");
    temp.writeFile("src/main.cpp", "/// Starts application\nint start();\n");

    docgen::DocumentationGenerator generator;
    generator.generate(temp.path(), temp.path() / "docs", "auto", "advanced");

    const auto index = temp.path() / "docs" / "index.html";
    if (!std::filesystem::exists(index)) {
        return scenarioFail("index.html was not created");
    }

    const auto html = scenarioReadFile(index);
    if (html.find("<nav>") == std::string::npos) {
        return scenarioFail("advanced navigation was not found");
    }
    if (html.find("Main API class") == std::string::npos || html.find("Starts application") == std::string::npos) {
        return scenarioFail("not all documented entities were generated");
    }

    std::cout << "Advanced generation scenario passed\n";
    return 0;
}
