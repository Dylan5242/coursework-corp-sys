#pragma once

#include <memory>
#include <string>
#include "docgen/generator/GenerationStrategy.h"

namespace docgen {

class GenerationStrategyFactory {
public:
    static std::unique_ptr<GenerationStrategy> create(const std::string& type);
};

} // namespace docgen
