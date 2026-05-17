#pragma once

#include <filesystem>
#include <string>
#include "docgen/observer/GenerationSubject.h"

namespace docgen {

class DocumentationGenerator {
public:
    void addObserver(IGenerationObserver* observer);

    void generate(
        const std::filesystem::path& inputPath,
        const std::filesystem::path& outputPath,
        const std::string& fileType,
        const std::string& generationType
    );

private:
    GenerationSubject subject_;
};

} // namespace docgen
