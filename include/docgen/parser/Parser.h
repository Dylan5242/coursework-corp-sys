#pragma once

#include <filesystem>
#include "docgen/model/FileDocumentation.h"

namespace docgen {

class Parser {
public:
    virtual FileDocumentation parse(const std::filesystem::path& filePath) const = 0;
    virtual ~Parser() = default;
};

} // namespace docgen
