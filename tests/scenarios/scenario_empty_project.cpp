#include "docgen/core/DocumentationGenerator.h"
#include "scenario_helpers.h"

#include <iostream>
#include <stdexcept>

// Scenario test for reporting an empty project as an expected failure case.
int main() {
    ScenarioTempDirectory temp("docgen_scenario_empty");

    docgen::DocumentationGenerator generator;
    try {
        generator.generate(temp.path(), temp.path() / "docs", "auto", "simple");
    } catch (const std::runtime_error&) {
        std::cout << "Empty project scenario passed\n";
        return 0;
    }

    return scenarioFail("generator should reject an empty project");
}
