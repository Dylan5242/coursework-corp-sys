#include "docgen/core/DocumentationGenerator.h"
#include "scenario_helpers.h"

#include <iostream>
#include <stdexcept>

// Scenario test for ignoring unsupported files during documentation generation.
int main() {
    ScenarioTempDirectory temp("docgen_scenario_unsupported");
    temp.writeFile("README.md", "# Not a C++ source file\n");
    temp.writeFile("script.py", "print('hello')\n");

    docgen::DocumentationGenerator generator;
    try {
        generator.generate(temp.path(), temp.path() / "docs", "auto", "simple");
    } catch (const std::runtime_error&) {
        std::cout << "Unsupported files scenario passed\n";
        return 0;
    }

    return scenarioFail("generator should reject a project without supported C++ files");
}
