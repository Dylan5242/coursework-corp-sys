#pragma once

#include <filesystem>
#include "docgen/model/DocumentationModel.h"

namespace docgen {

class GenerationStrategy {
public:
    virtual void generate(const DocumentationModel& model, const std::filesystem::path& outputPath) const = 0;
    virtual ~GenerationStrategy() = default;
};

} // namespace docgen
