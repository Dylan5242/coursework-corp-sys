#include "docgen/generator/GenerationStrategyFactory.h"
#include "docgen/generator/AdvancedHtmlStrategy.h"
#include "docgen/generator/SimpleHtmlStrategy.h"

namespace docgen {

std::unique_ptr<GenerationStrategy> GenerationStrategyFactory::create(const std::string& type) {
    if (type == "advanced") {
        return std::make_unique<AdvancedHtmlStrategy>();
    }

    return std::make_unique<SimpleHtmlStrategy>();
}

} // namespace docgen
