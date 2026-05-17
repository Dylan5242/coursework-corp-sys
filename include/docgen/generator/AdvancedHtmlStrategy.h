#pragma once

#include "docgen/generator/GenerationStrategy.h"

namespace docgen {

class AdvancedHtmlStrategy : public GenerationStrategy {
public:
    void generate(const DocumentationModel& model, const std::filesystem::path& outputPath) const override;
};

} // namespace docgen
